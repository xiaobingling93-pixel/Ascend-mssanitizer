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

#include "../data_process.h"
#include "../../ccec_defs.h"
#include "plugin/ccec/move_instructions/load_smask.cpp"

using namespace Sanitizer;

template <MemType srcMemType>
LoadSmaskRecord CreateLoadSmaskRecord()
{
    LoadSmaskRecord record{};
    ///smask table is 2KB
    record.dst = RandInt(0, 0x800);

    record.src = RandInt(0, 0x1000000);
    record.location.fileNo = RandInt(0, 0x100000);
    record.location.lineNo = RandInt(0, 0x1000);
    record.location.pc = RandInt(0, 0x1000);

    ///smask size is uint_8 and 32B alignment
    record.smaskSize = RandInt(0, 0xf) * 32;

    record.srcMemType = srcMemType;

    return record;
}

uint64_t ExtractLoadSmaskRecord(const LoadSmaskRecord &record)
{
    uint64_t config = 0;

    ///smaskSize 一共8位，由{Xt[11], Xt[6:0]}组成，当前为低7位
    constexpr uint64_t smaskSizeMaskLow = 0x7FUL;

    ///smaskSize 最高位
    constexpr uint64_t smaskSizeMaskHigh = 0x80UL;
    ///smaskSize 最高位在config中的偏移量
    constexpr uint64_t smaskSizeHighShift = 4;

    ///smaskSize 以32B对齐，且config每一位代表16位宽
    constexpr uint64_t smaskSizeAlign = 32;
    constexpr uint64_t smaskWidthSize = 16;

    return config | ((record.smaskSize / smaskSizeAlign * smaskWidthSize)  & smaskSizeMaskLow) |
           (((record.smaskSize / smaskSizeAlign * smaskWidthSize) & smaskSizeMaskHigh) << smaskSizeHighShift);
}

TEST(LoadSmaskInstructions, load_smask_table_from_gm)
{
    auto record = CreateLoadSmaskRecord<MemType::GM>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoadSmaskRecord(record);

    // 1526
    __sanitizer_report_load_smask_table_from_gm(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc, record.dst, reinterpret_cast<__gm__ void*>(record.src),
                                       config);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_SMASK>(ptr, record));
}

TEST(LoadSmaskInstructions, load_smask_table_from_ub)
{
    auto record = CreateLoadSmaskRecord<MemType::UB>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoadSmaskRecord(record);

    // 1527
    __sanitizer_report_load_smask_table_from_ub(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                                record.location.pc, record.dst,
                                                reinterpret_cast<__ubuf__ void*>(record.src), config);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_SMASK>(ptr, record));
}
