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
#include "plugin/ccec/calc_instructions/vcmax.cpp"

using namespace Sanitizer;

TEST(VcmaxCalcInstructions, dump_records_and_parse_each_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    uint8_t recordNum = 13;
    auto records = CreateRandomReduceOpRecords(recordNum);
    uint8_t idx = 0;
    std::vector<CheckReduceOpBlockInfo> blockInfoVec =
    {
        {1, 8, 2, 32},
        {1, 8, 2, 32},
        {1, 8, 4, 32},
        {1, 8, 2, 32},
        {1, 8, 2, 32},
        {1, 8, 2, 32},
        {1, 8, 2, 32},
        {1, 8, 4, 32},
        {1, 8, 2, 32},
        {1, 8, 4, 32},
        {1, 8, 8, 32},
        {1, 8, 2, 32},
        {1, 8, 4, 32},
    };

    CallReduceConfigFunc<half>(memInfo.data(), records, idx++, __sanitizer_report_vcmax);
    CallReduceConfigFunc<half>(memInfo.data(), records, idx++, __sanitizer_report_vcmax, bool{});
    CallReduceConfigFunc<float>(memInfo.data(), records, idx++, __sanitizer_report_vcmax, bool{});
    CallReduceConfigFunc<uint16_t>(memInfo.data(), records, idx++, __sanitizer_report_vcmax, bool{});

    CallReduceFunc<half>(memInfo.data(), records, idx++, __sanitizer_report_vcmax);
    CallReduceFunc<half>(memInfo.data(), records, idx++, __sanitizer_report_vcmax, bool{}, bool{});
    CallReduceFunc<half>(memInfo.data(), records, idx++, __sanitizer_report_vcmax, bool{}, bool{}, bool{});
    CallReduceFunc<float>(memInfo.data(), records, idx++, __sanitizer_report_vcmax, bool{}, bool{}, bool{});
    CallReduceFunc<uint16_t>(memInfo.data(), records, idx++, __sanitizer_report_vcmax, bool{}, bool{}, bool{});

    CallReduceConfigFunc<half>(memInfo.data(), records, idx++, __sanitizer_report_vcmax, OrderType::INDEX_VALUE);
    CallReduceConfigFunc<float>(memInfo.data(), records, idx++, __sanitizer_report_vcmax, OrderType::VALUE_INDEX);
    CallReduceFunc<half>(memInfo.data(), records, idx++, __sanitizer_report_vcmax, OrderType::ONLY_VALUE);
    CallReduceFunc<float>(memInfo.data(), records, idx++, __sanitizer_report_vcmax, OrderType::ONLY_INDEX);

    bool isEqual = MultiIsEqual<ReduceOpRecord, CheckReduceOpBlockInfo>(memInfo.data(), records, blockInfoVec);
    ASSERT_TRUE(isEqual);
}

TEST(VcmaxCalcInstructions, dump_records_and_parse_each_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    auto record = Sanitizer::CreateRandomReduceOpRecord();
    record.maskMode = MaskMode::MASK_COUNT;
    record.vectorMask.mask0 = 128 + 60;
    record.vectorMask.mask1 = 0;
    __sanitizer_report_vcmax(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
    reinterpret_cast<__ubuf__ half*>(record.dst), reinterpret_cast<__ubuf__ half*>(record.src),
    1, record.dstRepeatStride, record.srcBlockStride, record.srcRepeatStride, Order_t(1));
    ReduceOpRecord const *reduceOpRecord =
        reinterpret_cast<ReduceOpRecord *>(memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead) + sizeof(RecordType));
    ASSERT_EQ(reduceOpRecord->location.fileNo, record.location.fileNo);
    ASSERT_EQ(reduceOpRecord->location.lineNo, record.location.lineNo);
    ASSERT_EQ(reduceOpRecord->location.pc, record.location.pc);
    ASSERT_EQ(reduceOpRecord->dstDataBits, 16 * 2);
}
