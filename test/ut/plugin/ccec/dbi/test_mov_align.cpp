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
#include <iostream>
#include <gtest/gtest.h>
#include <cstdint>

#include "../data_process.h"
#define BUILD_DYNAMIC_PROBE
#include "plugin/ccec/dbi/probes/mov_align.cpp"
#include "plugin/ccec/dbi/probes/mov_align_registers.cpp"

template<MemType srcMemType, MemType dstMemType>
MovAlignRecordV2 CreateRandomMovAlignV2Record()
{
    MovAlignRecordV2 record{};
    record.dst = RandInt(0, 0x1000000);
    record.src = RandInt(0, 0x1000000);
    record.nBurst = RandInt(0, 0xFFF);
    record.lenBurst = RandInt(0, 0xFFFF);
    record.srcStride = RandInt(0, 0xFFF);
    record.dstStride = RandInt(0, 0xFFF);
    record.location.fileNo = RandInt(0, 0x100000);
    record.location.lineNo = RandInt(0, 0x1000);
    record.srcMemType = srcMemType;
    record.dstMemType = dstMemType;
    record.loop1DstStride = RandInt(0, 0xFFFFF);
    record.loop1SrcStride = RandInt(0, 0xFFFFF);
    record.loop2DstStride = RandInt(0, 0xFFFFF);
    record.loop2SrcStride = RandInt(0, 0xFFFFF);
    record.loop1Size = RandInt(0, 0xFFFFF);
    record.loop2Size = RandInt(0, 0xFFFFF);
    record.dataType = Sanitizer::DataType::DATA_B32;
    // not gen left/right padding
    return record;
}

void ExtractConfigFromMovAlignV2Record(const Sanitizer::MovAlignRecordV2 *record, uint64_t &config0, uint64_t &config1)
{
    SetConfByUint<24, 4>(config0, record->nBurst);
    SetConfByUint<45, 25>(config0, record->lenBurst);
    if (record->srcMemType == MemType::UB && record->dstMemType == MemType::GM) {
        SetConfByUint<39, 0>(config1, record->dstStride);
        SetConfByUint<60, 40>(config1, record->srcStride);
    } else {
        SetConfByUint<39, 0>(config1, record->srcStride);
        SetConfByUint<60, 40>(config1, record->dstStride);
    }
}

TEST(DbiMovAlignInstructions, dump_mov_align_v2_ub2gm_expect_get_correct_records)
{
    auto record = CreateRandomMovAlignV2Record<MemType::UB, MemType::GM>();
    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config0{};
    uint64_t config1{};
    ExtractConfigFromMovAlignV2Record(&record, config0, config1);

    uint8_t *ptr = memInfo.data();

    uint64_t loopSizeUbToOut = 0;
    SetConfByUint<20, 0>(loopSizeUbToOut, record.loop1Size);
    SetConfByUint<42, 21>(loopSizeUbToOut, record.loop2Size);
    uint64_t loop1StrideUbToOut = 0;
    SetConfByUint<39, 0>(loop1StrideUbToOut, record.loop1DstStride);
    SetConfByUint<60, 40>(loop1StrideUbToOut, record.loop1SrcStride);
    uint64_t loop2StrideUbToOut = 0;
    SetConfByUint<39, 0>(loop2StrideUbToOut, record.loop2DstStride);
    SetConfByUint<60, 40>(loop2StrideUbToOut, record.loop2SrcStride);

    __sanitizer_report_set_loop_size_ubtoout(memInfo.data(),
        record.location.pc, 0, loopSizeUbToOut);
    ptr += sizeof(RecordGlobalHead);
    Sanitizer::RecordBlockHead &recordBlockHead = *reinterpret_cast<Sanitizer::RecordBlockHead*>(ptr);
    uint64_t regVal = GetUintFromConf<20, 0>(recordBlockHead.registers.sprLoopSizeUb2Out);
    ASSERT_TRUE(regVal == record.loop1Size);
    regVal = GetUintFromConf<42, 21>(recordBlockHead.registers.sprLoopSizeUb2Out);
    ASSERT_TRUE(regVal == record.loop2Size);

    __sanitizer_report_set_loop1_stride_ubtoout(memInfo.data(),
        record.location.pc, 0, loop1StrideUbToOut);
    regVal = GetUintFromConf<39, 0>(recordBlockHead.registers.sprLoop1StrideUb2Out);
    ASSERT_TRUE(regVal == record.loop1DstStride);
    regVal = GetUintFromConf<60, 40>(recordBlockHead.registers.sprLoop1StrideUb2Out);
    ASSERT_TRUE(regVal == record.loop1SrcStride);

    __sanitizer_report_set_loop2_stride_ubtoout(memInfo.data(),
        record.location.pc, 0, loop2StrideUbToOut);
    regVal = GetUintFromConf<39, 0>(recordBlockHead.registers.sprLoop2StrideUb2Out);
    ASSERT_TRUE(regVal == record.loop2DstStride);
    regVal = GetUintFromConf<60, 40>(recordBlockHead.registers.sprLoop2StrideUb2Out);
    ASSERT_TRUE(regVal == record.loop2SrcStride);

    __sanitizer_report_copy_ubuf_to_gm_align_v2(memInfo.data(),
        record.location.pc,
        0,
        reinterpret_cast<void *>(record.dst),
        reinterpret_cast<void *>(record.src),
        config0,
        config1);

    ptr += sizeof(RecordBlockHead);
    auto x = reinterpret_cast<Sanitizer::MovAlignRecordV2 *>(ptr + sizeof(RecordType));
    record.location = x->location;
    ASSERT_TRUE(CheckRecordEqual<RecordType::MOV_ALIGN_V2>(ptr, record));
}

TEST(DbiMovAlignInstructions, dump_mov_align_v2_gm2ub__expect_get_correct_records)
{
    auto record = CreateRandomMovAlignV2Record<MemType::GM, MemType::UB>();
    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config0{};
    uint64_t config1{};
    ExtractConfigFromMovAlignV2Record(&record, config0, config1);

    uint8_t *ptr = memInfo.data();

    uint64_t loopSizeOut2Ub = 0;
    SetConfByUint<20, 0>(loopSizeOut2Ub, record.loop1Size);
    SetConfByUint<42, 21>(loopSizeOut2Ub, record.loop2Size);
    uint64_t loop1StrideOut2Ub = 0;
    SetConfByUint<39, 0>(loop1StrideOut2Ub, record.loop1SrcStride);
    SetConfByUint<60, 40>(loop1StrideOut2Ub, record.loop1DstStride);
    uint64_t loop2StrideOut2Ub = 0;
    SetConfByUint<39, 0>(loop2StrideOut2Ub, record.loop2SrcStride);
    SetConfByUint<60, 40>(loop2StrideOut2Ub, record.loop2DstStride);

    __sanitizer_report_set_loop_size_outtoub(memInfo.data(),
        record.location.pc, 0, loopSizeOut2Ub);
    ptr += sizeof(RecordGlobalHead);
    Sanitizer::RecordBlockHead &recordBlockHead = *reinterpret_cast<Sanitizer::RecordBlockHead*>(ptr);
    uint64_t regVal = GetUintFromConf<20, 0>(recordBlockHead.registers.sprLoopSizeOut2Ub);
    ASSERT_TRUE(regVal == record.loop1Size);
    regVal = GetUintFromConf<42, 21>(recordBlockHead.registers.sprLoopSizeOut2Ub);
    ASSERT_TRUE(regVal == record.loop2Size);

    __sanitizer_report_set_loop1_stride_outtoub(memInfo.data(),
        record.location.pc, 0, loop1StrideOut2Ub);
    regVal = GetUintFromConf<39, 0>(recordBlockHead.registers.sprLoop1StrideOut2Ub);
    ASSERT_TRUE(regVal == record.loop1SrcStride);
    regVal = GetUintFromConf<60, 40>(recordBlockHead.registers.sprLoop1StrideOut2Ub);
    ASSERT_TRUE(regVal == record.loop1DstStride);

    __sanitizer_report_set_loop2_stride_outtoub(memInfo.data(),
        record.location.pc, 0, loop2StrideOut2Ub);
    regVal = GetUintFromConf<39, 0>(recordBlockHead.registers.sprLoop2StrideOut2Ub);
    ASSERT_TRUE(regVal == record.loop2SrcStride);
    regVal = GetUintFromConf<60, 40>(recordBlockHead.registers.sprLoop2StrideOut2Ub);
    ASSERT_TRUE(regVal == record.loop2DstStride);

    __sanitizer_report_copy_gm_to_ubuf_align_v2_b32(memInfo.data(),
        record.location.pc,
        0,
        reinterpret_cast<void *>(record.dst),
        reinterpret_cast<void *>(record.src),
        config0,
        config1);

    ptr += sizeof(RecordBlockHead);
    auto x = reinterpret_cast<Sanitizer::MovAlignRecordV2 *>(ptr + sizeof(RecordType));
    record.location = x->location;
    ASSERT_TRUE(CheckRecordEqual<RecordType::MOV_ALIGN_V2>(ptr, record));
}

