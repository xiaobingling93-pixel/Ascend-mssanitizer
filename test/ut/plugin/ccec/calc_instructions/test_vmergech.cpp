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
#include "plugin/ccec/calc_instructions/vmergech.cpp"

using namespace Sanitizer;
using std::vector;

constexpr uint16_t DstElementNum = 256;

TEST(VmergechCalculationInstructions, dump_u8_and_half_type_record_expect_parse_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    auto record0 = CreateRandomUnaryOpRecord();
    auto record1 = CreateRandomUnaryOpRecord();
    auto record2 = CreateRandomUnaryOpRecord();
    auto record3 = CreateRandomUnaryOpRecord();
    auto record4 = CreateRandomUnaryOpRecord();
    uint64_t config0 = (static_cast<uint64_t>(record0.repeat) << 56);
    uint64_t config1 = (static_cast<uint64_t>(record1.repeat) << 56);
    uint64_t config2 = (static_cast<uint64_t>(record2.repeat) << 56);
    uint64_t config3 = (static_cast<uint64_t>(record3.repeat) << 56);
    uint64_t config4 = (static_cast<uint64_t>(record4.repeat) << 56);
    __sanitizer_report_VMERGECH_f16(memInfo.data(), record0.location.fileNo, record0.location.lineNo,
        record0.location.pc, reinterpret_cast<half*>(record0.dst), reinterpret_cast<half*>(record0.src), config0);
    __sanitizer_report_VMERGECH_b8(memInfo.data(), record1.location.fileNo, record1.location.lineNo,
        record1.location.pc, reinterpret_cast<uint8_t*>(record1.dst), reinterpret_cast<uint8_t*>(record1.src), config1);
    __sanitizer_report_VMERGECH_f16(memInfo.data(), record2.location.fileNo, record2.location.lineNo,
        record2.location.pc, reinterpret_cast<half*>(record2.dst), reinterpret_cast<half*>(record2.src), config2);
    __sanitizer_report_VMERGECH_f16(memInfo.data(), record3.location.fileNo, record3.location.lineNo,
        record3.location.pc, reinterpret_cast<half*>(record3.dst), reinterpret_cast<half*>(record3.src), config3);
    __sanitizer_report_VMERGECH_b8(memInfo.data(), record4.location.fileNo, record4.location.lineNo,
        record4.location.pc, reinterpret_cast<uint8_t*>(record4.dst), reinterpret_cast<uint8_t*>(record4.src), config4);

    auto record = reinterpret_cast<UnaryOpRecord *>(memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead) +
                                                    sizeof(RecordType) * 2
        + sizeof(UnaryOpRecord) * 1);
    auto offset = sizeof(RecordType) + sizeof(UnaryOpRecord);

    ASSERT_EQ(record->repeat, record1.repeat);
    ASSERT_EQ(record->dst, record1.dst);
    ASSERT_EQ(record->src, record1.src);
    ASSERT_EQ(record->dstBlockStride, 1);
    ASSERT_EQ(record->srcBlockStride, 1);
    ASSERT_EQ(record->dstBlockSize, 32);
    ASSERT_EQ(record->srcBlockSize, 256);

    record = reinterpret_cast<UnaryOpRecord*>(reinterpret_cast<uint8_t*>(record) + offset * 2);
    ASSERT_EQ(record->repeat, record3.repeat);
    ASSERT_EQ(record->dst, record3.dst);
    ASSERT_EQ(record->src, record3.src);
    ASSERT_EQ(record->dstBlockStride, 1);
    ASSERT_EQ(record->srcBlockStride, 1);
    ASSERT_EQ(record->dstBlockSize, 64);
    ASSERT_EQ(record->srcBlockSize, 256);

    record = reinterpret_cast<UnaryOpRecord*>(reinterpret_cast<uint8_t*>(record) + offset * 1);
    ASSERT_EQ(record->repeat, record4.repeat);
    ASSERT_EQ(record->dst, record4.dst);
    ASSERT_EQ(record->src, record4.src);
    ASSERT_EQ(record->dstBlockStride, 1);
    ASSERT_EQ(record->srcBlockStride, 1);
    ASSERT_EQ(record->dstBlockSize, 32);
    ASSERT_EQ(record->srcBlockSize, 256);
}
