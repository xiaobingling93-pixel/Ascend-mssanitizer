/* -------------------------------------------------------------------------
 * This file is part of the MindStudio project.
 * Copyright (c) 2025 Huawei Technologies Co.,Ltd.
 *
 * MindStudio is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 * ------------------------------------------------------------------------- */


#include "checker.h"
#include <atomic>
#include <thread>
#include <iostream>

#include "record_defs.h"
#include "record_format.h"
#include "runtime_context.h"
#include "sanitizer_base.h"
#include "record_pre_process.h"
#include "file_mapping.h"
#include "utility/ustring.h"
#include "utility/serializer.h"
#include "utility/umask_guard.h"
#include "utility/log.h"
#include "utility/cpp_future.h"

namespace Sanitizer {

Checker::Checker(Config const &config) : config_(config)
{
    // 后续依据不同的工具选择使能不同的sanitizer功能
    auto rootTid = RuntimeContext::Instance().rootTid_;

    auto createCheckerAndThread = [this, rootTid](ToolType toolType, int index, bool enabled) {
        if (enabled) {
            sanitizerArr_[index] = SanitizerFactory::GetInstance().Create(toolType);
            if (sanitizerArr_[index]) {
                workers_.push_back(std::thread(&Checker::ConsumeRecordThread, this, index, rootTid));
                done_[index] = true;
            }
        }
    };
    createCheckerAndThread(ToolType::MEMCHECK, 0, config.defaultCheck);
    createCheckerAndThread(ToolType::RACECHECK, 1, config.raceCheck);
    createCheckerAndThread(ToolType::SYNCCHECK, 2, config.syncCheck);
}

Checker::~Checker()
{
    try {
        this->Finish();
    } catch (const std::exception &ex) {
        SAN_WARN_LOG("Got internel exception %s when checker::finish", ex.what());
    }
}

inline void Checker::WaitAfterConsumed(uint8_t consumeId)
{
    {
        std::lock_guard<std::mutex> lock(mtx_[consumeId]);
        if (workerArgs_[consumeId].empty()) {
            done_[consumeId] = true;
        }
    }
    producerCv_.notify_one();
}

inline bool Checker::IsNeedFilterDbi(const SanitizerRecord &record, uint8_t toolIdx)
{
    /// host侧内存记录全部默认处理
    if (record.version == RecordVersion::MEMORY_RECORD) {
        return false;
    }

    /// 静态插桩默认处理
    if (!isKernelWithDBI_) {
        return false;
    }

    /// 动态插桩不处理racecheck和synccheck
    if ((toolIdx == static_cast<uint8_t>(Sanitizer::ToolType::RACECHECK) && config_.raceCheck) ||
        (toolIdx == static_cast<uint8_t>(Sanitizer::ToolType::SYNCCHECK) && config_.syncCheck)) {
        return true;
    }
    return false;
}

inline bool IsTargetBlockIdx(DeviceType deviceType, int16_t checkBlockId, uint64_t blockIdx)
{
    if (HasSubBlocks(deviceType)) {
        uint64_t vecTargetBlockIdx = checkBlockId / C220_VEC_SUB_BLOCKDIM * C220_MIX_SUB_BLOCKDIM +
            checkBlockId % C220_VEC_SUB_BLOCKDIM;
        uint64_t cubeTargetBlockIdx = checkBlockId * C220_MIX_SUB_BLOCKDIM + C220_VEC_SUB_BLOCKDIM;
        return blockIdx == vecTargetBlockIdx || blockIdx == cubeTargetBlockIdx;
    } else {
        return blockIdx == static_cast<uint64_t>(checkBlockId);
    }
}

bool Checker::SupportSimt() const
{
    return this->deviceType_ >= DeviceType::ASCEND_910_950z && this->deviceType_ <= DeviceType::ASCEND_910_9599;
}

bool Checker::IsTargetBlock(uint64_t blockIdx) const
{
    return this->config_.checkBlockId == CHECK_ALL_BLOCK ||
        IsTargetBlockIdx(deviceType_, config_.checkBlockId, blockIdx);
}

void Checker::ConsumeRecordThread(uint8_t consumeId, const std::thread::id &rootTid)
{
    RuntimeContext().Instance().rootTid_ = rootTid;
    auto &que = workerArgs_[consumeId];
    while (true) {
        WorkArgs args;
        if (finishProduce_ && !que.empty()) {
            args = que.front();
            que.pop();
        } else {
            std::unique_lock<std::mutex> lock(mtx_[consumeId]);
            workerCv_.wait(lock, [&que] () { return !que.empty(); });
            args = que.front();
            que.pop();
        }
        switch (args.eventType) {
            case BroadcastEvent::SANITIZER_RECORD_ARRIVED:
                break;
            case BroadcastEvent::DEVICE_INFO_UPDATED: {
                RuntimeContext::Instance().deviceSummary_ = args.deviceInfo;
                continue;
            }
            case BroadcastEvent::KERNEL_SUMMARY_UPDATED: {
                RuntimeContext::Instance().kernelSummary_ = args.kernelSummary;
                continue;
            }
            case BroadcastEvent::STOP:
                // clear queue
                std::queue<WorkArgs>().swap(que);
                return;
            default:
                continue;
        };

        const auto &record = args.record;
        const auto &events = args.event;
        if ((initWithDeviceInfoDone_[consumeId] ||
            initWithKernelInfoDone_[consumeId]) &&
            sanitizerArr_[consumeId]->CheckRecordBeforeProcess(record)) {
            sanitizerArr_[consumeId]->Do(events);
        }
        if (finishProduce_) {
            WaitAfterConsumed(consumeId);
        }
    };
}

void Checker::SetDeviceInfo(const DeviceInfoSummary &deviceInfo)
{
    this->deviceType_ = deviceInfo.device;
    for (uint8_t i = 0; i < TOOL_NUM; ++i) {
        if (sanitizerArr_[i] != nullptr) {
            bool initDone = sanitizerArr_[i]->SetDeviceInfo(deviceInfo, config_);
            initWithDeviceInfoDone_[i] = initDone;
            std::lock_guard<std::mutex> lock(mtx_[i]);
            workerArgs_[i].push({ BroadcastEvent::DEVICE_INFO_UPDATED, {}, {}, deviceInfo, {} });
        }
    }
    workerCv_.notify_all();
}

void Checker::SetKernelInfo(const KernelSummary &kernelInfo)
{
    isKernelWithDBI_ = kernelInfo.isKernelWithDBI;
    printMissDebugLine_ = true;
    for (uint8_t i = 0; i < TOOL_NUM; ++i) {
        if (sanitizerArr_[i] != nullptr) {
            bool initDone = sanitizerArr_[i]->SetKernelInfo(kernelInfo);
            initWithKernelInfoDone_[i] = initDone;
            workerArgs_[i].push({ BroadcastEvent::KERNEL_SUMMARY_UPDATED, {}, {}, {}, kernelInfo });
        }
    }
}

void Checker::TryPrintMissDebugLine()
{
    // 每个 kernel 出现第一条异常，并且缺失 debugline 信息时，打印提示
    const bool printMissDebugLine = printMissDebugLine_.exchange(false);
    if (printMissDebugLine && !RuntimeContext::Instance().kernelSummary_.hasDebugLine) {
        std::string kernelName(RuntimeContext::Instance().kernelSummary_.kernelName);
        if (kernelName.empty()) { kernelName = "Unknown"; }
        printf("[mssanitizer] WARN: Kernel %s missed debug_line information. Please recompile kernel "
               "with `-g' or `-gline-tables-only' option to enable callstack display.\n", kernelName.c_str());
    }
}

void Checker::SetDetectionInfo(const LogLv &expectLv, std::ostream &detectionOstream)
{
    auto func = [expectLv, &detectionOstream, this](const LogLv &lv, SanitizerBase::MSG_GEN &&gen) {
        if (lv >= expectLv) {
            TryPrintMissDebugLine();
            std::lock_guard<std::mutex> guard(detMutex_);
            detectionOstream << gen().message << std::flush;
        }
    };
    for (uint8_t i = 0; i < TOOL_NUM; ++i) {
        if (sanitizerArr_[i] != nullptr) {
            sanitizerArr_[i]->RegisterNotifyFunc(func);
        }
    }
    return;
}

void Checker::ParseOnlineError(const SanitizerRecord &record)
{
    uint8_t memCheckIdx = static_cast<uint8_t>(ToolType::MEMCHECK);
    if (sanitizerArr_[memCheckIdx] != nullptr) {
        const auto &kernelRecord = record.payload.kernelRecord;
        sanitizerArr_[memCheckIdx]->ParseOnlineError(kernelRecord.payload.kernelErrorRecord,
            kernelRecord.blockType, kernelRecord.serialNo);
        std::stringstream ss;
        ss << record << ", deviceId:" << RuntimeContext::Instance().GetDeviceId();
        SAN_LOG("%s", ss.str().c_str());
    }
}

void Checker::Do(const SanitizerRecord &record)
{
    std::lock_guard<std::mutex> lk(doMutex_);
     /// 解析在线检测的异常结果
    if (record.version == RecordVersion::KERNEL_RECORD &&
        record.payload.kernelRecord.recordType == RecordType::MEM_ERROR) {
        ParseOnlineError(record);
        return;
    }
    std::vector<SanEvent> events;
    RecordPreProcess::GetInstance().Process(record, events);
    finishProduce_ = false;
    for (uint8_t i = 0; i < TOOL_NUM; ++i) {
        if (IsNeedFilterDbi(record, i)) {
            continue;
        }
        if (sanitizerArr_[i] != nullptr) {
            std::lock_guard<std::mutex> lock(mtx_[i]);
            done_[i] = false;
            workerArgs_[i].push({
                BroadcastEvent::SANITIZER_RECORD_ARRIVED,
                record,
                events,
                {}
            });
        }
    }
    if (record.payload.kernelRecord.recordType != RecordType::FINISH) {
        workerCv_.notify_all();
    } else {
        finishProduce_ = true;
        workerCv_.notify_all();
        for (uint8_t i = 0; i < TOOL_NUM; ++i) {
            if (sanitizerArr_[i] == nullptr) {
                continue;
            }
            if (IsNeedFilterDbi(record, i)) {
                continue;
            }
            std::unique_lock<std::mutex> lock(mtx_[i]);
            producerCv_.wait(lock, [this, i] () { return done_[i]; });
            printMissDebugLine_ = false;
        }
    }

    std::stringstream ss;
    ss << record << ", deviceId:" << RuntimeContext::Instance().GetDeviceId();
    SAN_LOG("%s", ss.str().c_str());
}

void Checker::Finish()
{
    if (!workers_.empty()) {
        for (uint8_t i = 0; i < TOOL_NUM; ++i) {
            if (sanitizerArr_[i] != nullptr) {
                std::lock_guard<std::mutex> lock(mtx_[i]);
                workerArgs_[i].push({ BroadcastEvent::STOP, {}, {}, {} });
            }
        }
        workerCv_.notify_all();
        for (auto &worker: workers_) {
            worker.join();
        }
        workers_.clear();
    }
    // dump mem leak errors
    for (uint8_t i = 0; i < TOOL_NUM; ++i) {
        if (sanitizerArr_[i] != nullptr) {
            sanitizerArr_[i]->Exit();
        }
    }
}

} // namespace Sanitizer
