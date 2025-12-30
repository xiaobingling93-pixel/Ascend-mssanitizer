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


#include <utility>
#include <gtest/gtest.h>

#include "../data_process.h"
#include "../../ccec_defs.h"
#include "plugin/ccec/move_instructions/broadcast_cc.cpp"

using namespace Sanitizer;

constexpr uint64_t MEM_INFO_SIZE = 1024;

template<MemType srcMemType, MemType dstMemType, DataType srcDataType, DataType dstDataType>
BroadcastRecord CreateBroadcastRecord()
{
    BroadcastRecord record{};
    record.dst = RandInt(0, 0x1000000);
    record.src = RandInt(0, 0x1000000);
    record.nBurst = RandInt(0, 0xF);
    record.lenBurst = RandInt(0, 0xFF);
    record.enableRepeat = false;
    record.srcGap = RandInt(0, 0xF);
    record.dstGap = RandInt(0, 0xF);
    record.location.fileNo = RandInt(0, 0x100000);
    record.location.lineNo = RandInt(0, 0x1000);

    record.srcDataType = srcDataType;
    record.dstDataType = dstDataType;
    record.srcMemType = srcMemType;
    record.dstMemType = dstMemType;

    return record;
}

uint64_t ExtractBroadcastRecord(const BroadcastRecord &record)
{
    constexpr uint64_t nBurstMask = 0xFFUL;
    constexpr uint64_t lenBurstMask = 0xFFUL;
    constexpr uint64_t srcGapMask = 0xFFUL;
    constexpr uint64_t dstGapMask = 0xFFUL;
    constexpr uint64_t enableRepeatMask = 0x1;

    constexpr uint64_t nBurstShift = 0;
    constexpr uint64_t lenBurstShift = 8;
    constexpr uint64_t srcGapShift = 16;
    constexpr uint64_t dstGapShift = 24;
    constexpr uint64_t enableRepeatShift = 63;

    return ((record.nBurst & nBurstMask) << nBurstShift) |
        ((record.lenBurst & lenBurstMask) << lenBurstShift) |
        ((record.srcGap & srcGapMask) << srcGapShift) |
        ((record.dstGap & dstGapMask) << dstGapShift) |
        ((record.enableRepeat & enableRepeatMask) << enableRepeatShift);
}

TEST(BroadcastInstructions, broadcast_ub_to_cc_L0C16)
{
    auto record = CreateBroadcastRecord<MemType::UB, MemType::L0C, DataType::DATA_B16, DataType::DATA_B16>();
    std::vector<uint8_t> memInfo = CreateMemInfo();
    uint64_t config = ExtractBroadcastRecord(record);

    // #4
    __sanitizer_report_broadcast_ub_to_cc(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cc__ half *>(record.dst),
        reinterpret_cast<__ubuf__ half *>(record.src), config);

    // #5
    __sanitizer_report_broadcast_ub_to_cc(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cc__ half *>(record.dst),
        reinterpret_cast<__ubuf__ half *>(record.src), record.nBurst, record.lenBurst,
        record.srcGap, record.dstGap);

    __sanitizer_report_broadcast_ub_to_cc(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cc__ half *>(record.dst),
        reinterpret_cast<__ubuf__ half *>(record.src), record.nBurst, record.lenBurst, record.srcGap,
        record.dstGap, record.enableRepeat);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::BROADCAST>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(BroadcastRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::BROADCAST>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(BroadcastRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::BROADCAST>(ptr, record));
}

TEST(BroadcastInstructions, broadcast_ub_to_cc_L0C32_and_conv_is_zero)
{
    auto record = CreateBroadcastRecord<MemType::UB, MemType::L0C, DataType::DATA_B32, DataType::DATA_B32>();
    std::vector<uint8_t> memInfo = CreateMemInfo();
    uint64_t config = ExtractBroadcastRecord(record);

    // #7
    __sanitizer_report_broadcast_ub_to_cc(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cc__ float *>(record.dst),
        reinterpret_cast<__ubuf__ float *>(record.src), config);

    __sanitizer_report_broadcast_ub_to_cc(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cc__ float *>(record.dst),
        reinterpret_cast<__ubuf__ float *>(record.src), record.nBurst, record.lenBurst,
        record.srcGap, record.dstGap);

    __sanitizer_report_broadcast_ub_to_cc(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cc__ float *>(record.dst),
        reinterpret_cast<__ubuf__ float *>(record.src), record.nBurst, record.lenBurst,
        record.srcGap, record.dstGap, record.enableRepeat);

    // #13
    __sanitizer_report_broadcast_ub_to_cc(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cc__ int32_t *>(record.dst),
        reinterpret_cast<__ubuf__ int32_t *>(record.src), config);

    __sanitizer_report_broadcast_ub_to_cc(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cc__ int32_t *>(record.dst),
        reinterpret_cast<__ubuf__ int32_t *>(record.src), record.nBurst, record.lenBurst,
        record.srcGap, record.dstGap);

    __sanitizer_report_broadcast_ub_to_cc(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cc__ int32_t *>(record.dst),
        reinterpret_cast<__ubuf__ int32_t *>(record.src), record.nBurst, record.lenBurst,
        record.srcGap, record.dstGap, record.enableRepeat);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::BROADCAST>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(BroadcastRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::BROADCAST>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(BroadcastRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::BROADCAST>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(BroadcastRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::BROADCAST>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(BroadcastRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::BROADCAST>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(BroadcastRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::BROADCAST>(ptr, record));
}

TEST(BroadcastInstructions, broadcast_ub_to_cc_L0C32_and_conv_not_zero)
{
    auto record = CreateBroadcastRecord<MemType::UB, MemType::L0C, DataType::DATA_B16, DataType::DATA_B32>();
    std::vector<uint8_t> memInfo = CreateMemInfo();
    uint64_t config = ExtractBroadcastRecord(record);

    // #10
    __sanitizer_report_broadcast_ub_to_cc(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cc__ float *>(record.dst),
        reinterpret_cast<__ubuf__ half *>(record.src), config);

    __sanitizer_report_broadcast_ub_to_cc(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cc__ float *>(record.dst),
        reinterpret_cast<__ubuf__ half *>(record.src), record.nBurst, record.lenBurst,
        record.srcGap, record.dstGap);

    __sanitizer_report_broadcast_ub_to_cc(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cc__ float *>(record.dst),
        reinterpret_cast<__ubuf__ half *>(record.src), record.nBurst, record.lenBurst,
        record.srcGap, record.dstGap, record.enableRepeat);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::BROADCAST>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(BroadcastRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::BROADCAST>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(BroadcastRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::BROADCAST>(ptr, record));
}
