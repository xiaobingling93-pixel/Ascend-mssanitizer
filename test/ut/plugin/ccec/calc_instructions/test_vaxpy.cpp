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
#include "plugin/ccec/calc_instructions/vaxpy.cpp"

using namespace Sanitizer;

TEST(VaxpyCalcInstructions, dump_records_and_parse_each_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    uint8_t recordNum = 12;
    auto records = CreateRandomUnaryOpRecords(recordNum);
    uint8_t idx = 0;
    std::vector<CheckBinaryOpBlockInfo> blockInfoVec =
    {
        {8, 8, 8, 32, 32, 32},
        {8, 8, 8, 32, 32, 32},
        {8, 4, 8, 32, 32, 32},
        {8, 8, 8, 32, 32, 32},
        {8, 8, 8, 32, 32, 32},
        {8, 4, 8, 32, 32, 32},
        {8, 8, 8, 32, 32, 32},
        {8, 8, 8, 32, 32, 32},
        {8, 4, 8, 32, 32, 32},
        {8, 8, 8, 32, 32, 32},
        {8, 8, 8, 32, 32, 32},
        {8, 4, 8, 32, 32, 32},
    };

    CallUnaryScalarConfigFunc<half>(memInfo.data(), records, idx++, __sanitizer_report_vaxpy);
    CallUnaryScalarConfigFunc<float>(memInfo.data(), records, idx++, __sanitizer_report_vaxpy);
    CallUnaryScalarConfigFunc<float, half>(memInfo.data(), records, idx++, __sanitizer_report_vaxpy);

    CallUnaryScalarU8Func<half>(memInfo.data(), records, idx++, __sanitizer_report_vaxpy, bool{}, bool{});
    CallUnaryScalarU8Func<float>(memInfo.data(), records, idx++, __sanitizer_report_vaxpy, bool{}, bool{});
    CallUnaryScalarU8Func<float, half>(memInfo.data(), records, idx++, __sanitizer_report_vaxpy, bool{}, bool{});

#if defined(__DAV_C220__) || defined(__DAV_C220_VEC__)
    CallUnaryScalarU16Func<half>(memInfo.data(), records, idx++, __sanitizer_report_vaxpy);
    CallUnaryScalarU16Func<float>(memInfo.data(), records, idx++, __sanitizer_report_vaxpy);
    CallUnaryScalarU16Func<float, half>(memInfo.data(), records, idx++, __sanitizer_report_vaxpy);
#else
    CallUnaryScalarU8Func<half>(memInfo.data(), records, idx++, __sanitizer_report_vaxpy);
    CallUnaryScalarU8Func<float>(memInfo.data(), records, idx++, __sanitizer_report_vaxpy);
    CallUnaryScalarU8Func<float, half>(memInfo.data(), records, idx++, __sanitizer_report_vaxpy);
#endif

    auto isEqual = MultiIsEqual<BinaryOpRecord, CheckBinaryOpBlockInfo, UnaryOpRecord>
                  (memInfo.data(), records, blockInfoVec);
    ASSERT_TRUE(isEqual);
}
