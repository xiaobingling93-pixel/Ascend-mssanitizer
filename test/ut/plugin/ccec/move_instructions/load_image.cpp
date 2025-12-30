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
#include "plugin/ccec/move_instructions/load_image.cpp"

using namespace Sanitizer;

template<MemType dstMemType, DataType dataType>
LoadImageRecord CreateRandomLoadImageRecord()
{
    constexpr uint64_t randSize = 100;
    LoadImageRecord record{};
    record.dst = RandInt(0, 0x1000000);
    record.location.fileNo = RandInt(0, 0x100000);
    record.location.lineNo = RandInt(0, 0x1000);
    record.horSize = RandInt(1, randSize);
    record.verSize = RandInt(1, randSize);
    record.horStartP = RandInt(1, randSize);
    record.verStartP = RandInt(1, randSize);
    record.sHorRes = 1;
    record.lPadSize = RandInt(1, randSize);
    record.rPadSize = RandInt(1, randSize);
    record.topPadSize = RandInt(1, randSize);
    record.botPadSize = RandInt(1, randSize);
    record.dstMemType = dstMemType;
    record.dataType = dataType;
    return record;
}

std::pair<uint64_t, uint64_t> ExtractConfigFromLoadImageRecord(const LoadImageRecord &record)
{
    constexpr uint64_t horSizeMask = 0x1FFF;
    constexpr uint64_t verSizeMask = 0x1FFF;
    constexpr uint64_t horStartPMask = 0x1FFF;
    constexpr uint64_t verStartPMask = 0x1FFF;
    constexpr uint64_t sHorResMask = 0xFFFF;
    constexpr uint64_t topPadSizeMask = 0xFF;
    constexpr uint64_t botPadSizeMask = 0xFF;
    constexpr uint64_t lPadSizeMask = 0x1FFF;
    constexpr uint64_t rPadSizeMask = 0x1FFF;

    constexpr uint64_t verSizeShift = 16;
    constexpr uint64_t horStartPShift = 32;
    constexpr uint64_t verStartPShift = 48;
    constexpr uint64_t topPadSizeShift = 16;
    constexpr uint64_t botPadSizeShift = 24;
    constexpr uint64_t lPadSizeShift = 32;
    constexpr uint64_t rPadSizeShift = 45;

    return {
            (record.horSize & horSizeMask) |
            ((record.verSize & verSizeMask) << verSizeShift) |
            ((record.horStartP & horStartPMask) << horStartPShift) |
            ((record.verStartP & verStartPMask) << verStartPShift),
            (record.sHorRes & sHorResMask) |
            ((record.topPadSize & topPadSizeMask) << topPadSizeShift) |
            ((record.botPadSize & botPadSizeMask) << botPadSizeShift) |
            ((record.lPadSize & lPadSizeMask) << lPadSizeShift) |
            ((record.rPadSize & rPadSizeMask) << rPadSizeShift)
    };
}

TEST(LoadImageInstructions, load_image_f16)
{
    auto record = CreateRandomLoadImageRecord<MemType::L1, DataType::DATA_B16>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    auto config = ExtractConfigFromLoadImageRecord(record);

    // #1516
    __sanitizer_report_load_image_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                          record.location.pc,
                                          reinterpret_cast<__cbuf__ half*>(record.dst), config.first, config.second);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_IMAGE>(ptr, record));

    // #1517
    __sanitizer_report_load_image_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                          record.location.pc,
                                          reinterpret_cast<__cbuf__ half*>(record.dst), record.horSize, record.verSize,
                                          record.horStartP, record.verStartP, record.sHorRes, record.topPadSize,
                                          record.botPadSize, record.lPadSize, record.rPadSize, 0);
    ptr += sizeof(RecordType) + sizeof(LoadImageRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_IMAGE>(ptr, record));
}

TEST(LoadImageInstructions, load_image_s8)
{
    auto record = CreateRandomLoadImageRecord<MemType::L1, DataType::DATA_B8>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    auto config = ExtractConfigFromLoadImageRecord(record);

    // #1520
    __sanitizer_report_load_image_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                          record.location.pc,
                                          reinterpret_cast<__cbuf__ int8_t*>(record.dst), config.first, config.second);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_IMAGE>(ptr, record));

    // #1521
    __sanitizer_report_load_image_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                          record.location.pc,
                                          reinterpret_cast<__cbuf__ int8_t*>(record.dst), record.horSize,
                                          record.verSize, record.horStartP, record.verStartP, record.sHorRes,
                                          record.topPadSize, record.botPadSize, record.lPadSize, record.rPadSize, 0);
    ptr += sizeof(RecordType) + sizeof(LoadImageRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_IMAGE>(ptr, record));
}
