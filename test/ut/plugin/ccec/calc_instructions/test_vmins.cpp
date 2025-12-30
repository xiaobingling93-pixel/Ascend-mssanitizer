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
#include "plugin/ccec/calc_instructions/vmins.cpp"

using namespace Sanitizer;

TEST(VminsCalcInstructions, dump_records_and_parse_each_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    uint8_t recordNum = 12;
    auto records = CreateRandomUnaryOpRecords(recordNum);
    uint8_t idx = 0;
    std::vector<CheckUnaryOpBlockInfo> blockInfoVec(12);
    CheckUnaryOpBlockInfo  initialValue= {8, 8, 32, 32};
    std::fill_n(blockInfoVec.begin(), blockInfoVec.size(), initialValue);

    CallUnaryScalarConfigFunc<half>(memInfo.data(), records, idx++, __sanitizer_report_vmins);
    CallUnaryScalarConfigFunc<float>(memInfo.data(), records, idx++, __sanitizer_report_vmins);
    CallUnaryScalarConfigFunc<int16_t>(memInfo.data(), records, idx++, __sanitizer_report_vmins);
    CallUnaryScalarConfigFunc<int32_t>(memInfo.data(), records, idx++, __sanitizer_report_vmins);

    CallUnaryScalarU8Func<half>(memInfo.data(), records, idx++, __sanitizer_report_vmins, bool{}, bool{});
    CallUnaryScalarU8Func<float>(memInfo.data(), records, idx++, __sanitizer_report_vmins, bool{}, bool{});
    CallUnaryScalarU8Func<int16_t>(memInfo.data(), records, idx++, __sanitizer_report_vmins, bool{}, bool{});
    CallUnaryScalarU8Func<int32_t>(memInfo.data(), records, idx++, __sanitizer_report_vmins, bool{}, bool{});

    CallUnaryScalarU16Func<half>(memInfo.data(), records, idx++, __sanitizer_report_vmins);
    CallUnaryScalarU16Func<float>(memInfo.data(), records, idx++, __sanitizer_report_vmins);
    CallUnaryScalarU16Func<int16_t>(memInfo.data(), records, idx++, __sanitizer_report_vmins);
    CallUnaryScalarU16Func<int32_t>(memInfo.data(), records, idx++, __sanitizer_report_vmins);

    bool isEqual = MultiIsEqual<UnaryOpRecord, CheckUnaryOpBlockInfo>(memInfo.data(), records, blockInfoVec);
    ASSERT_TRUE(isEqual);
}