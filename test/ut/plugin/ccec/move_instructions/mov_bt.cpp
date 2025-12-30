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
#include "plugin/ccec/move_instructions/mov_bt.cpp"

using namespace Sanitizer;

template<MemType srcMemType, MemType dstMemType>
MovBtRecord CreateRandomMovBtRecord()
{
    MovBtRecord record{};
    record.dst = RandInt(0, 0x1000000);
    record.src = RandInt(0, 0x1000000);
    record.nBurst = RandInt(0, 0xFFF);
    record.lenBurst = RandInt(0, 0xFFFF);
    record.srcGap = RandInt(0, 0xFFFF);
    record.dstGap = RandInt(0, 0xFFFF);
    record.location.fileNo = RandInt(0, 0x100000);
    record.location.lineNo = RandInt(0, 0x1000);
    record.convControl = 0;

    record.srcMemType = srcMemType;
    record.dstMemType = dstMemType;
    return record;
}

uint64_t ExtractConfigFromMovBtRecord(const MovBtRecord &record)
{
    constexpr uint64_t lenBurstMask = 0xFFFFUL;
    constexpr uint64_t nBurstMask = 0xFFFUL;
    constexpr uint64_t gapMask = 0xFFFFUL;

    constexpr uint64_t lenBurstShift = 16;
    constexpr uint64_t nBurstShift = 4;
    constexpr uint64_t dstGapShift = 48;
    constexpr uint64_t srcGapShift = 32;
    constexpr uint64_t convControlShift = 3;

    return ((record.convControl & 0x1) << convControlShift) | ((record.lenBurst & lenBurstMask) << lenBurstShift) |
           ((record.nBurst & nBurstMask) << nBurstShift) | ((record.srcGap & gapMask) << srcGapShift) |
           ((record.dstGap & gapMask) << dstGapShift);
}

TEST(MovBtInstructions, mov_bt)
{
    auto record = CreateRandomMovBtRecord<MemType::L1, MemType::BT>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractConfigFromMovBtRecord(record);

    // #47
    __sanitizer_report_copy_cbuf_to_bt(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       record.dst, reinterpret_cast<void*>(record.src), config);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::MOV_BT>(ptr, record));

    // #48
    __sanitizer_report_copy_cbuf_to_bt(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       record.dst, reinterpret_cast<void*>(record.src), record.convControl,
                                       record.nBurst, record.lenBurst, record.srcGap, record.dstGap);
    ptr += sizeof(RecordType) + sizeof(MovBtRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::MOV_BT>(ptr, record));
}
