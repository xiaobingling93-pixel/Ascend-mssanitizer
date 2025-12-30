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
#include "plugin/ccec/move_instructions/mov_align.cpp"

using namespace Sanitizer;

template<MemType srcMemType, MemType dstMemType, DataType dataType>
MovAlignRecord CreateRandomMovAlignRecord()
{
    MovAlignRecord record{};
    record.dst = RandInt(0, 0x1000000);
    record.src = RandInt(0, 0x1000000);
    record.nBurst = RandInt(0, 0xFFF);
    record.lenBurst = RandInt(0, 0x1FFFF);
    record.srcGap = RandInt(0, 0xFFFFFFF);
    record.dstGap = RandInt(0, 0xFFFFFFF);
    record.leftPaddingNum = RandInt(0, 0x3F);
    record.rightPaddingNum = RandInt(0, 0x3F);
    record.location.fileNo = RandInt(0, 0x100000);
    record.location.lineNo = RandInt(0, 0x1000);

    record.srcMemType = srcMemType;
    record.dstMemType = dstMemType;
    record.dataType = dataType;
    return record;
}

std::pair<uint64_t, uint64_t> ExtractConfigFromMovAlignRecord(const MovAlignRecord &record)
{
    constexpr uint64_t paddingNumMask = 0x3FUL;
    constexpr uint64_t lenBurstMask = 0x1FFFFFUL;
    constexpr uint64_t nBurstMask = 0xFFFUL;
    constexpr uint64_t gapMask = 0xFFFFFFFFUL;

    constexpr uint64_t rightPaddingNumShift = 54;
    constexpr uint64_t leftPaddingNumShift = 48;
    constexpr uint64_t lenBurstShift = 16;
    constexpr uint64_t nBurstShift = 4;
    constexpr uint64_t dstGapShift = 32;
    constexpr uint64_t srcGapShift = 0;
    return {
        ((record.rightPaddingNum & paddingNumMask) << rightPaddingNumShift) |
        ((record.leftPaddingNum & paddingNumMask) << leftPaddingNumShift) |
        ((record.lenBurst & lenBurstMask) << lenBurstShift) |
        ((record.nBurst & nBurstMask) << nBurstShift),

        ((record.srcGap & gapMask) << srcGapShift) |
        ((record.dstGap & gapMask) << dstGapShift)
    };
}

TEST(MovAlignInstructions, dump_copy_gm_to_ubuf_align_b8_expect_get_correct_records)
{
    auto record = CreateRandomMovAlignRecord<MemType::GM, MemType::UB, DataType::DATA_B8>();

    uint8_t sid = 0;
    std::vector<uint8_t> memInfo = CreateMemInfo();

    std::pair<uint64_t, uint64_t> config = ExtractConfigFromMovAlignRecord(record);
    __sanitizer_report_copy_gm_to_ubuf_align_b8(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                       config.first, config.second);

    __sanitizer_report_copy_gm_to_ubuf_align_b8(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                       sid, record.nBurst, record.lenBurst, record.leftPaddingNum,
                                       record.rightPaddingNum, record.srcGap, record.dstGap);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::MOV_ALIGN>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(MovAlignRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::MOV_ALIGN>(ptr, record));
}

TEST(MovAlignInstructions, dump_copy_gm_to_ubuf_align_b16_expect_get_correct_records)
{
    auto record = CreateRandomMovAlignRecord<MemType::GM, MemType::UB, DataType::DATA_B16>();

    uint8_t sid = 0;
    std::vector<uint8_t> memInfo = CreateMemInfo();

    std::pair<uint64_t, uint64_t> config = ExtractConfigFromMovAlignRecord(record);
    __sanitizer_report_copy_gm_to_ubuf_align_b16(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                       config.first, config.second);

    __sanitizer_report_copy_gm_to_ubuf_align_b16(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                       sid, record.nBurst, record.lenBurst, record.leftPaddingNum,
                                       record.rightPaddingNum, record.srcGap, record.dstGap);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::MOV_ALIGN>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(MovAlignRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::MOV_ALIGN>(ptr, record));
}

TEST(MovAlignInstructions, dump_copy_gm_to_ubuf_align_b32_expect_get_correct_records)
{
    auto record = CreateRandomMovAlignRecord<MemType::GM, MemType::UB, DataType::DATA_B32>();

    uint8_t sid = 0;
    std::vector<uint8_t> memInfo = CreateMemInfo();

    std::pair<uint64_t, uint64_t> config = ExtractConfigFromMovAlignRecord(record);
    __sanitizer_report_copy_gm_to_ubuf_align_b32(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                       config.first, config.second);

    __sanitizer_report_copy_gm_to_ubuf_align_b32(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                       sid, record.nBurst, record.lenBurst, record.leftPaddingNum,
                                       record.rightPaddingNum, record.srcGap, record.dstGap);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::MOV_ALIGN>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(MovAlignRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::MOV_ALIGN>(ptr, record));
}

TEST(MovAlignInstructions, dump_copy_ubuf_to_gm_align_b8_expect_get_correct_records)
{
    auto record = CreateRandomMovAlignRecord<MemType::UB, MemType::GM, DataType::DATA_B8>();

    uint8_t sid = 0;
    std::vector<uint8_t> memInfo = CreateMemInfo();

    std::pair<uint64_t, uint64_t> config = ExtractConfigFromMovAlignRecord(record);
    __sanitizer_report_copy_ubuf_to_gm_align_b8(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                       config.first, config.second);

    __sanitizer_report_copy_ubuf_to_gm_align_b8(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                       sid, record.nBurst, record.lenBurst, record.leftPaddingNum,
                                       record.rightPaddingNum, record.srcGap, record.dstGap);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::MOV_ALIGN>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(MovAlignRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::MOV_ALIGN>(ptr, record));
}

TEST(MovAlignInstructions, dump_copy_ubuf_to_gm_align_b16_expect_get_correct_records)
{
    auto record = CreateRandomMovAlignRecord<MemType::UB, MemType::GM, DataType::DATA_B16>();

    uint8_t sid = 0;
    std::vector<uint8_t> memInfo = CreateMemInfo();

    std::pair<uint64_t, uint64_t> config = ExtractConfigFromMovAlignRecord(record);
    __sanitizer_report_copy_ubuf_to_gm_align_b16(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                       config.first, config.second);

    __sanitizer_report_copy_ubuf_to_gm_align_b16(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                       sid, record.nBurst, record.lenBurst, record.leftPaddingNum,
                                       record.rightPaddingNum, record.srcGap, record.dstGap);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::MOV_ALIGN>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(MovAlignRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::MOV_ALIGN>(ptr, record));
}

TEST(MovAlignInstructions, dump_copy_ubuf_to_gm_align_b32_expect_get_correct_records)
{
    auto record = CreateRandomMovAlignRecord<MemType::UB, MemType::GM, DataType::DATA_B32>();

    uint8_t sid = 0;
    std::vector<uint8_t> memInfo = CreateMemInfo();

    std::pair<uint64_t, uint64_t> config = ExtractConfigFromMovAlignRecord(record);
    __sanitizer_report_copy_ubuf_to_gm_align_b32(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                       config.first, config.second);

    __sanitizer_report_copy_ubuf_to_gm_align_b32(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                       sid, record.nBurst, record.lenBurst, record.leftPaddingNum,
                                       record.rightPaddingNum, record.srcGap, record.dstGap);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::MOV_ALIGN>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(MovAlignRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::MOV_ALIGN>(ptr, record));
}
