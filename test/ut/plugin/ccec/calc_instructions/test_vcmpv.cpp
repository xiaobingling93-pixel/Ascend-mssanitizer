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
#include "plugin/ccec/calc_instructions/vcmpv.cpp"

TEST(VcmpvCalcInstructions, dump_records_and_parse_each_expect_success_first)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    uint8_t recordNum = 13;
    auto records = CreateRandomBinaryOpRecords(recordNum);
    uint8_t idx = 0;
    std::vector<CheckBinaryOpBlockInfo> blockInfoVec =
    {
        {1, 8, 8, 16, 32, 32},
        {1, 8, 8, 16, 32, 32},
        {1, 8, 8, 8, 32, 32},
        {1, 8, 8, 8, 32, 32},
        {1, 8, 8, 8, 32, 32},
        {1, 8, 8, 8, 32, 32},
        {1, 8, 8, 16, 32, 32},
        {1, 8, 8, 16, 32, 32},
        {1, 8, 8, 8, 32, 32},
        {1, 8, 8, 8, 32, 32},
        {1, 8, 8, 16, 32, 32},
        {1, 8, 8, 16, 32, 32},
        {1, 8, 8, 8, 32, 32},
    };
 
    CallBinaryConfigFunc<uint8_t, half>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_eq);
    CallBinaryFunc<uint8_t, half>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_eq);
    CallBinaryConfigFunc<uint8_t, float>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_eq);
    CallBinaryFunc<uint8_t, float>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_eq);
    CallBinaryConfigFunc<uint8_t, int32_t>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_eq);
    CallBinaryFunc<uint8_t, int32_t>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_eq);
    
    CallBinaryConfigFunc<uint8_t, half>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_ge);
    CallBinaryFunc<uint8_t, half>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_ge);
    CallBinaryConfigFunc<uint8_t, float>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_ge);
    CallBinaryFunc<uint8_t, float>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_ge);

    CallBinaryConfigFunc<uint8_t, half>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_gt);
    CallBinaryFunc<uint8_t, half>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_gt);
    CallBinaryConfigFunc<uint8_t, float>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_gt);

    bool isEqual = MultiIsEqual<BinaryOpRecord, CheckBinaryOpBlockInfo>(memInfo.data(), records, blockInfoVec);
    ASSERT_TRUE(isEqual);
}

TEST(VcmpvCalcInstructions, dump_records_and_parse_each_expect_success_second)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    uint8_t recordNum = 13;
    auto records = CreateRandomBinaryOpRecords(recordNum);
    uint8_t idx = 0;
    std::vector<CheckBinaryOpBlockInfo> blockInfoVec =
    {
        {1, 8, 8, 8, 32, 32},
        {1, 8, 8, 16, 32, 32},
        {1, 8, 8, 16, 32, 32},
        {1, 8, 8, 8, 32, 32},
        {1, 8, 8, 8, 32, 32},
        {1, 8, 8, 16, 32, 32},
        {1, 8, 8, 16, 32, 32},
        {1, 8, 8, 8, 32, 32},
        {1, 8, 8, 8, 32, 32},
        {1, 8, 8, 16, 32, 32},
        {1, 8, 8, 16, 32, 32},
        {1, 8, 8, 8, 32, 32},
        {1, 8, 8, 8, 32, 32},
    };
 
    CallBinaryFunc<uint8_t, float>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_gt);

    CallBinaryConfigFunc<uint8_t, half>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_le);
    CallBinaryFunc<uint8_t, half>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_le);
    CallBinaryConfigFunc<uint8_t, float>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_le);
    CallBinaryFunc<uint8_t, float>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_le);

    CallBinaryConfigFunc<uint8_t, half>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_lt);
    CallBinaryFunc<uint8_t, half>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_lt);
    CallBinaryConfigFunc<uint8_t, float>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_lt);
    CallBinaryFunc<uint8_t, float>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_lt);

    CallBinaryConfigFunc<uint8_t, half>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_ne);
    CallBinaryFunc<uint8_t, half>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_ne);
    CallBinaryConfigFunc<uint8_t, float>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_ne);
    CallBinaryFunc<uint8_t, float>(memInfo.data(), records, idx++, __sanitizer_report_vcmpv_ne);

    bool isEqual = MultiIsEqual<BinaryOpRecord, CheckBinaryOpBlockInfo>(memInfo.data(), records, blockInfoVec);
    ASSERT_TRUE(isEqual);
}