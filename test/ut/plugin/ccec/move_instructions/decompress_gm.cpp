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


#include <utility>
#include <gtest/gtest.h>

#include "../data_process.h"
#include "../../ccec_defs.h"
#include "plugin/ccec/move_instructions/decompress_gm.cpp"

using namespace Sanitizer;

template<MemType srcMemType>
DecompressHeaderRecord CreateDecompressHeaderRecord()
{
    DecompressHeaderRecord record{};
    record.src = RandInt(0, 0x1000000);
    record.nBlock = RandInt(0, 0xFF);
    
    record.location.fileNo = RandInt(0, 0x100000);
    record.location.lineNo = RandInt(0, 0x1000);

    record.srcMemType = srcMemType;

    return record;
}

uint64_t ExtractDecompressHeaderRecord(const DecompressHeaderRecord &record)
{
    constexpr uint64_t nBlockMask = 0x3FFUL;
    constexpr uint64_t nBlockShift = 0;

    return ((record.nBlock & nBlockMask) << nBlockShift);
}

TEST(DecompressHeaderInstructions, load_decompress_header_from_gm)
{
    auto record = CreateDecompressHeaderRecord<MemType::GM>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractDecompressHeaderRecord(record);

    // #1298
    __sanitizer_report_load_decompress_header_from_gm(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__gm__ void *>(record.src), config);

    __sanitizer_report_load_decompress_header_from_gm(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__gm__ void *>(record.src), record.nBlock, 0);


    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DECOMPRESS_HEADER>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DecompressHeaderRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DECOMPRESS_HEADER>(ptr, record));
}
