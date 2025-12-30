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
#include "plugin/ccec/calc_instructions/vbitsort.cpp"
 
using namespace Sanitizer;

TEST(VbitsortCalcInstructions, VMS_16_dump_half_type_records_and_parse_last_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    auto record0 = CreateRandomUnaryOpRecord();
    uint64_t config = ExtractConfigFromUnaryOpRecord(record0);
    __sanitizer_report_VBS16_f16(memInfo.data(), record0.location.fileNo, record0.location.lineNo, record0.location.pc,
        reinterpret_cast<__ubuf__ half *>(record0.dst), reinterpret_cast<__ubuf__ half *>(record0.src), config);
    auto unaryOp = reinterpret_cast<UnaryOpRecord *>(memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead) +
                                                     sizeof(RecordType));
    ASSERT_EQ(unaryOp->dst, record0.dst);
    ASSERT_EQ(unaryOp->src, record0.src);
    ASSERT_EQ(unaryOp->location.fileNo, record0.location.fileNo);
    ASSERT_EQ(unaryOp->location.lineNo, record0.location.lineNo);
    ASSERT_EQ(unaryOp->repeat, record0.repeat);
    ASSERT_EQ(unaryOp->dstBlockStride, record0.dstBlockStride);
    ASSERT_EQ(unaryOp->srcBlockStride, record0.srcBlockStride);
    ASSERT_EQ(unaryOp->dstRepeatStride, record0.dstRepeatStride);
    ASSERT_EQ(unaryOp->srcRepeatStride, record0.srcRepeatStride);
    ASSERT_EQ(unaryOp->dstBlockSize, 256);
    ASSERT_EQ(unaryOp->srcBlockSize, 256);
    ASSERT_EQ(unaryOp->dstBlockNum, 1);
    ASSERT_EQ(unaryOp->srcBlockNum, 1);
}

TEST(VbitsortCalcInstructions, VMS_16_dump_float_type_records_and_parse_last_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    auto record0 = CreateRandomUnaryOpRecord();
    uint64_t config = ExtractConfigFromUnaryOpRecord(record0);
    __sanitizer_report_VBS16_f32(memInfo.data(), record0.location.fileNo, record0.location.lineNo, record0.location.pc,
        reinterpret_cast<__ubuf__ float *>(record0.dst), reinterpret_cast<__ubuf__ float *>(record0.src), config);
    auto unaryOp = reinterpret_cast<UnaryOpRecord *>(memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead) +
                                                     sizeof(RecordType));
    ASSERT_EQ(unaryOp->dst, record0.dst);
    ASSERT_EQ(unaryOp->src, record0.src);
    ASSERT_EQ(unaryOp->location.fileNo, record0.location.fileNo);
    ASSERT_EQ(unaryOp->location.lineNo, record0.location.lineNo);
    ASSERT_EQ(unaryOp->repeat, record0.repeat);
    ASSERT_EQ(unaryOp->dstBlockStride, record0.dstBlockStride);
    ASSERT_EQ(unaryOp->srcBlockStride, record0.srcBlockStride);
    ASSERT_EQ(unaryOp->dstRepeatStride, record0.dstRepeatStride);
    ASSERT_EQ(unaryOp->srcRepeatStride, record0.srcRepeatStride);
    ASSERT_EQ(unaryOp->dstBlockSize, 256);
    ASSERT_EQ(unaryOp->srcBlockSize, 256);
    ASSERT_EQ(unaryOp->dstBlockNum, 1);
    ASSERT_EQ(unaryOp->srcBlockNum, 1);
}
 
TEST(VbitsortCalcInstructions, dump_half_type_records_and_parse_last_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    auto record0 = CreateRandomBinaryOpRecord();
    auto record1 = CreateRandomBinaryOpRecord();
 
    __sanitizer_report_VBS32_f16(memInfo.data(), record0.location.fileNo, record0.location.lineNo, record0.location.pc,
        reinterpret_cast<__ubuf__ half *>(record0.dst), reinterpret_cast<__ubuf__ half *>(record0.src0),
        reinterpret_cast<__ubuf__ uint32_t *>(record0.src1), 534534534534);
    __sanitizer_report_VBS32_f16(memInfo.data(), record1.location.fileNo, record1.location.lineNo, record1.location.pc,
        reinterpret_cast<__ubuf__ half *>(record1.dst), reinterpret_cast<__ubuf__ half *>(record1.src0),
        reinterpret_cast<__ubuf__ uint32_t *>(record1.src1), record1.repeat, record1.dstBlockStride,
        record1.src0BlockStride, record1.src1BlockStride, record1.dstRepeatStride, record1.src0RepeatStride,
        record1.src1RepeatStride);
    
    auto binaryOp = reinterpret_cast<BinaryOpRecord *>(memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead) +
           sizeof(RecordType) * 2 + sizeof(BinaryOpRecord));
 
    ASSERT_EQ(binaryOp->dst, record1.dst);
    ASSERT_EQ(binaryOp->src0, record1.src0);
    ASSERT_EQ(binaryOp->src1, record1.src1);
    ASSERT_EQ(binaryOp->location.fileNo, record1.location.fileNo);
    ASSERT_EQ(binaryOp->location.lineNo, record1.location.lineNo);
    ASSERT_EQ(binaryOp->repeat, record1.repeat);
    ASSERT_EQ(binaryOp->dstBlockStride, 0);
    ASSERT_EQ(binaryOp->src0BlockStride, 0);
    ASSERT_EQ(binaryOp->src1BlockStride, 0);
    ASSERT_EQ(binaryOp->dstRepeatStride, 1);
    ASSERT_EQ(binaryOp->src0RepeatStride, 1);
    ASSERT_EQ(binaryOp->src1RepeatStride, 1);
    ASSERT_EQ(binaryOp->dstBlockSize, 256);
    ASSERT_EQ(binaryOp->src0BlockSize, 64);
    ASSERT_EQ(binaryOp->src1BlockSize, 128);
    ASSERT_EQ(binaryOp->dstBlockNum, 1);
    ASSERT_EQ(binaryOp->src0BlockNum, 1);
    ASSERT_EQ(binaryOp->src1BlockNum, 1);
}

TEST(VbitsortCalcInstructions, dump_float_type_records_and_parse_last_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    auto record0 = CreateRandomBinaryOpRecord();
    auto record1 = CreateRandomBinaryOpRecord();
 
    __sanitizer_report_VBS32_f32(memInfo.data(), record0.location.fileNo, record0.location.lineNo, record0.location.pc,
        reinterpret_cast<__ubuf__ float *>(record0.dst), reinterpret_cast<__ubuf__ float *>(record0.src0),
        reinterpret_cast<__ubuf__ uint32_t *>(record0.src1), 534534534534);
    __sanitizer_report_VBS32_f32(memInfo.data(), record1.location.fileNo, record1.location.lineNo, record1.location.pc,
        reinterpret_cast<__ubuf__ float *>(record1.dst), reinterpret_cast<__ubuf__ float *>(record1.src0),
        reinterpret_cast<__ubuf__ uint32_t *>(record1.src1), record1.repeat, record1.dstBlockStride,
        record1.src0BlockStride, record1.src1BlockStride, record1.dstRepeatStride, record1.src0RepeatStride,
        record1.src1RepeatStride);
    
    auto binaryOp = reinterpret_cast<BinaryOpRecord *>(memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead) +
           sizeof(RecordType) * 2 + sizeof(BinaryOpRecord));

    ASSERT_EQ(binaryOp->dst, record1.dst);
    ASSERT_EQ(binaryOp->src0, record1.src0);
    ASSERT_EQ(binaryOp->src1, record1.src1);
    ASSERT_EQ(binaryOp->location.fileNo, record1.location.fileNo);
    ASSERT_EQ(binaryOp->location.lineNo, record1.location.lineNo);
    ASSERT_EQ(binaryOp->repeat, record1.repeat);
    ASSERT_EQ(binaryOp->dstBlockStride, 0);
    ASSERT_EQ(binaryOp->src0BlockStride, 0);
    ASSERT_EQ(binaryOp->src1BlockStride, 0);
    ASSERT_EQ(binaryOp->dstRepeatStride, 1);
    ASSERT_EQ(binaryOp->src0RepeatStride, 1);
    ASSERT_EQ(binaryOp->src1RepeatStride, 1);
    ASSERT_EQ(binaryOp->dstBlockSize, 256);
    ASSERT_EQ(binaryOp->src0BlockSize, 128);
    ASSERT_EQ(binaryOp->src1BlockSize, 128);
    ASSERT_EQ(binaryOp->dstBlockNum, 1);
    ASSERT_EQ(binaryOp->src0BlockNum, 1);
    ASSERT_EQ(binaryOp->src1BlockNum, 1);
}
