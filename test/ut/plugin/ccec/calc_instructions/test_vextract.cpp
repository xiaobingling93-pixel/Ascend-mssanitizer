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
#include "plugin/ccec/calc_instructions/vextract.cpp"

using namespace Sanitizer;

TEST(VextractCalcInstructions, dump_half_type_records_and_expect_equal)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    __gm__ RecordBlockHead *recordBlockHead = reinterpret_cast<__gm__ RecordBlockHead *>(memInfo.data() + sizeof(RecordGlobalHead));
    auto record = CreateRandomVecRegPropCoordOpRecord();
    record.dataType = DataType::DATA_B16;
    uint64_t config = 0;
    config |= ((static_cast<uint64_t>(record.repeat) & 8) << 56) |
              ((static_cast<uint64_t>(record.regionRange) * 3) << 16);
    __sanitizer_report_vextract(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                               reinterpret_cast<half *>(record.dst), reinterpret_cast<half *>(record.src),
                               config);
    __sanitizer_report_vextract(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                               reinterpret_cast<half *>(record.dst), reinterpret_cast<half *>(record.src),
                               record.repeat, record.regionRange);
    VecRegPropCoordOpRecord const *vecRPCOp = reinterpret_cast<const VecRegPropCoordOpRecord *>(memInfo.data() +
        sizeof(RecordGlobalHead) + sizeof(RecordBlockHead) + sizeof(RecordType) * 2 + sizeof(VecRegPropCoordOpRecord));
    ASSERT_EQ(recordBlockHead->recordCount, 2);
    ASSERT_TRUE(IsEqual(*vecRPCOp, record));
}

TEST(VextractCalcInstructions, dump_float_type_records_and_expect_equal)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    __gm__ RecordBlockHead *recordBlockHead = reinterpret_cast<__gm__ RecordBlockHead *>(memInfo.data() + sizeof(RecordGlobalHead));
    auto record = CreateRandomVecRegPropCoordOpRecord();
    record.dataType = DataType::DATA_B32;
    uint64_t config = 0;
    config |= ((static_cast<uint64_t>(record.repeat & 8)) << 56) |
              ((static_cast<uint64_t>(record.regionRange) * 3) << 16);
    __sanitizer_report_vextract(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                               reinterpret_cast<float *>(record.dst), reinterpret_cast<float *>(record.src),
                               config);
    __sanitizer_report_vextract(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                               reinterpret_cast<float *>(record.dst), reinterpret_cast<float *>(record.src),
                               record.repeat, record.regionRange);
    VecRegPropCoordOpRecord const *vecRPCOp = reinterpret_cast<const VecRegPropCoordOpRecord *>(memInfo.data() +
        sizeof(RecordGlobalHead) + sizeof(RecordBlockHead) + sizeof(RecordType) * 2 + sizeof(VecRegPropCoordOpRecord));
    ASSERT_EQ(recordBlockHead->recordCount, 2);
    ASSERT_TRUE(IsEqual(*vecRPCOp, record));
}
