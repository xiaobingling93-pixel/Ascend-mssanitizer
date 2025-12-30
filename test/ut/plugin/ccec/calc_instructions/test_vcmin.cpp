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
#include "../../ccec_defs.h"
#include "../data_process.h"
#include "plugin/ccec/calc_instructions/vcmin.cpp"

TEST(VcminCalcInstructions, dump_records_and_parse_each_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    uint8_t recordNum = 13;
    auto records = CreateRandomReduceOpRecords(recordNum);
    uint8_t idx = 0;
    std::vector<CheckReduceOpBlockInfo> blockInfoVec =
    {
        {1, 8, 2, 32},
        {1, 8, 2, 32},
        {1, 8, 2, 32},
        {1, 8, 2, 32},
        {1, 8, 2, 32},
        {1, 8, 4, 32},
        {1, 8, 4, 32},
        {1, 8, 2, 32},
        {1, 8, 2, 32},
    };

    CallReduceConfigFunc<half>(memInfo.data(), records, idx++, __sanitizer_report_vcmin);
    CallReduceFunc<half>(memInfo.data(), records, idx++, __sanitizer_report_vcmin);
    CallReduceFunc<half>(memInfo.data(), records, idx++, __sanitizer_report_vcmin, bool{}, bool{});
    CallReduceConfigFunc<half>(memInfo.data(), records, idx++, __sanitizer_report_vcmin, bool{});
    CallReduceFunc<half>(memInfo.data(), records, idx++, __sanitizer_report_vcmin, bool{}, bool{}, bool{});

    CallReduceConfigFunc<float>(memInfo.data(), records, idx++, __sanitizer_report_vcmin, bool{});
    CallReduceFunc<float>(memInfo.data(), records, idx++, __sanitizer_report_vcmin, bool{}, bool{}, bool{});

    CallReduceConfigFunc<uint16_t>(memInfo.data(), records, idx++, __sanitizer_report_vcmin, bool{});
    CallReduceFunc<uint16_t>(memInfo.data(), records, idx++, __sanitizer_report_vcmin, bool{}, bool{}, bool{});

    bool isEqual = MultiIsEqual<ReduceOpRecord, CheckReduceOpBlockInfo>(memInfo.data(), records, blockInfoVec);
    ASSERT_TRUE(isEqual);
}
