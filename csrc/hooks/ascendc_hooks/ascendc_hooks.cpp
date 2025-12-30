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


#include <algorithm>
#include <cstdint>
#include <linux/limits.h>

#include "constant.h"
#include "record_defs.h"
#include "arch_def.h"
#include "record_format.h"
#include "hook_report.h"
#include "securec.h"
#include "runtime.h"
#include "dev_mem_manager.h"
#include "platform_config.h"

#if defined(BUILD_TRACEKIT)
#define KERNEL_LAUNCH_INIT __mstrace_init
#define KERNEL_LAUNCH_FINALIZE __mstrace_finalize
#else
#define KERNEL_LAUNCH_INIT __sanitizer_init
#define KERNEL_LAUNCH_FINALIZE __sanitizer_finalize
#endif

extern "C" {
uint8_t* KERNEL_LAUNCH_INIT(uint64_t blockDim);
void KERNEL_LAUNCH_FINALIZE(uint8_t *memInfo, uint64_t blockDim);
}

namespace Sanitizer {
static uint64_t g_totalBlockDim;

template <typename Record>
bool ParseRecordByType(uint8_t const *ptr, Record &record, uint64_t &offset)
{
    record = *reinterpret_cast<Record const *>(ptr);
    offset += sizeof(Record);

    uint64_t pcStartAddr = HookReport::Instance().pcStartAddr;
    if (pcStartAddr != 0x00 && record.location.pc >= pcStartAddr) {
        // update pc offset with pc start addr
        record.location.pc -= pcStartAddr;
    } else {
        // -1 stands for invalid pc offset
        record.location.pc = INVALID_PC_OFFSET;
    }
    return true;
}

const std::map<RecordType, std::function<bool(uint8_t const *, KernelRecord &, uint64_t &)>> RECORD_TYPE_MAP = {
    {RecordType::LOAD, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.loadStoreRecord, offset);
    }},
    {RecordType::STORE, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.loadStoreRecord, offset);
    }},
    {RecordType::DMA_MOV, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.dmaMovRecord, offset);
    }},
    {RecordType::DMA_MOV_CONV_RELU, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.dmaMovConvReluRecord, offset);
    }},
    {RecordType::DMA_MOV_ND2NZ, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.dmaMovNd2nzRecord, offset);
    }},
    {RecordType::DMA_MOV_ND2NZ_D, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.dmaMovNd2nzDavRecord, offset);
    }},
    {RecordType::DMA_MOV_DN2NZ_D, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.dmaMovNd2nzDavRecord, offset);
    }},
    {RecordType::MOV_ALIGN, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.movAlignRecord, offset);
    }},
    {RecordType::MOV_ALIGN_V2, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.movAlignRecordV2, offset);
    }},
    {RecordType::ND_DMA_OUT_TO_UB, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.ndDMAOut2UbRecord, offset);
    }},
    {RecordType::MOV_BT, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.movBtRecord, offset);
    }},
    {RecordType::MOV_FP, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.movFpRecord, offset);
    }},
    {RecordType::VEC_DUP, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.vecDupRecord, offset);
    }},
    {RecordType::LOAD_2D, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.load2DRecord, offset);
    }},
    {RecordType::LOAD_2D_SPARSE, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.load2DSparseRecord, offset);
    }},
    {RecordType::LOAD_2D_TRANSPOSE, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.load2DTransposeRecord, offset);
    }},
    {RecordType::DECOMPRESS_HEADER, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.decompressHeaderRecord, offset);
    }},
    {RecordType::BROADCAST, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.broadcastRecord, offset);
    }},
    {RecordType::SCATTERVNCHWCONV, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.scatterVnchwconvRecord, offset);
    }},
    {RecordType::LOAD_3D, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.load3DRecord, offset);
    }},
    {RecordType::LOAD_IMAGE, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.loadImageRecord, offset);
    }},
    {RecordType::LOAD_SMASK, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.loadSmaskRecord, offset);
    }},
    {RecordType::SET_2D, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.set2DRecord, offset);
    }},
    {RecordType::LOAD_B2, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.loadB2Record, offset);
    }},
    {RecordType::LOAD_A_WINOGRAD, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.loadAWinogradRecord, offset);
    }},
    {RecordType::LOAD_B_WINOGRAD, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.loadBWinogradRecord, offset);
    }},
    {RecordType::UNARY_OP, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.unaryOpRecord, offset);
    }},
    {RecordType::VGATHER, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.vgatherRecord, offset);
    }},
    {RecordType::ELEMENT, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.elementRecord, offset);
    }},
    {RecordType::VCOPY_OP, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.unaryOpRecord, offset);
    }},
    {RecordType::VREDUCEV2_UNARY, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.unaryOpRecord, offset);
    }},
    {RecordType::VREDUCEV2_BINARY, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.binaryOpRecord, offset);
    }},
    {RecordType::VMRGSORT4_OP_C220, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.unaryOpRecord, offset);
    }},
    {RecordType::VMRGSORT4_OP_M200, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.unaryOpRecord, offset);
    }},
    {RecordType::BINARY_OP, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.binaryOpRecord, offset);
    }},
    {RecordType::TERNARY_OP, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.binaryOpRecord, offset);
    }},
    {RecordType::MATRIX_MUL_OP, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.matrixMulOpRecord, offset);
    }},
    {RecordType::VEC_REGPROPCOOR_OP, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.vecRegPropCoordOpRecord, offset);
    }},
    {RecordType::MSTX_STUB, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.mstxRecord, offset);
    }},
    {RecordType::REDUCE_OP, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.reduceOpRecord, offset);
    }},
    {RecordType::CMPMASK_OP, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.unaryOpRecord, offset);
    }},
    {RecordType::SET_FLAG, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.syncRecord, offset);
    }},
    {RecordType::WAIT_FLAG, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.syncRecord, offset);
    }},
    {RecordType::FFTS_SYNC, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.fftsSyncRecord, offset);
    }},
    {RecordType::WAIT_FLAG_DEV, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.waitFlagDevRecord, offset);
    }},
    {RecordType::HSET_FLAG, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.hardSyncRecord, offset);
    }},
    {RecordType::HWAIT_FLAG, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.hardSyncRecord, offset);
    }},
    {RecordType::PIPE_BARRIER, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.pipeBarrierRecord, offset);
    }},
    {RecordType::SET_ATOMIC, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.atomicModeRecord, offset);
    }},
    {RecordType::IB_SET_STUB, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.softSyncRecord, offset);
    }},
    {RecordType::IB_WAIT_STUB, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.softSyncRecord, offset);
    }},
    {RecordType::SYNC_ALL_STUB, [](uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset) {
        return ParseRecordByType(record, kernelRecord.payload.softSyncRecord, offset);
    }},
};

bool ParseRecord(RecordType const &recordType, uint8_t const *record, KernelRecord &kernelRecord, uint64_t &offset)
{
    auto it = RECORD_TYPE_MAP.find(kernelRecord.recordType);
    if (it != RECORD_TYPE_MAP.end()) {
        return it->second(record, kernelRecord, offset);
    } else {
        std::cout << "[mssanitizer] parse record failed, " << kernelRecord.recordType <<
                     " record type does not exist in recordTypeMap" << std::endl;
        return false;
    }
}

void ReportSignalRecord(RecordType recordType)
{
    auto signalRecord = Sanitizer::SanitizerRecord {};
    signalRecord.version = Sanitizer::RecordVersion::KERNEL_RECORD;
    signalRecord.payload.kernelRecord.recordType = recordType;
    Sanitizer::HookReport::Instance().Report(signalRecord);
}

void ReportSanitizerRecords(uint8_t *memInfoHost, uint64_t size, size_t blockIdx, uint8_t vecSubBlockDim)
{
    Sanitizer::HookReport &report = Sanitizer::HookReport::Instance();
    RecordGlobalHead *recordGlobalHead = reinterpret_cast<RecordGlobalHead *>(memInfoHost);
    RecordBlockHead* recordBlockHead = reinterpret_cast<RecordBlockHead *>(memInfoHost + sizeof(RecordGlobalHead));
    if (!blockIdx && recordGlobalHead->securityVal != RECORD_HEAD_SECURITY_VALUE) {
        std::cout << "[mssanitizer] [error]"
            "The record address has been changed by the user program, and the detection result is unreliable." << std::endl;
    }
    uint8_t *memInfoBlockRecord = memInfoHost + sizeof(RecordBlockHead);
    const uint64_t recordWriteCount = recordBlockHead->recordWriteCount;
    const uint64_t recordCount = recordBlockHead->recordCount;
    uint64_t recordOffset = 0;
    auto kernelRecord = KernelRecord {};

    kernelRecord.blockType = recordBlockHead->blockInfo.blockType;
    for (uint64_t recordIdx = 0; recordIdx < recordWriteCount; ++recordIdx) {
        RecordType *recordTypePtr = reinterpret_cast<RecordType *>(memInfoBlockRecord + recordOffset);
        recordOffset += sizeof(RecordType);
        kernelRecord.recordType = *recordTypePtr;

        auto *record = static_cast<uint8_t *>(static_cast<void *>(recordTypePtr + 1));
        if (!ParseRecord(*recordTypePtr, record, kernelRecord, recordOffset)) {
            break;
        }
        /// 后续字段赋值，需要在ParseRecord之后赋值，ParseRecord会重新解析record，在此之前赋值会导致信息丢失；
        if (*recordTypePtr == RecordType::FFTS_SYNC) {
            kernelRecord.payload.fftsSyncRecord.vecSubBlockDim = vecSubBlockDim;
        }

        uint64_t curSize = sizeof(RecordBlockHead) + recordOffset;
        if (curSize > size) {
            std::cout << "[mssanitizer] " << " the size of memInfo is " << size
                << ", but the current size is " <<  curSize << " which beyond the maximum size" << std::endl;
            break;
        }

        auto sanitizerRecord = Sanitizer::SanitizerRecord {};
        sanitizerRecord.version = Sanitizer::RecordVersion::KERNEL_RECORD;
        sanitizerRecord.payload.kernelRecord = kernelRecord;
        report.Report(sanitizerRecord);
    }
    ReportSignalRecord(RecordType::BLOCK_FINISH);
    if (recordWriteCount < recordCount) {
        uint64_t needCacheSize = recordGlobalHead->checkParms.cacheSize +
            (recordBlockHead->offset - recordBlockHead->writeOffset) / MB_TO_BYTES + 1; // +1的目的是向上取整
        if (needCacheSize > MAX_RECORD_BUF_SIZE_EACH_BLOCK) {
            std::cout << "[mssanitizer] [error] " << (recordCount - recordWriteCount)
                  << " records undetected, exceed the max record buffer size for each block: " << MAX_RECORD_BUF_SIZE_EACH_BLOCK
                  << ". Some records were discarded, the detection result maybe unreliable!" << std::endl;
        } else {
            std::cout << "[mssanitizer] [warning] " << (recordCount - recordWriteCount)
                  << " records undetected, please use --cache-size=" << needCacheSize << " to run the operator again." << std::endl;
        }
    }
}

bool CheckBlockDimValid(uint64_t blockDim)
{
    if (blockDim == 0) {
        std::cout << "[mssanitizer] " \
        << "blockdim must be greater than 0, and then the true value is 0" \
        << std::endl;
        return false;
    } else if (blockDim > MAX_BLOCKDIM_NUMS) {
        std::cout << "[mssanitizer] " \
        << "Blockdim cannot exceed the maximum value, the maximum value is " \
        << MAX_BLOCKDIM_NUMS \
        << std::endl;
        return false;
    }
    return true;
}

bool AssignTotalBlockDim(RecordGlobalHead &head, uint64_t blockDim)
{
    head.checkParms = HookReport::Instance().GetCheckParms();
    head.kernelInfo = HookReport::Instance().GetKernelInfo();
    DeviceType deviceType = HookReport::Instance().GetDeviceType();
    if (HasSubBlocks(deviceType)) {
        g_totalBlockDim = head.checkParms.checkBlockId == CHECK_ALL_BLOCK ?
            (C220_VEC_SUB_BLOCKDIM + C220_CUBE_SUB_BLOCKDIM) * blockDim :
            (C220_VEC_SUB_BLOCKDIM + C220_CUBE_SUB_BLOCKDIM);
    } else {
        g_totalBlockDim = head.checkParms.checkBlockId == CHECK_ALL_BLOCK ? blockDim : 1;
    }

    uint64_t singleBlockSize = head.checkParms.cacheSize;
    uint64_t totalSize = singleBlockSize * g_totalBlockDim;
    if (singleBlockSize == 0 || singleBlockSize > MAX_RECORD_BUF_SIZE_EACH_BLOCK ||
        totalSize > MAX_RECORD_BUF_SIZE_ALL_BLOCK) {
            std::cout << "[mssanitizer] ERROR:" <<
            " cache size must be [1, " << MAX_RECORD_BUF_SIZE_EACH_BLOCK <<
            "](MB), total cache size must less than: " <<
            MAX_RECORD_BUF_SIZE_ALL_BLOCK << "MB, but current cache size is: " <<
            head.checkParms.cacheSize <<
            "MB, total cache size is " << totalSize << "MB" << std::endl;
            return false;
    }
    return true;
}

}  // namespace Sanitizer

using namespace Sanitizer;

void KERNEL_LAUNCH_FINALIZE(uint8_t *memInfo, uint64_t blockDim)
{
    auto &devMemManager = DevMemManager::GetInstance();
    if (!devMemManager.IsMemoryInit()) {
        return;
    }
    bool isValid = CheckBlockDimValid(blockDim);
    if (!isValid) {
        return;
    }
    if (memInfo == nullptr) {
        return;
    }
    RecordGlobalHead recordGlobalHead{};
    recordGlobalHead.checkParms = HookReport::Instance().GetCheckParms();
    uint64_t singleBlockByteSize = recordGlobalHead.checkParms.cacheSize * MB_TO_BYTES + sizeof(RecordBlockHead);
    uint8_t *memInfoHost = new uint8_t[singleBlockByteSize];
    rtError_t error;
    Sanitizer::HookReport::Instance().DetermineMemInfoSrc();
    uint8_t vecSubBlockDim = 0;
    
    // 此处是device -> host的数据搬运，根据实际协议头的writeOffset值进行分批搬运，避免搬运过多数据导致性能变慢
    for (size_t i = 0; i < g_totalBlockDim; i++) {
        error = rtMemcpy(memInfoHost, singleBlockByteSize, memInfo + sizeof(RecordGlobalHead) + i * singleBlockByteSize,
            sizeof(RecordBlockHead), RT_MEMCPY_DEVICE_TO_HOST);
        if (error) {
            std::cout << "[mssanitizer] finalize rtMemcpy header error:" << error << std::endl;
            break;
        }

        RecordBlockHead *pHead = reinterpret_cast<RecordBlockHead*>(memInfoHost);
        if (i == 0) {
            vecSubBlockDim = pHead->blockInfo.vecSubBlockDim;
        }

        if (pHead->writeOffset == 0) {
            ReportSanitizerRecords(memInfoHost, singleBlockByteSize, i, vecSubBlockDim);
            continue;
        }

        error = rtMemcpy(memInfoHost + sizeof(RecordBlockHead), singleBlockByteSize - sizeof(RecordBlockHead),
            memInfo + i * singleBlockByteSize + sizeof(RecordBlockHead), pHead->writeOffset,
            RT_MEMCPY_DEVICE_TO_HOST);
        if (error) {
            std::cout << "[mssanitizer] finalize rtMemcpy offset error:" << error << std::endl;
            break;
        }
        ReportSanitizerRecords(memInfoHost, singleBlockByteSize, i, vecSubBlockDim);
    }
    ReportSignalRecord(RecordType::FINISH);
    delete[] memInfoHost;
    memInfoHost = nullptr;
    devMemManager.SetMemoryInitFlag(false);
}

/// register、kernelLaunch和init之间的关系如下：
/// register-kernel -> init1-meminfo1 -> kernelLaunch -> get-kernelType -> init2-meminfo2
/// 内核调用符场景，sanitizer_init会被调用两次，一次为编译器调用init1，一次为工具调用init2；
/// 真正执行信息记录对应的地址为编译器调用返回的地址meminfo1；
/// __sanitizer_init内部做了内存复用的逻辑，两次执行时内存大小如果相同，
/// 则第二次会复用第一次的内存（sanitizer_init接口内部做了head处的deviceType、kernelType的拷贝）；
/// 基于上述逻辑，__sanitizer_init申请需要按照mix算子统一申请最大内存；这样init2返回的meminfo2
/// 为第一次init1返回的meminfo1，此时init2会将正确的kernelType拷贝到meminfo1中；
uint8_t* KERNEL_LAUNCH_INIT(uint64_t blockDim)
{
    bool isValid = CheckBlockDimValid(blockDim);
    if (!isValid) {
        std::cout << "[mssanitizer] " << "blockdim is invalid" << std::endl;
        return nullptr;
    }
    
    RecordGlobalHead recordGlobalHead{};
    if (!AssignTotalBlockDim(recordGlobalHead, blockDim)) {
        return nullptr;
    }

    uint64_t singleBlockByteSize = recordGlobalHead.checkParms.cacheSize * MB_TO_BYTES + sizeof(RecordBlockHead);
    uint64_t totalByteSize = sizeof(RecordGlobalHead) + singleBlockByteSize * g_totalBlockDim;
    auto &devMemManager = DevMemManager::GetInstance();
    void *memPtr = nullptr;
    rtError_t error = devMemManager.MallocMemory(&memPtr, totalByteSize);
    if (error != RT_ERROR_NONE) {
        std::cout << "[mssanitizer] " << "rtMalloc error:" << error << std::endl;
        return nullptr;
    }
    uint8_t *memInfo = static_cast<uint8_t*>(memPtr);
    error = rtMemcpy(memInfo, sizeof(RecordGlobalHead), reinterpret_cast<uint8_t*>(&recordGlobalHead),
        sizeof(RecordGlobalHead), RT_MEMCPY_HOST_TO_DEVICE);
    if (error) {
        std::cout << "[mssanitizer] " << "rtMemcpy error" << error << std::endl;
        devMemManager.Free();
        return nullptr;
    }
    if (devMemManager.IsMemoryInit()) {
        return memInfo;
    }
    // 每个核的实际处理数据长度是 协议头的长度 + 头内的offset大小，故数据重置仅重置协议头
    // 上面的rtMemcpy已经拷贝了核0的头数据，故此处以核1起始进行数据重置
    for (size_t i = 1; i < g_totalBlockDim; i++) {
        error = rtMemset(memInfo + sizeof(RecordGlobalHead) + i * singleBlockByteSize,
                         totalByteSize - i * singleBlockByteSize,
                         0x00, sizeof(RecordBlockHead));
        if (error) {
            std::cout << "[mssanitizer] " << "rtMemset error:" << error << std::endl;
            devMemManager.Free();
            return nullptr;
        }
    }
    devMemManager.SetMemoryInitFlag(true);
    return memInfo;
}
