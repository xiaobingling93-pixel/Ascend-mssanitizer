
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


#include <gtest/gtest.h>
#include "ccec_defs.h"
#include "ccec/data_process.h"
#include "plugin/ccec/calc_instructions/viou.cpp"

using namespace Sanitizer;
using std::vector;

constexpr uint16_t DstElementNum = 256;

TEST(ViouCalculationInstructions, dump_half_type_record_expect_parse_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    auto record0 = CreateRandomBinaryOpRecord();
    auto record1 = CreateRandomBinaryOpRecord();
    uint64_t config0 = (static_cast<uint64_t>(record0.repeat) << 56);
    uint64_t config1 = (static_cast<uint64_t>(record1.repeat) << 56);
    __sanitizer_report_VIOU_f16(memInfo.data(), record0.location.fileNo, record0.location.lineNo, record0.location.pc,
        reinterpret_cast<half*>(record0.dst), reinterpret_cast<half*>(record0.src0),
        reinterpret_cast<half*>(record0.src1), config0);
    __sanitizer_report_VIOU_f16(memInfo.data(), record1.location.fileNo, record1.location.lineNo, record1.location.pc,
        reinterpret_cast<half*>(record1.dst), reinterpret_cast<half*>(record1.src0),
        reinterpret_cast<half*>(record1.src1), config1);

    auto record = reinterpret_cast<BinaryOpRecord *>(memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead) +
                                                     sizeof(RecordType) * 2
        + sizeof(BinaryOpRecord) * 1);

    ASSERT_EQ(record->repeat, record1.repeat);
    ASSERT_EQ(record->dst, record1.dst);
    ASSERT_EQ(record->src0, record1.src0);
    ASSERT_EQ(record->src1, record1.src1);
    ASSERT_EQ(record->dstBlockStride, 1);
    ASSERT_EQ(record->src0BlockStride, 1);
    ASSERT_EQ(record->src1BlockStride, 0);
    ASSERT_EQ(record->dstBlockSize, DstElementNum * sizeof(half));
}

TEST(ViouCalculationInstructions, dump_float_type_record_expect_parse_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    auto record0 = CreateRandomBinaryOpRecord();
    auto record1 = CreateRandomBinaryOpRecord();
    uint64_t config0 = (static_cast<uint64_t>(record0.repeat) << 56);
    uint64_t config1 = (static_cast<uint64_t>(record1.repeat) << 56);
    __sanitizer_report_VIOU_f32(memInfo.data(), record0.location.fileNo, record0.location.lineNo, record0.location.pc,
        reinterpret_cast<float*>(record0.dst), reinterpret_cast<float*>(record0.src0),
        reinterpret_cast<float*>(record0.src1), config0);
    __sanitizer_report_VIOU_f32(memInfo.data(), record1.location.fileNo, record1.location.lineNo, record1.location.pc,
        reinterpret_cast<float*>(record1.dst), reinterpret_cast<float*>(record1.src0),
        reinterpret_cast<float*>(record1.src1), config1);

    auto record = reinterpret_cast<BinaryOpRecord *>(memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead) +
                                                     sizeof(RecordType) * 2
        + sizeof(BinaryOpRecord) * 1);

    ASSERT_EQ(record->repeat, record1.repeat);
    ASSERT_EQ(record->dst, record1.dst);
    ASSERT_EQ(record->src0, record1.src0);
    ASSERT_EQ(record->src1, record1.src1);
    ASSERT_EQ(record->dstBlockStride, 1);
    ASSERT_EQ(record->src0BlockStride, 1);
    ASSERT_EQ(record->src1BlockStride, 0);
    ASSERT_EQ(record->dstBlockSize, DstElementNum * sizeof(float));
}
