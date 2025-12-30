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
#include "plugin/ccec/calc_instructions/vbrcb.cpp"

using namespace Sanitizer;

TEST(VbrcbCalcInstructions, dump_uint16_t_type_records_and_parse_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    __gm__  RecordBlockHead* recordBlockHead = reinterpret_cast<__gm__ RecordBlockHead *>(memInfo.data() + sizeof(RecordGlobalHead));
    auto record0 = CreateRandomUnaryOpRecord();
    auto record1 = CreateRandomUnaryOpRecord();

    __sanitizer_report_vbrcb(memInfo.data(), record0.location.fileNo, record0.location.lineNo, record0.location.pc,
        reinterpret_cast<__ubuf__ uint16_t *>(record0.dst), reinterpret_cast<__ubuf__ uint16_t *>(record0.src),
        42342342);
    __sanitizer_report_vbrcb(memInfo.data(), record1.location.fileNo, record1.location.lineNo, record1.location.pc,
        reinterpret_cast<__ubuf__ uint16_t *>(record1.dst), reinterpret_cast<__ubuf__ uint16_t *>(record1.src),
        record1.dstBlockStride, record1.dstRepeatStride, record1.repeat);

    UnaryOpRecord const *unaryOp = reinterpret_cast<UnaryOpRecord const*>(memInfo.data() + sizeof(RecordGlobalHead) +
            sizeof(RecordBlockHead) + sizeof(RecordType) * 2 + sizeof(UnaryOpRecord) * 1);
    ASSERT_EQ(recordBlockHead->recordCount, 2);
    ASSERT_EQ(unaryOp->dstBlockNum, 8);
    ASSERT_EQ(unaryOp->srcBlockNum, 1);
    ASSERT_EQ(unaryOp->dstBlockSize, 32);
    ASSERT_EQ(unaryOp->srcBlockSize, 16);
    ASSERT_EQ(unaryOp->srcBlockStride, 0);
    ASSERT_EQ(unaryOp->srcRepeatStride, 1);
    ASSERT_EQ(unaryOp->repeat, record1.repeat);
    ASSERT_EQ(unaryOp->src, record1.src);
    ASSERT_EQ(unaryOp->dst, record1.dst);
}

TEST(VbrcbCalcInstructions, dump_uint32_t_type_records_and_parse_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    __gm__  RecordBlockHead* recordBlockHead = reinterpret_cast<__gm__ RecordBlockHead *>(memInfo.data() + sizeof(RecordGlobalHead));
    auto record0 = CreateRandomUnaryOpRecord();
    auto record1 = CreateRandomUnaryOpRecord();

    __sanitizer_report_vbrcb(memInfo.data(), record0.location.fileNo, record0.location.lineNo, record0.location.pc,
        reinterpret_cast<__ubuf__ uint32_t *>(record0.dst), reinterpret_cast<__ubuf__ uint32_t *>(record0.src),
        42342342);
    __sanitizer_report_vbrcb(memInfo.data(), record1.location.fileNo, record1.location.lineNo, record1.location.pc,
        reinterpret_cast<__ubuf__ uint32_t *>(record1.dst), reinterpret_cast<__ubuf__ uint32_t *>(record1.src),
        record1.dstBlockStride, record1.dstRepeatStride, record1.repeat);

    UnaryOpRecord const *unaryOp = reinterpret_cast<UnaryOpRecord const*>(memInfo.data() + sizeof(RecordGlobalHead) +
            sizeof(RecordBlockHead) + sizeof(RecordType) * 2 + sizeof(UnaryOpRecord) * 1);
    ASSERT_EQ(recordBlockHead->recordCount, 2);
    ASSERT_EQ(unaryOp->dstBlockNum, 8);
    ASSERT_EQ(unaryOp->srcBlockNum, 1);
    ASSERT_EQ(unaryOp->dstBlockSize, 32);
    ASSERT_EQ(unaryOp->srcBlockSize, 32);
    ASSERT_EQ(unaryOp->srcBlockStride, 0);
    ASSERT_EQ(unaryOp->srcRepeatStride, 1);
    ASSERT_EQ(unaryOp->repeat, record1.repeat);
    ASSERT_EQ(unaryOp->src, record1.src);
    ASSERT_EQ(unaryOp->dst, record1.dst);
}
