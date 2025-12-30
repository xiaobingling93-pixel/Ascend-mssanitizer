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
#include "plugin/ccec/calc_instructions/vpadding.cpp"

using namespace Sanitizer;

TEST(VpaddingCalcInstructions, dump_u16_type_records_and_parse_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    auto record0 = CreateRandomUnaryOpRecord();
    auto record1 = CreateRandomUnaryOpRecord();
    __sanitizer_report_vpadding(memInfo.data(), record0.location.fileNo, record0.location.lineNo, record0.location.pc,
        reinterpret_cast<__ubuf__ uint16_t*>(record0.dst), reinterpret_cast<__ubuf__ uint16_t*>(record0.src),
        record0.repeat, record0.dstBlockStride, record0.srcBlockStride, record0.dstRepeatStride,
        record0.srcRepeatStride, record0.dstBlockNum, record0.srcBlockNum, record0.dstBlockSize, record0.srcBlockSize);

    auto config = ExtractConfigFromUnaryOpRecord(record1);
    __sanitizer_report_vpadding(memInfo.data(), record1.location.fileNo, record1.location.lineNo, record1.location.pc,
        reinterpret_cast<__ubuf__ uint16_t*>(record1.dst), reinterpret_cast<__ubuf__ uint16_t*>(record1.src), config);
    
    auto record = reinterpret_cast<UnaryOpRecord *>(memInfo.data() + sizeof(RecordGlobalHead) +
        sizeof(RecordBlockHead) + sizeof(RecordType) * 2 + sizeof(UnaryOpRecord) * 1);

    ASSERT_TRUE(IsEqual(*record, record1));
}

TEST(VpaddingCalcInstructions, dump_u32_type_records_and_parse_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    auto record0 = CreateRandomUnaryOpRecord();
    auto record1 = CreateRandomUnaryOpRecord();
    __sanitizer_report_vpadding(memInfo.data(), record0.location.fileNo, record0.location.lineNo, record0.location.pc,
        reinterpret_cast<__ubuf__ uint32_t*>(record0.dst), reinterpret_cast<__ubuf__ uint32_t*>(record0.src),
        record0.repeat, record0.dstBlockStride, record0.srcBlockStride, record0.dstRepeatStride,
        record0.srcRepeatStride, record0.dstBlockNum, record0.srcBlockNum, record0.dstBlockSize, record0.srcBlockSize);

    auto config = ExtractConfigFromUnaryOpRecord(record1);
    __sanitizer_report_vpadding(memInfo.data(), record1.location.fileNo, record1.location.lineNo, record1.location.pc,
        reinterpret_cast<__ubuf__ uint32_t*>(record1.dst), reinterpret_cast<__ubuf__ uint32_t*>(record1.src), config);
    
    auto record = reinterpret_cast<UnaryOpRecord *>(memInfo.data() + sizeof(RecordGlobalHead) +
        sizeof(RecordBlockHead) + sizeof(RecordType) * 2 + sizeof(UnaryOpRecord) * 1);

    ASSERT_TRUE(IsEqual(*record, record1));
}
