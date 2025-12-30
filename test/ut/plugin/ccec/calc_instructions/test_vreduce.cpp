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
#include "plugin/ccec/calc_instructions/vreduce.cpp"

TEST(VreduceCalcInstructions, vreduce_dump_records_and_parse_last_expect_success_1)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    __gm__ RecordBlockHead *recordBlockHead = reinterpret_cast<__gm__ RecordBlockHead *>(memInfo.data() + sizeof(RecordGlobalHead));

    auto record0 = CreateRandomBinaryOpRecord();
    auto record1 = CreateRandomBinaryOpRecord();
    auto record2 = CreateRandomBinaryOpRecord();
    auto record3 = CreateRandomBinaryOpRecord();
    auto record4 = CreateRandomBinaryOpRecord();
    auto record5 = CreateRandomBinaryOpRecord();

    __sanitizer_report_vreduce(memInfo.data(), record0.location.fileNo, record0.location.lineNo, record0.location.pc,
        reinterpret_cast<__ubuf__ uint16_t *>(record0.dst), reinterpret_cast<__ubuf__ uint16_t *>(record0.src0),
        reinterpret_cast<__ubuf__ uint16_t *>(record0.src1), 144405493639545089);   // mode = 0
    __sanitizer_report_vreduce(memInfo.data(), record1.location.fileNo, record1.location.lineNo, record1.location.pc,
        reinterpret_cast<__ubuf__ uint16_t *>(record1.dst), reinterpret_cast<__ubuf__ uint16_t *>(record1.src0),
        reinterpret_cast<__ubuf__ uint16_t *>(record1.src1), record1.repeat, record1.dstBlockStride,
        record1.src0BlockStride, record1.src1BlockStride, record1.dstRepeatStride, record1.src0RepeatStride,
        record1.src1RepeatStride);
    __sanitizer_report_vreduce(memInfo.data(), record2.location.fileNo, record2.location.lineNo, record2.location.pc,
        reinterpret_cast<__ubuf__ uint16_t *>(record2.dst), reinterpret_cast<__ubuf__ uint16_t *>(record2.src0),
        reinterpret_cast<__ubuf__ uint16_t *>(record2.src1), record2.repeat, record2.src0BlockStride, 0,
         record2.src0RepeatStride, record2.src1RepeatStride);
    __sanitizer_report_vreduce(memInfo.data(), record3.location.fileNo, record3.location.lineNo, record3.location.pc,
        reinterpret_cast<__ubuf__ uint32_t *>(record3.dst), reinterpret_cast<__ubuf__ uint32_t *>(record3.src0),
        reinterpret_cast<__ubuf__ uint32_t *>(record3.src1), 144405493639545089);   // mode = 0
    __sanitizer_report_vreduce(memInfo.data(), record4.location.fileNo, record4.location.lineNo, record4.location.pc,
        reinterpret_cast<__ubuf__ uint32_t *>(record4.dst), reinterpret_cast<__ubuf__ uint32_t *>(record4.src0),
        reinterpret_cast<__ubuf__ uint32_t *>(record4.src1), record4.repeat, record4.dstBlockStride,
        record4.src0BlockStride, record4.src1BlockStride, record4.dstRepeatStride, record4.src0RepeatStride,
        record4.src1RepeatStride);
    __sanitizer_report_vreduce(memInfo.data(), record5.location.fileNo, record5.location.lineNo, record5.location.pc,
        reinterpret_cast<__ubuf__ uint32_t *>(record5.dst), reinterpret_cast<__ubuf__ uint32_t *>(record5.src0),
        reinterpret_cast<__ubuf__ uint32_t *>(record5.src1), record5.repeat, record5.src0BlockStride, 0,
         record5.src0RepeatStride, record5.src1RepeatStride);

    BinaryOpRecord const * binaryOp = reinterpret_cast<BinaryOpRecord const *>(memInfo.data() + sizeof(RecordGlobalHead) +
        sizeof(RecordBlockHead) + sizeof(RecordType) * 6 + sizeof(BinaryOpRecord) * 5);

    ASSERT_EQ(recordBlockHead->recordCount, 6);
    ASSERT_EQ(binaryOp->dstBlockNum, 8);
    ASSERT_EQ(binaryOp->dstBlockSize, 32);
    ASSERT_EQ(record5.dst, binaryOp->dst);
    ASSERT_EQ(record5.src0, binaryOp->src0);
    ASSERT_EQ(record5.src1, binaryOp->src1);
    ASSERT_EQ(record5.repeat, binaryOp->repeat);
    ASSERT_EQ(record5.src0BlockStride, binaryOp->src0BlockStride);
    ASSERT_EQ(record5.src0RepeatStride, binaryOp->src0RepeatStride);
    ASSERT_EQ(record5.src1RepeatStride, binaryOp->src1RepeatStride);
}

TEST(VreduceCalcInstructions, vreduce_dump_records_and_parse_last_expect_success_2)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    __gm__ RecordBlockHead *recordBlockHead = reinterpret_cast<__gm__ RecordBlockHead *>(memInfo.data() + sizeof(RecordGlobalHead));

    auto record0 = CreateRandomBinaryOpRecord();
    auto record1 = CreateRandomBinaryOpRecord();
    auto record2 = CreateRandomBinaryOpRecord();
    auto record3 = CreateRandomBinaryOpRecord();

    __sanitizer_report_vreduce(memInfo.data(), record0.location.fileNo, record0.location.lineNo, record0.location.pc,
        reinterpret_cast<__ubuf__ uint16_t *>(record0.dst), reinterpret_cast<__ubuf__ uint16_t *>(record0.src0),
        reinterpret_cast<__ubuf__ uint16_t *>(record0.src1), 144405493639610625);   // mode = 1
    __sanitizer_report_vreduce(memInfo.data(), record1.location.fileNo, record1.location.lineNo, record1.location.pc,
        reinterpret_cast<__ubuf__ uint16_t *>(record1.dst), reinterpret_cast<__ubuf__ uint16_t *>(record1.src0),
        reinterpret_cast<__ubuf__ uint16_t *>(record1.src1), record1.repeat, record1.src0BlockStride, 3,
         record1.src0RepeatStride, record1.src1RepeatStride);
    __sanitizer_report_vreduce(memInfo.data(), record2.location.fileNo, record2.location.lineNo, record2.location.pc,
        reinterpret_cast<__ubuf__ uint32_t *>(record2.dst), reinterpret_cast<__ubuf__ uint32_t *>(record2.src0),
        reinterpret_cast<__ubuf__ uint32_t *>(record2.src1), 144405493639676161);   // mode = 2
    __sanitizer_report_vreduce(memInfo.data(), record3.location.fileNo, record3.location.lineNo, record3.location.pc,
        reinterpret_cast<__ubuf__ uint32_t *>(record3.dst), reinterpret_cast<__ubuf__ uint32_t *>(record3.src0),
        reinterpret_cast<__ubuf__ uint32_t *>(record3.src1), record3.repeat, record3.src0BlockStride, 4,
         record3.src0RepeatStride, record3.src1RepeatStride);

    UnaryOpRecord const *unaryOp = reinterpret_cast<UnaryOpRecord const*>(memInfo.data() + sizeof(RecordGlobalHead) +
        sizeof(RecordBlockHead) + sizeof(RecordType) * 4 + sizeof(UnaryOpRecord) * 3);

    ASSERT_EQ(recordBlockHead->recordCount, 4);
    ASSERT_EQ(unaryOp->dstBlockNum, 2);
    ASSERT_EQ(unaryOp->dstBlockSize, 32);
    ASSERT_EQ(record3.dst, unaryOp->dst);
    ASSERT_EQ(record3.src0, unaryOp->src);
    ASSERT_EQ(record3.repeat, unaryOp->repeat);
    ASSERT_EQ(record3.src0BlockStride, unaryOp->srcBlockStride);
    ASSERT_EQ(record3.src0RepeatStride, unaryOp->srcRepeatStride);
}
