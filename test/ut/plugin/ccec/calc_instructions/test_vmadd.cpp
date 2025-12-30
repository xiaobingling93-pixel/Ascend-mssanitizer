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
#include "plugin/ccec/calc_instructions/vmadd.cpp"

TEST(VmaddCalcInstructions, dump_records_and_parse_each_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    uint8_t recordNum = 6;
    auto records = CreateRandomBinaryOpRecords(recordNum);
    uint8_t idx = 0;
    std::vector<CheckBinaryOpBlockInfo> blockInfoVec(6);
    CheckBinaryOpBlockInfo initialValue = {8, 8, 8, 32, 32, 32};
    std::fill_n(blockInfoVec.begin(), blockInfoVec.size(), initialValue);
 
    CallBinaryConfigFunc<half>(memInfo.data(), records, idx++, __sanitizer_report_vmadd);
    CallBinaryConfigFunc<float>(memInfo.data(), records, idx++, __sanitizer_report_vmadd);

    CallBinaryFunc<half>(memInfo.data(), records, idx++, __sanitizer_report_vmadd);
    CallBinaryFunc<float>(memInfo.data(), records, idx++, __sanitizer_report_vmadd);

    CallBinaryFunc<half>(memInfo.data(), records, idx++, __sanitizer_report_vmadd, bool{}, bool{});
    CallBinaryFunc<float>(memInfo.data(), records, idx++, __sanitizer_report_vmadd, bool{}, bool{});

    bool isEqual = MultiIsEqual<BinaryOpRecord, CheckBinaryOpBlockInfo>(memInfo.data(), records, blockInfoVec);
    ASSERT_TRUE(isEqual);
}
