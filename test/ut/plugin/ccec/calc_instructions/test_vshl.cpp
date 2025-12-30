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
#include "plugin/ccec/calc_instructions/vshl.cpp"

using namespace Sanitizer;

TEST(VshlCalcInstructions, dump_int_type_records_and_parse_last_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    __gm__ RecordBlockHead *recordBlockHead = reinterpret_cast<__gm__ RecordBlockHead *>(memInfo.data() + sizeof(RecordGlobalHead));

    auto record0 = CreateRandomUnaryOpRecord();
    auto record1 = CreateRandomUnaryOpRecord();
    auto record2 = CreateRandomUnaryOpRecord();
    auto record3 = CreateRandomUnaryOpRecord();
    auto record4 = CreateRandomUnaryOpRecord();
    auto record5 = CreateRandomUnaryOpRecord();

    __sanitizer_report_vshl(memInfo.data(), record0.location.fileNo, record0.location.lineNo, record0.location.pc,
        reinterpret_cast<__ubuf__ int16_t *>(record0.dst), reinterpret_cast<__ubuf__ int16_t *>(record0.src),
        3, 42342342);
    __sanitizer_report_vshl(memInfo.data(), record1.location.fileNo, record1.location.lineNo, record1.location.pc,
        reinterpret_cast<__ubuf__ int16_t *>(record1.dst), reinterpret_cast<__ubuf__ int16_t *>(record1.src),
        2, record1.repeat, record1.dstBlockStride, record1.srcBlockStride, record1.dstRepeatStride,
        record1.srcRepeatStride);
    __sanitizer_report_vshl(memInfo.data(), record2.location.fileNo, record2.location.lineNo, record2.location.pc,
        reinterpret_cast<__ubuf__ int16_t *>(record2.dst), reinterpret_cast<__ubuf__ int16_t *>(record2.src),
        4, record2.repeat, record2.dstBlockStride, record2.srcBlockStride,
        record2.dstRepeatStride, record2.srcRepeatStride, 0, 1);

    __sanitizer_report_vshl(memInfo.data(), record3.location.fileNo, record3.location.lineNo, record3.location.pc,
        reinterpret_cast<__ubuf__ int32_t *>(record3.dst), reinterpret_cast<__ubuf__ int32_t *>(record3.src),
        2, 45345344);
    __sanitizer_report_vshl(memInfo.data(), record4.location.fileNo, record4.location.lineNo, record4.location.pc,
        reinterpret_cast<__ubuf__ int32_t *>(record4.dst), reinterpret_cast<__ubuf__ int32_t *>(record4.src),
        1, record4.repeat, record4.dstBlockStride, record4.srcBlockStride,
        record4.dstRepeatStride, record4.srcRepeatStride);
    __sanitizer_report_vshl(memInfo.data(), record5.location.fileNo, record5.location.lineNo, record5.location.pc,
        reinterpret_cast<__ubuf__ int32_t *>(record5.dst), reinterpret_cast<__ubuf__ int32_t *>(record5.src),
        3, record5.repeat, record5.dstBlockStride, record5.srcBlockStride, record5.dstRepeatStride,
        record5.srcRepeatStride, 1, 1);

    UnaryOpRecord const *unaryOp = reinterpret_cast<UnaryOpRecord const*>(memInfo.data() + sizeof(RecordGlobalHead) +
        sizeof(RecordBlockHead) + sizeof(RecordType) * 6 + sizeof(UnaryOpRecord) * 5);
    ASSERT_EQ(recordBlockHead->recordCount, 6);
    ASSERT_EQ(unaryOp->dstBlockNum, 8);
    ASSERT_EQ(unaryOp->dstBlockSize, 32);
    ASSERT_TRUE(IsEqual(record5, *unaryOp));
}

TEST(VshlCalcInstructions, dump_uint_type_records_and_parse_last_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    __gm__ RecordBlockHead *recordBlockHead = reinterpret_cast<__gm__ RecordBlockHead *>(memInfo.data() + sizeof(RecordGlobalHead));

    auto record0 = CreateRandomUnaryOpRecord();
    auto record1 = CreateRandomUnaryOpRecord();
    auto record2 = CreateRandomUnaryOpRecord();
    auto record3 = CreateRandomUnaryOpRecord();
    auto record4 = CreateRandomUnaryOpRecord();
    auto record5 = CreateRandomUnaryOpRecord();

    __sanitizer_report_vshl(memInfo.data(), record0.location.fileNo, record0.location.lineNo, record0.location.pc,
        reinterpret_cast<__ubuf__ uint16_t *>(record0.dst), reinterpret_cast<__ubuf__ uint16_t *>(record0.src),
        3, 42342342);
    __sanitizer_report_vshl(memInfo.data(), record1.location.fileNo, record1.location.lineNo, record1.location.pc,
        reinterpret_cast<__ubuf__ uint16_t *>(record1.dst), reinterpret_cast<__ubuf__ uint16_t *>(record1.src),
        2, record1.repeat, record1.dstBlockStride, record1.srcBlockStride, record1.dstRepeatStride,
        record1.srcRepeatStride);
    __sanitizer_report_vshl(memInfo.data(), record2.location.fileNo, record2.location.lineNo, record2.location.pc,
        reinterpret_cast<__ubuf__ uint16_t *>(record2.dst), reinterpret_cast<__ubuf__ uint16_t *>(record2.src),
        4, record2.repeat, record2.dstBlockStride, record2.srcBlockStride,
        record2.dstRepeatStride, record2.srcRepeatStride, 0, 1);

    __sanitizer_report_vshl(memInfo.data(), record3.location.fileNo, record3.location.lineNo, record3.location.pc,
        reinterpret_cast<__ubuf__ uint32_t *>(record3.dst), reinterpret_cast<__ubuf__ uint32_t *>(record3.src),
        2, 45345344);
    __sanitizer_report_vshl(memInfo.data(), record4.location.fileNo, record4.location.lineNo, record4.location.pc,
        reinterpret_cast<__ubuf__ uint32_t *>(record4.dst), reinterpret_cast<__ubuf__ uint32_t *>(record4.src),
        1, record4.repeat, record4.dstBlockStride, record4.srcBlockStride,
        record4.dstRepeatStride, record4.srcRepeatStride);
    __sanitizer_report_vshl(memInfo.data(), record5.location.fileNo, record5.location.lineNo, record5.location.pc,
        reinterpret_cast<__ubuf__ uint32_t *>(record5.dst), reinterpret_cast<__ubuf__ uint32_t *>(record5.src),
        3, record5.repeat, record5.dstBlockStride, record5.srcBlockStride, record5.dstRepeatStride,
        record5.srcRepeatStride, 1, 1);

    UnaryOpRecord const *unaryOp = reinterpret_cast<UnaryOpRecord const*>(memInfo.data() + sizeof(RecordGlobalHead) +
        sizeof(RecordBlockHead) + sizeof(RecordType) * 6 + sizeof(UnaryOpRecord) * 5);
    ASSERT_EQ(recordBlockHead->recordCount, 6);
    ASSERT_EQ(unaryOp->dstBlockNum, 8);
    ASSERT_EQ(unaryOp->dstBlockSize, 32);
    ASSERT_TRUE(IsEqual(record5, *unaryOp));
}
