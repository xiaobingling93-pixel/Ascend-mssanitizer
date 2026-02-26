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
#include "plugin/ccec/calc_instructions/cmpmask.cpp"

using namespace Sanitizer;

TEST(VcmpmaskCalcInstructions, dump_get_cmpmask_type_records_and_parse_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    __gm__  RecordBlockHead* recordHead = reinterpret_cast<__gm__ RecordBlockHead *>(memInfo.data() + sizeof(RecordGlobalHead));
    auto record = CreateRandomCmpMaskRecord();
    __sanitizer_report_get_cmpmask(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
        reinterpret_cast<__ubuf__ half *>(record.addr));

    CmpMaskRecord const *cmpMaskRecord = reinterpret_cast<CmpMaskRecord const*>(memInfo.data() + sizeof(RecordGlobalHead) +
        sizeof(RecordBlockHead) + sizeof(RecordType) * 1);
    ASSERT_EQ(recordHead->recordCount, 1);
    ASSERT_EQ(cmpMaskRecord->addr, record.addr);
    ASSERT_EQ(cmpMaskRecord->size, 16);
    ASSERT_EQ(cmpMaskRecord->accessType, AccessType::WRITE);
}

TEST(VcmpmaskCalcInstructions, dump_set_cmpmask_type_records_and_parse_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    __gm__  RecordGlobalHead* globalHead = reinterpret_cast<__gm__ RecordGlobalHead *>(memInfo.data());
    auto record = CreateRandomCmpMaskRecord();
    __sanitizer_report_set_cmpmask(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
        reinterpret_cast<__ubuf__ half *>(record.addr));

    int64_t regIdx = GetRegisterIdx();
    ASSERT_EQ(globalHead->registers[regIdx].cmpMaskAddr, record.addr);
}
