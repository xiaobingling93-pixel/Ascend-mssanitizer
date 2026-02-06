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


#include "sync_sanitizer.h"
#include "sync_info_display.h"
#include "core/framework/utility/log.h"

namespace Sanitizer {

bool SyncSanitizer::SetDeviceInfo(DeviceInfoSummary const &deviceInfo, Config const &config)
{
    checkBlockId_ = config.checkBlockId;
    return false;
}

bool SyncSanitizer::SetKernelInfo(KernelSummary const &kernelInfo)
{
    if (kernelInfo.kernelType == KernelType::AICPU) {
        return false;
    }
    if (kernelInfo.blockDim >= MAX_BLOCKDIM_NUMS) {
        return false;
    }
    return true;
}

bool SyncSanitizer::CheckRecordBeforeProcess(const SanitizerRecord &record)
{
    // 预处理当前仅处理KernelRecord
    if (record.version == RecordVersion::KERNEL_RECORD) {
        return true;
    }
    return false;
}

inline bool SyncSanitizer::IsTargetBlockId(uint32_t blockId)
{
    if (checkBlockId_ == CHECK_ALL_BLOCK || blockId == static_cast<uint32_t>(checkBlockId_)) {
        return true;
    }
    return false;
}

void SyncSanitizer::Do(const SanitizerRecord &record, const std::vector<SanEvent> &events)
{
    (void)record;

    if (events.empty()) {
        return;
    }
    uint64_t selfSyncID {}; // 当前指令
    uint64_t peerSyncID {}; // 与之配对的指令
    for (auto& event : events) {
        if (event.isEndFrame) {
            isFinished_ = true;
            break;
        }
        if (!IsTargetBlockId(event.loc.coreId)) {
            continue;
        }
        if (event.type == EventType::SYNC_EVENT && event.eventInfo.syncInfo.opType == SyncType::SET_FLAG) {
            selfSyncID = CalcSetFlagSyncID(event);
            peerSyncID = CalcWaitFlagSyncID(event);
        } else if (event.type == EventType::SYNC_EVENT && event.eventInfo.syncInfo.opType == SyncType::WAIT_FLAG) {
            selfSyncID = CalcWaitFlagSyncID(event);
            peerSyncID = CalcSetFlagSyncID(event);
        } else {
            continue;
        }

        if (syncEvents_.find(peerSyncID) != syncEvents_.end()) {
            // 如果找到配对指令，就删除配对指令
            if (syncEvents_[peerSyncID].size() > 1) {
                syncEvents_[peerSyncID].pop_back();
            } else {
                syncEvents_.erase(peerSyncID);
            }
        } else {
            // 如果找不到，就插入当前指令
            auto selfSyncInfo = SyncDispInfo {};
            selfSyncInfo.baseEvent.Init(MemEvent(event));
            selfSyncInfo.srcPipe = event.eventInfo.syncInfo.srcPipe;
            selfSyncInfo.dstPipe = event.eventInfo.syncInfo.dstPipe;
            syncEvents_[selfSyncID].push_back(selfSyncInfo);
        }
    }
    if (isFinished_ && !syncEvents_.empty()) {
        ReportUnpairedInfo();
    }
}

void SyncSanitizer::ParseOnlineError(const KernelErrorRecord &record, BlockType blockType, uint64_t serialNo)
{
    (void)record;
    (void)blockType;
    (void)serialNo;
}

void SyncSanitizer::ReportUnpairedInfo()
{
    // build pc stack map cache
    std::set<uint64_t> pcOffsets;
    std::vector<SyncDispInfo> dispEvents;
    for (auto &each : syncEvents_) {
        if (!(each.first & 0xF)) { // 为空说明是set_flag，有值则为wait_flag
            auto &syncEvents = each.second;
            for (auto &syncEvent : syncEvents) {
                pcOffsets.insert(syncEvent.baseEvent.pc);
                dispEvents.push_back(syncEvent);
            }
        }
    }

    if (dispEvents.empty()) {
        return;
    }
    CallStack::Instance().CachePcOffsets(pcOffsets);
    for (SyncDispInfo const &it : dispEvents) {
        msgFunc_(LogLv::WARN, [&it](void) {
            std::stringstream ss;
            ss << it << std::endl;
            return DetectionInfo{ToolType::SYNCCHECK, ss.str()};
        });
    }
}

uint64_t SyncSanitizer::CalcSetFlagSyncID(SanEvent const &event)
{
    constexpr uint8_t coreIDShift = 20;
    constexpr uint8_t blockTypeShift = 16;
    constexpr uint8_t srcPipeShift = 12;
    constexpr uint8_t dstPipeShift = 8;
    constexpr uint8_t eventIDShift = 4;

    return ((static_cast<uint64_t>(event.loc.coreId) & 0xFFFF) << coreIDShift) |
           ((static_cast<uint64_t>(event.loc.blockType) & 0xF) << blockTypeShift) |
           ((static_cast<uint64_t>(event.eventInfo.syncInfo.srcPipe) & 0xF) << srcPipeShift) |
           ((static_cast<uint64_t>(event.eventInfo.syncInfo.dstPipe) & 0xF) << dstPipeShift) |
           ((static_cast<uint64_t>(event.eventInfo.syncInfo.eventId) & 0xF) << eventIDShift);
}

uint64_t SyncSanitizer::CalcWaitFlagSyncID(SanEvent const &event)
{
    return (CalcSetFlagSyncID(event) + 1UL);
}

void SyncSanitizer::RegisterNotifyFunc(const MSG_FUNC &func)
{
    msgFunc_ = func;
}


void SyncSanitizer::Exit()
{
}

static std::shared_ptr<SanitizerBase> CreateSyncSanitizer()
{
    return std::make_shared<SyncSanitizer>();
}

static RegisteSanitizer g_regSyncSanitizer(ToolType::SYNCCHECK, CreateSyncSanitizer);
}
