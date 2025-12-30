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
#include "plugin/ccec/move_instructions/load_2d.cpp"

using namespace Sanitizer;

template<MemType srcMemType, MemType dstMemType, AddrCalMode addrCalMode = AddrCalMode::INC>
Load2DRecord CreateLoad2DRecord()
{
    Load2DRecord record{};
    record.dst = RandInt(0, 0x1000000);
    record.src = RandInt(0, 0x1000000);
    record.baseIdx = RandInt(0, 0xFFF);
    record.repeat = RandInt(0, 0xFF);
    record.addrCalMode = addrCalMode;
    record.srcStride = RandInt(0, 0xFFF);
    record.dstStride = RandInt(0, 0xFFF);
    record.blockSize = 512;
    record.location.fileNo = RandInt(0, 0x100000);
    record.location.lineNo = RandInt(0, 0x1000);

    record.srcMemType = srcMemType;
    record.dstMemType = dstMemType;

    return record;
}

uint64_t ExtractLoad2DRecord(const Load2DRecord &record)
{
    constexpr uint64_t baseIdxMask = 0xFFFFUL;
    constexpr uint64_t repeatMask = 0xFFUL;
    constexpr uint64_t srcStrideMask = 0xFFFFUL;
    constexpr uint64_t dstStrideMask = 0xFFFFUL;

    constexpr uint64_t baseIdxShift = 0;
    constexpr uint64_t repeatShift = 16;
    constexpr uint64_t srcStrideShift = 24;
    constexpr uint64_t dstStrideShift = 44;

    return ((record.baseIdx & baseIdxMask) << baseIdxShift) |
        ((record.repeat & repeatMask) << repeatShift) |
        ((record.srcStride & srcStrideMask) << srcStrideShift) |
        ((record.dstStride & dstStrideMask) << dstStrideShift);
}

template<MemType srcMemType, MemType dstMemType>
Load2DSparseRecord CreateLoad2DSparseRecord()
{
    Load2DSparseRecord record{};
    record.dst = RandInt(0, 0x1000000);
    record.src0 = RandInt(0, 0x1000000) & 0xFFFFFFFF;
    record.src1 = RandInt(0, 0x1000000) & 0xFFFFFFFF;
    record.startId = RandInt(0, 0xFFF);
    record.repeat = RandInt(0, 0xFFF);
    record.location.fileNo = RandInt(0, 0x100000);
    record.location.lineNo = RandInt(0, 0x1000);

    record.srcMemType = srcMemType;
    record.dstMemType = dstMemType;

    return record;
}

uint64_t ExtractLoad2DSparseRecord(const Load2DSparseRecord &record)
{
    constexpr uint64_t startIdMask = 0xFFFFUL;
    constexpr uint64_t repeatMask = 0xFFUL;

    constexpr uint64_t startIdShift = 0;
    constexpr uint64_t repeatShift = 16;

    return ((record.startId & startIdMask) << startIdShift) |
        ((record.repeat & repeatMask) << repeatShift);
}

template<MemType srcMemType, MemType dstMemType, DataType dataType>
Load2DTransposeRecord CreateLoad2DTransposeRecord()
{
    Load2DTransposeRecord record{};
    record.dst = RandInt(0, 0x1000000);
    record.src = RandInt(0, 0x1000000);
    record.indexId = RandInt(0, 0xFFF);
    record.repeat = RandInt(0, 0xFFF);
    record.addrMode = RandInt(0, 0xFFF);
    record.srcStride = RandInt(0, 0xFFF);
    record.dstStride = RandInt(0, 0xFFF);
    record.dstFracStride = RandInt(0, 0xFFF);
    record.location.fileNo = RandInt(0, 0x100000);
    record.location.lineNo = RandInt(0, 0x1000);

    record.dataType = dataType;
    record.srcMemType = srcMemType;
    record.dstMemType = dstMemType;

    return record;
}

uint64_t ExtractLoad2DTransposeConfigRecord(const Load2DTransposeRecord &record)
{
    constexpr uint64_t indexIdMask = 0xFFFFUL;
    constexpr uint64_t repeatMask = 0xFFUL;
    constexpr uint64_t srcStrideMask = 0xFFFFUL;
    constexpr uint64_t dstStrideMask = 0xFFFFUL;
    constexpr uint64_t addrModeMask = 0x1UL;

    constexpr uint64_t indexIdShift = 0;
    constexpr uint64_t repeatShift = 16;
    constexpr uint64_t srcStrideShift = 24;
    constexpr uint64_t dstStrideShift = 44;
    constexpr uint64_t addrModeShift = 63;

    return ((record.indexId & indexIdMask) << indexIdShift) |
        ((record.repeat & repeatMask) << repeatShift) |
        ((record.srcStride & srcStrideMask) << srcStrideShift) |
        ((record.dstStride & dstStrideMask) << dstStrideShift) |
        ((record.addrMode & addrModeMask) << addrModeShift);
}

uint64_t ExtractLoad2DTransposeFracStrideRecord(const Load2DTransposeRecord &record)
{
    constexpr uint64_t dstFracStrideMask = 0xFFFFUL;
    constexpr uint64_t dstFracStrideShift = 0;

    return ((record.dstFracStride & dstFracStrideMask) << dstFracStrideShift);
}

TEST(Load2DInstructions, load_gm_to_cbuf_without_mode)
{
    auto record = CreateLoad2DRecord<MemType::GM, MemType::L1>();
    auto record0 = CreateLoad2DRecord<MemType::GM, MemType::L1>();
    record0.dstStride = 0;

    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoad2DRecord(record);
    // 1460
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ __bf16*>(record.dst), reinterpret_cast<__gm__ __bf16*>(record.src), config);
    // 1465
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ half *>(record.dst), reinterpret_cast<__gm__ half *>(record.src), config);
    // 1476
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ int8_t *>(record.dst), reinterpret_cast< __gm__ int8_t *>(record.src), config);
    // 1484
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint8_t *>(record.dst), reinterpret_cast<__gm__ uint8_t *>(record.src), config);
    // 1492
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record0.location.fileNo, record0.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cb__ half *>(record0.dst), reinterpret_cast<__gm__ half *>(record0.src),
        record0.baseIdx, record0.repeat, record0.srcStride, 0);
    // 1498
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record0.location.fileNo, record0.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cb__ int8_t *>(record0.dst), reinterpret_cast<__gm__ int8_t *>(record0.src),
        record0.baseIdx, record0.repeat, record0.srcStride, 0);
    // 1504
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record0.location.fileNo, record0.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cb__ uint8_t *>(record0.dst), reinterpret_cast<__gm__ uint8_t *>(record0.src),
        record0.baseIdx, record0.repeat, record0.srcStride, 0);

    record.dstStride += 1U;
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record0));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record0));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record0));
}

TEST(Load2DInstructions, load_gm_to_cbuf_with_inc_mode)
{
    auto record = CreateLoad2DRecord<MemType::GM, MemType::L1, AddrCalMode::INC>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoad2DRecord(record);

    // 1461
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ __bf16 *>(record.dst),
        reinterpret_cast<__gm__ __bf16 *>(record.src), config, AddrCalMode::INC);
    // 1466
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ half *>(record.dst),
        reinterpret_cast<__gm__ half *>(record.src), config, AddrCalMode::INC);
    // 1470
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ float *>(record.dst),
        reinterpret_cast<__gm__ float *>(record.src), config, AddrCalMode::INC);
    // 1473
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ int32_t *>(record.dst),
        reinterpret_cast<__gm__ int32_t *>(record.src), config, AddrCalMode::INC);
    // 1477
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ int8_t *>(record.dst), reinterpret_cast<__gm__ int8_t *>(record.src),
        config, AddrCalMode::INC);
    // 1481
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint32_t *>(record.dst), reinterpret_cast<__gm__ uint32_t *>(record.src),
        config, AddrCalMode::INC);
    // 1485
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint8_t *>(record.dst), reinterpret_cast<__gm__ uint8_t *>(record.src),
        config, AddrCalMode::INC);
    // 1512
    __sanitizer_report_load_gm_to_cbuf_s4(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cb__ void *>(record.dst), reinterpret_cast<__gm__ void *>(record.src),
        config, AddrCalMode::INC);
    record.dstStride += 1U;


    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
}

TEST(Load2DInstructions, Load2DInstructions_load_gm_to_cbuf_with_inc_mode_individual_param)
{
    auto record = CreateLoad2DRecord<MemType::GM, MemType::L1, AddrCalMode::INC>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    auto record0 = CreateLoad2DRecord<MemType::GM, MemType::L1, AddrCalMode::INC>();
    record0.dstStride = 0;
    // 1491
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<__cb__ half *>(record.dst),
                                       reinterpret_cast<__gm__ half *>(record.src), record.baseIdx, record.repeat,
                                       record.srcStride, record.dstStride, 0, AddrCalMode::INC);
    // 1497
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<__cb__ int8_t *>(record.dst),
                                       reinterpret_cast<__gm__ int8_t *>(record.src), record.baseIdx, record.repeat,
                                       record.srcStride, record.dstStride, 0, AddrCalMode::INC);
    // 1503
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<__cb__ uint8_t *>(record.dst),
                                       reinterpret_cast<__gm__ uint8_t *>(record.src), record.baseIdx, record.repeat,
                                       record.srcStride, record.dstStride, 0, AddrCalMode::INC);
    // 1513
    __sanitizer_report_load_gm_to_cbuf_s4(memInfo.data(), record0.location.fileNo, record0.location.lineNo,
                                          record.location.pc,
                                          reinterpret_cast<__cb__ void *>(record0.dst),
                                          reinterpret_cast<__gm__ void *>(record0.src), record0.baseIdx, record0.repeat,
                                          record0.srcStride, 0, AddrCalMode::INC);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record0));
}

TEST(Load2DInstructions, load_gm_to_cbuf_with_dec_mode)
{
    auto record = CreateLoad2DRecord<MemType::GM, MemType::L1, AddrCalMode::DEC>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoad2DRecord(record);

    // 1461
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ __bf16 *>(record.dst),
        reinterpret_cast<__gm__ __bf16 *>(record.src), config, AddrCalMode::DEC);
    // 1466
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ half *>(record.dst),
        reinterpret_cast<__gm__ half *>(record.src), config, AddrCalMode::DEC);
    // 1470
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ float *>(record.dst),
        reinterpret_cast<__gm__ float *>(record.src), config, AddrCalMode::DEC);
    // 1473
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ int32_t *>(record.dst),
        reinterpret_cast<__gm__ int32_t *>(record.src), config, AddrCalMode::DEC);
    // 1477
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ int8_t *>(record.dst), reinterpret_cast<__gm__ int8_t *>(record.src),
        config, AddrCalMode::DEC);
    // 1481
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint32_t *>(record.dst), reinterpret_cast<__gm__ uint32_t *>(record.src),
        config, AddrCalMode::DEC);
    // 1485
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint8_t *>(record.dst), reinterpret_cast<__gm__ uint8_t *>(record.src),
        config, AddrCalMode::DEC);
    // 1512
    __sanitizer_report_load_gm_to_cbuf_s4(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cb__ void *>(record.dst), reinterpret_cast<__gm__ void *>(record.src),
        config, AddrCalMode::DEC);
    record.dstStride += 1U;

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
}

TEST(Load2DInstructions, Load2DInstructions_load_gm_to_cbuf_with_dec_mode_individual_param)
{
    auto record = CreateLoad2DRecord<MemType::GM, MemType::L1, AddrCalMode::DEC>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    auto record0 = CreateLoad2DRecord<MemType::GM, MemType::L1, AddrCalMode::DEC>();
    record0.dstStride = 0;
    // 1491
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<__cb__ half *>(record.dst),
                                       reinterpret_cast<__gm__ half *>(record.src), record.baseIdx, record.repeat,
                                       record.srcStride, record.dstStride, 0, AddrCalMode::DEC);
    // 1497
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<__cb__ int8_t *>(record.dst),
                                       reinterpret_cast<__gm__ int8_t *>(record.src), record.baseIdx, record.repeat,
                                       record.srcStride, record.dstStride, 0, AddrCalMode::DEC);
    // 1503
    __sanitizer_report_load_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<__cb__ uint8_t *>(record.dst),
                                       reinterpret_cast<__gm__ uint8_t *>(record.src), record.baseIdx, record.repeat,
                                       record.srcStride, record.dstStride, 0, AddrCalMode::DEC);
    // 1513
    __sanitizer_report_load_gm_to_cbuf_s4(memInfo.data(), record0.location.fileNo, record0.location.lineNo,
                                          record.location.pc,
                                          reinterpret_cast<__cb__ void *>(record0.dst),
                                          reinterpret_cast<__gm__ void *>(record0.src), record0.baseIdx, record0.repeat,
                                          record0.srcStride, 0, AddrCalMode::DEC);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record0));
}

// load_gm_to_ca
TEST(Load2DInstructions, load_gm_to_ca_without_mode)
{
    auto record = CreateLoad2DRecord<MemType::GM, MemType::L0A>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoad2DRecord(record);
    // 1316
    __sanitizer_report_load_gm_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ __bf16*>(record.dst), reinterpret_cast<__gm__ __bf16*>(record.src),
        config);
    // 1322
    __sanitizer_report_load_gm_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ half *>(record.dst), reinterpret_cast<__gm__ half *>(record.src),
        config);
    // 1332
    __sanitizer_report_load_gm_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ int8_t *>(record.dst), reinterpret_cast< __gm__ int8_t *>(record.src),
        config);
    // 1340
    __sanitizer_report_load_gm_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint8_t *>(record.dst), reinterpret_cast<__gm__ uint8_t *>(record.src),
        config);

    record.dstStride += 1U;
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
}

TEST(Load2DInstructions, load_gm_to_ca_with_inc_mode)
{
    auto record = CreateLoad2DRecord<MemType::GM, MemType::L0A, AddrCalMode::INC>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoad2DRecord(record);
    // 1317
    __sanitizer_report_load_gm_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ __bf16 *>(record.dst),
        reinterpret_cast<__gm__ __bf16 *>(record.src), config, AddrCalMode::INC);
    // 1322
    __sanitizer_report_load_gm_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ half *>(record.dst),
        reinterpret_cast<__gm__ half *>(record.src), config, AddrCalMode::INC);
    // 1326
    __sanitizer_report_load_gm_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ float *>(record.dst),
        reinterpret_cast<__gm__ float *>(record.src), config, AddrCalMode::INC);
    // 1329
    __sanitizer_report_load_gm_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ int32_t *>(record.dst),
        reinterpret_cast<__gm__ int32_t *>(record.src), config, AddrCalMode::INC);
    // 1333
    __sanitizer_report_load_gm_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ int8_t *>(record.dst), reinterpret_cast<__gm__ int8_t *>(record.src),
        config, AddrCalMode::INC);
    // 1337
    __sanitizer_report_load_gm_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint32_t *>(record.dst), reinterpret_cast<__gm__ uint32_t *>(record.src),
        config, AddrCalMode::INC);
    // 1341
    __sanitizer_report_load_gm_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint8_t *>(record.dst), reinterpret_cast<__gm__ uint8_t *>(record.src),
        config, AddrCalMode::INC);
    // 1368
    __sanitizer_report_load_gm_to_ca_s4(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__ca__ void *>(record.dst), reinterpret_cast<__gm__ void *>(record.src),
        config, AddrCalMode::INC);

    record.dstStride += 1U;
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
}

TEST(Load2DInstructions, load_gm_to_ca_with_dec_mode)
{
    auto record = CreateLoad2DRecord<MemType::GM, MemType::L0A, AddrCalMode::DEC>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoad2DRecord(record);
    // 1317
    __sanitizer_report_load_gm_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ __bf16 *>(record.dst),
        reinterpret_cast<__gm__ __bf16 *>(record.src), config, AddrCalMode::DEC);
    // 1322
    __sanitizer_report_load_gm_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ half *>(record.dst),
        reinterpret_cast<__gm__ half *>(record.src), config, AddrCalMode::DEC);
    // 1326
    __sanitizer_report_load_gm_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ float *>(record.dst),
        reinterpret_cast<__gm__ float *>(record.src), config, AddrCalMode::DEC);
    // 1329
    __sanitizer_report_load_gm_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ int32_t *>(record.dst),
        reinterpret_cast<__gm__ int32_t *>(record.src), config, AddrCalMode::DEC);
    // 1333
    __sanitizer_report_load_gm_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ int8_t *>(record.dst), reinterpret_cast<__gm__ int8_t *>(record.src),
        config, AddrCalMode::DEC);
    // 1337
    __sanitizer_report_load_gm_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint32_t *>(record.dst), reinterpret_cast<__gm__ uint32_t *>(record.src),
        config, AddrCalMode::DEC);
    // 1341
    __sanitizer_report_load_gm_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint8_t *>(record.dst), reinterpret_cast<__gm__ uint8_t *>(record.src),
        config, AddrCalMode::DEC);
    // 1368
    __sanitizer_report_load_gm_to_ca_s4(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__ca__ void *>(record.dst), reinterpret_cast<__gm__ void *>(record.src),
        config, AddrCalMode::DEC);
    record.dstStride += 1U;
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
}

// load_gm_to_cb
TEST(Load2DInstructions, load_gm_to_cb_without_mode)
{
    auto record = CreateLoad2DRecord<MemType::GM, MemType::L0B>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoad2DRecord(record);
    // 1388
    __sanitizer_report_load_gm_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ __bf16*>(record.dst), reinterpret_cast<__gm__ __bf16*>(record.src),
        config);
    // 1393
    __sanitizer_report_load_gm_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ half *>(record.dst), reinterpret_cast<__gm__ half *>(record.src), config);
    // 1404
    __sanitizer_report_load_gm_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ int8_t *>(record.dst), reinterpret_cast< __gm__ int8_t *>(record.src), config);
    // 1412
    __sanitizer_report_load_gm_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint8_t *>(record.dst), reinterpret_cast<__gm__ uint8_t *>(record.src), config);

    record.dstStride += 1U;
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
}

TEST(Load2DInstructions, load_gm_to_cb_with_inc_mode)
{
    auto record = CreateLoad2DRecord<MemType::GM, MemType::L0B, AddrCalMode::INC>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoad2DRecord(record);
    // 1389
    __sanitizer_report_load_gm_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ __bf16 *>(record.dst),
        reinterpret_cast<__gm__ __bf16 *>(record.src), config, AddrCalMode::INC);
    // 1394
    __sanitizer_report_load_gm_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ half *>(record.dst),
        reinterpret_cast<__gm__ half *>(record.src), config, AddrCalMode::INC);
    // 1398
    __sanitizer_report_load_gm_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ float *>(record.dst),
        reinterpret_cast<__gm__ float *>(record.src), config, AddrCalMode::INC);
    // 1401
    __sanitizer_report_load_gm_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ int32_t *>(record.dst),
        reinterpret_cast<__gm__ int32_t *>(record.src), config, AddrCalMode::INC);
    // 1405
    __sanitizer_report_load_gm_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ int8_t *>(record.dst), reinterpret_cast<__gm__ int8_t *>(record.src),
        config, AddrCalMode::INC);
    // 1409
    __sanitizer_report_load_gm_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint32_t *>(record.dst), reinterpret_cast<__gm__ uint32_t *>(record.src),
        config, AddrCalMode::INC);
    // 1413
    __sanitizer_report_load_gm_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint8_t *>(record.dst), reinterpret_cast<__gm__ uint8_t *>(record.src),
        config, AddrCalMode::INC);
    // 1440
    __sanitizer_report_load_gm_to_cb_s4(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cb__ void *>(record.dst), reinterpret_cast<__gm__ void *>(record.src),
        config, AddrCalMode::INC);
    record.dstStride += 1U;
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
}

TEST(Load2DInstructions, load_gm_to_cb_with_dec_mode)
{
    auto record = CreateLoad2DRecord<MemType::GM, MemType::L0B, AddrCalMode::DEC>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoad2DRecord(record);
    // 1389
    __sanitizer_report_load_gm_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ __bf16 *>(record.dst),
        reinterpret_cast<__gm__ __bf16 *>(record.src), config, AddrCalMode::DEC);
    // 1394
    __sanitizer_report_load_gm_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ half *>(record.dst),
        reinterpret_cast<__gm__ half *>(record.src), config, AddrCalMode::DEC);
    // 1398
    __sanitizer_report_load_gm_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ float *>(record.dst),
        reinterpret_cast<__gm__ float *>(record.src), config, AddrCalMode::DEC);
    // 1401
    __sanitizer_report_load_gm_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ int32_t *>(record.dst),
        reinterpret_cast<__gm__ int32_t *>(record.src), config, AddrCalMode::DEC);
    // 1405
    __sanitizer_report_load_gm_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ int8_t *>(record.dst), reinterpret_cast<__gm__ int8_t *>(record.src),
        config, AddrCalMode::DEC);
    // 1409
    __sanitizer_report_load_gm_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint32_t *>(record.dst), reinterpret_cast<__gm__ uint32_t *>(record.src),
        config, AddrCalMode::DEC);
    // 1413
    __sanitizer_report_load_gm_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint8_t *>(record.dst), reinterpret_cast<__gm__ uint8_t *>(record.src),
        config, AddrCalMode::DEC);
    // 1440
    __sanitizer_report_load_gm_to_cb_s4(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cb__ void *>(record.dst), reinterpret_cast<__gm__ void *>(record.src),
        config, AddrCalMode::DEC);
    record.dstStride += 1U;
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
}

// load_cbuf_to_ca
TEST(Load2DInstructions, load_cbuf_to_ca_without_mode)
{
    auto record = CreateLoad2DRecord<MemType::L1, MemType::L0A>();
    auto record0 = CreateLoad2DRecord<MemType::L1, MemType::L0A>();
    record0.dstStride = 0;

    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoad2DRecord(record);
    // 1106
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ __bf16*>(record.dst), reinterpret_cast<__gm__ __bf16*>(record.src), config, true);
    // 1111
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ half *>(record.dst), reinterpret_cast<__gm__ half *>(record.src),
        config, true);
    // 1122
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ int8_t *>(record.dst), reinterpret_cast< __gm__ int8_t *>(record.src), config, true);
    // 1130
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint8_t *>(record.dst), reinterpret_cast<__gm__ uint8_t *>(record.src),
        config, true);
    // 1138
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record0.location.fileNo, record0.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ half *>(record0.dst), reinterpret_cast<__gm__ half *>(record0.src),
        record0.baseIdx, record0.repeat, record0.srcStride, 0, true);
    // 1144
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record0.location.fileNo, record0.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ int8_t *>(record0.dst), reinterpret_cast< __gm__ int8_t *>(record0.src),
        record0.baseIdx, record0.repeat, record0.srcStride, 0, true);
    // 1150
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record0.location.fileNo, record0.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint8_t *>(record0.dst), reinterpret_cast<__gm__ uint8_t *>(record0.src),
        record0.baseIdx, record0.repeat, record0.srcStride, 0, true);

    record.dstStride += 1U;
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record0));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record0));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record0));
}

TEST(Load2DInstructions, load_cbuf_to_ca_with_inc_mode)
{
    auto record = CreateLoad2DRecord<MemType::L1, MemType::L0A, AddrCalMode::INC>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoad2DRecord(record);
    // 1107
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ __bf16 *>(record.dst), reinterpret_cast<__gm__ __bf16 *>(record.src),
        config, true, AddrCalMode::INC);
    // 1112
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ half *>(record.dst), reinterpret_cast<__gm__ half *>(record.src),
        config, true, AddrCalMode::INC);
    // 1116
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ float *>(record.dst), reinterpret_cast<__gm__ float *>(record.src),
        config, true, AddrCalMode::INC);
    // 1119
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ int32_t *>(record.dst), reinterpret_cast<__gm__ int32_t *>(record.src),
        config, true, AddrCalMode::INC);
    // 1123
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ int8_t *>(record.dst), reinterpret_cast<__gm__ int8_t *>(record.src),
        config, true, AddrCalMode::INC);
    // 1127
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint32_t *>(record.dst), reinterpret_cast<__gm__ uint32_t *>(record.src),
        config, true, AddrCalMode::INC);
    // 1131
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint8_t *>(record.dst), reinterpret_cast<__gm__ uint8_t *>(record.src),
        config, true, AddrCalMode::INC);
    record.dstStride += 1U;

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
}


TEST(Load2DInstructions, load_cbuf_to_ca_with_inc_mode_individual_param)
{
    auto record = CreateLoad2DRecord<MemType::L1, MemType::L0A, AddrCalMode::INC>();

    std::vector<uint8_t> memInfo = CreateMemInfo();
    // 1138
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<__cbuf__ half *>(record.dst),
                                       reinterpret_cast<__gm__ half *>(record.src), record.baseIdx, record.repeat,
                                       record.srcStride, record.dstStride, 0, true, AddrCalMode::INC);
    // 1144
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<__cbuf__ int8_t *>(record.dst),
                                       reinterpret_cast<__gm__ int8_t *>(record.src), record.baseIdx, record.repeat,
                                       record.srcStride, record.dstStride, 0, true, AddrCalMode::INC);
    // 1150
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<__cbuf__ uint8_t *>(record.dst),
                                       reinterpret_cast<__gm__ uint8_t *>(record.src), record.baseIdx, record.repeat,
                                       record.srcStride, record.dstStride, 0, true, AddrCalMode::INC);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
}

TEST(Load2DInstructions, load_cbuf_to_ca_with_dec_mode)
{
    auto record = CreateLoad2DRecord<MemType::L1, MemType::L0A, AddrCalMode::DEC>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoad2DRecord(record);
    // 1107
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ __bf16 *>(record.dst), reinterpret_cast<__gm__ __bf16 *>(record.src),
        config, true, AddrCalMode::DEC);
    // 1112
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ half *>(record.dst), reinterpret_cast<__gm__ half *>(record.src),
        config, true, AddrCalMode::DEC);
    // 1116
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ float *>(record.dst), reinterpret_cast<__gm__ float *>(record.src),
        config, true, AddrCalMode::DEC);
    // 1119
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ int32_t *>(record.dst), reinterpret_cast<__gm__ int32_t *>(record.src),
        config, true, AddrCalMode::DEC);
    // 1123
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ int8_t *>(record.dst), reinterpret_cast<__gm__ int8_t *>(record.src),
        config, true, AddrCalMode::DEC);
    // 1127
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint32_t *>(record.dst), reinterpret_cast<__gm__ uint32_t *>(record.src),
        config, true, AddrCalMode::DEC);
    // 1131
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint8_t *>(record.dst), reinterpret_cast<__gm__ uint8_t *>(record.src),
        config, true, AddrCalMode::DEC);
    record.dstStride += 1U;


    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
}


TEST(Load2DInstructions, load_cbuf_to_ca_with_dec_mode_inidividual_param)
{
    auto record = CreateLoad2DRecord<MemType::L1, MemType::L0A, AddrCalMode::DEC>();

    std::vector<uint8_t> memInfo = CreateMemInfo();
    // 1138
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<__cbuf__ half *>(record.dst),
                                       reinterpret_cast<__gm__ half *>(record.src), record.baseIdx, record.repeat,
                                       record.srcStride, record.dstStride, 0, true, AddrCalMode::DEC);
    // 1144
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<__cbuf__ int8_t *>(record.dst),
                                       reinterpret_cast<__gm__ int8_t *>(record.src), record.baseIdx, record.repeat,
                                       record.srcStride, record.dstStride, 0, true, AddrCalMode::DEC);
    // 1150
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<__cbuf__ uint8_t *>(record.dst),
                                       reinterpret_cast<__gm__ uint8_t *>(record.src), record.baseIdx, record.repeat,
                                       record.srcStride, record.dstStride, 0, true, AddrCalMode::DEC);
    // 1171
    __sanitizer_report_load_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<__cbuf__ uint8_t *>(record.dst),
                                       reinterpret_cast<__gm__ uint8_t *>(record.src), record.baseIdx, record.repeat,
                                       record.srcStride, record.dstStride, 0, true, AddrCalMode::DEC);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
}

// load_cbuf_to_cb
TEST(Load2DInstructions, load_cbuf_to_cb_without_mode)
{
    auto record = CreateLoad2DRecord<MemType::L1, MemType::L0B>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoad2DRecord(record);
    // 1192
    __sanitizer_report_load_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<__cbuf__ __bf16 *>(record.dst),
                                       reinterpret_cast<__gm__ __bf16 *>(record.src), config, true);
    // 1197
    __sanitizer_report_load_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<__cbuf__ half *>(record.dst),
                                       reinterpret_cast<__gm__ half *>(record.src), config, true);
    // 1208
    __sanitizer_report_load_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<__cbuf__ int8_t *>(record.dst),
                                       reinterpret_cast<__gm__ int8_t *>(record.src), config, true);
    // 1216
    __sanitizer_report_load_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<__cbuf__ uint8_t *>(record.dst),
                                       reinterpret_cast<__gm__ uint8_t *>(record.src), config, true);

    record.dstStride += 1U;
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
}

TEST(Load2DInstructions, load_cbuf_to_cb_with_inc_mode)
{
    auto record = CreateLoad2DRecord<MemType::L1, MemType::L0B, AddrCalMode::INC>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoad2DRecord(record);
    // 1193
    __sanitizer_report_load_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ __bf16 *>(record.dst),
        reinterpret_cast<__gm__ __bf16 *>(record.src), config, true, AddrCalMode::INC);
    // 1198
    __sanitizer_report_load_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ half *>(record.dst),
        reinterpret_cast<__gm__ half *>(record.src), config, true, AddrCalMode::INC);
    // 1202
    __sanitizer_report_load_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ float *>(record.dst),
        reinterpret_cast<__gm__ float *>(record.src), config, true, AddrCalMode::INC);
    // 1205
    __sanitizer_report_load_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ int32_t *>(record.dst),
        reinterpret_cast<__gm__ int32_t *>(record.src), config, true, AddrCalMode::INC);
    // 1209
    __sanitizer_report_load_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ int8_t *>(record.dst), reinterpret_cast<__gm__ int8_t *>(record.src),
        config, true, AddrCalMode::INC);
    // 1213
    __sanitizer_report_load_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint32_t *>(record.dst), reinterpret_cast<__gm__ uint32_t *>(record.src),
        config, true, AddrCalMode::INC);
    // 1217
    __sanitizer_report_load_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint8_t *>(record.dst), reinterpret_cast<__gm__ uint8_t *>(record.src),
        config, true, AddrCalMode::INC);
    // 1270
    __sanitizer_report_load_cbuf_to_cb_s4(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint8_t *>(record.dst), reinterpret_cast<__gm__ uint8_t *>(record.src),
        config, true, AddrCalMode::INC);
    record.dstStride += 1U;
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
}

TEST(Load2DInstructions, load_cbuf_to_cb_with_dec_mode)
{
    auto record = CreateLoad2DRecord<MemType::L1, MemType::L0B, AddrCalMode::DEC>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoad2DRecord(record);
    // 1193
    __sanitizer_report_load_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ __bf16 *>(record.dst),
        reinterpret_cast<__gm__ __bf16 *>(record.src), config, true, AddrCalMode::DEC);
    // 1198
    __sanitizer_report_load_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ half *>(record.dst),
        reinterpret_cast<__gm__ half *>(record.src), config, true, AddrCalMode::DEC);
    // 1202
    __sanitizer_report_load_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ float *>(record.dst),
        reinterpret_cast<__gm__ float *>(record.src), config, true, AddrCalMode::DEC);
    // 1205
    __sanitizer_report_load_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ int32_t *>(record.dst),
        reinterpret_cast<__gm__ int32_t *>(record.src), config, true, AddrCalMode::DEC);
    // 1209
    __sanitizer_report_load_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ int8_t *>(record.dst), reinterpret_cast<__gm__ int8_t *>(record.src),
        config, true, AddrCalMode::DEC);
    // 1213
    __sanitizer_report_load_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint32_t *>(record.dst), reinterpret_cast<__gm__ uint32_t *>(record.src),
        config, true, AddrCalMode::DEC);
    // 1217
    __sanitizer_report_load_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint8_t *>(record.dst), reinterpret_cast<__gm__ uint8_t *>(record.src),
        config, true, AddrCalMode::DEC);
    // 1270
    __sanitizer_report_load_cbuf_to_cb_s4(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cbuf__ uint8_t *>(record.dst), reinterpret_cast<__gm__ uint8_t *>(record.src),
        config, true, AddrCalMode::DEC);
    record.dstStride += 1U;
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D>(ptr, record));
}

// load_cbuf_to_cb_sp
TEST(Load2DSparseInstructions, load_cbuf_to_cb_sp)
{
    auto record = CreateLoad2DSparseRecord<MemType::L1, MemType::L0B>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoad2DSparseRecord(record);
    // #1272
    __sanitizer_report_load_cbuf_to_cb_sp(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cb__ int8_t *>(record.dst),
        reinterpret_cast<__cbuf__ int8_t *>((static_cast<uint64_t>(record.src1) << 32) | record.src0), config);
    // #1273
    __sanitizer_report_load_cbuf_to_cb_sp(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cb__ int8_t *>(record.dst),
        reinterpret_cast<__cbuf__ int8_t *>((static_cast<uint64_t>(record.src1) << 32) | record.src0),
        record.startId, record.repeat);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_SPARSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DSparseRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_SPARSE>(ptr, record));
}

// load_cbuf_to_cb_transpose_s4
TEST(Load2DTransposeInstructions, load_cbuf_to_cb_transpose_b4)
{
    auto record = CreateLoad2DTransposeRecord<MemType::L1, MemType::L0B, DataType::DATA_B4>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoad2DTransposeConfigRecord(record);
    uint64_t fracStride = ExtractLoad2DTransposeFracStrideRecord(record);

    // #1288
    __sanitizer_report_load_cbuf_to_cb_transpose_s4(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ void *>(record.dst),
        reinterpret_cast<__cb__ void *>(record.src), config, fracStride);

    // #1289
    __sanitizer_report_load_cbuf_to_cb_transpose_s4(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ void *>(record.dst),
        reinterpret_cast<__cb__ void *>(record.src), record.indexId, record.repeat,
        record.srcStride, record.dstStride, record.addrMode, record.dstFracStride);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DTransposeRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
}

TEST(Load2DTransposeInstructions, load_cbuf_to_cb_transpose_b16)
{
    auto record = CreateLoad2DTransposeRecord<MemType::L1, MemType::L0B, DataType::DATA_B16>();
    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoad2DTransposeConfigRecord(record);
    uint64_t fracStride = ExtractLoad2DTransposeFracStrideRecord(record);
    
    __sanitizer_report_load_cbuf_to_cb_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ __bf16 *>(record.dst),
        reinterpret_cast<__cbuf__ __bf16 *>(record.src), config, fracStride);

    __sanitizer_report_load_cbuf_to_cb_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ __bf16 *>(record.dst),
        reinterpret_cast<__cbuf__ __bf16 *>(record.src), record.indexId, record.repeat,
        record.srcStride, record.dstStride, record.addrMode, record.dstFracStride);

    __sanitizer_report_load_cbuf_to_cb_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ half *>(record.dst),
        reinterpret_cast<__cbuf__ half *>(record.src), config, fracStride);

    __sanitizer_report_load_cbuf_to_cb_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ half *>(record.dst),
        reinterpret_cast<__cbuf__ half *>(record.src), record.indexId, record.repeat,
        record.srcStride, record.dstStride, record.addrMode, record.dstFracStride);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DTransposeRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DTransposeRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DTransposeRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
}

TEST(Load2DTransposeInstructions, load_cbuf_to_cb_transpose_b8)
{
    auto record = CreateLoad2DTransposeRecord<MemType::L1, MemType::L0B, DataType::DATA_B8>();
    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoad2DTransposeConfigRecord(record);
    uint64_t fracStride = ExtractLoad2DTransposeFracStrideRecord(record);

    __sanitizer_report_load_cbuf_to_cb_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ int8_t *>(record.dst),
        reinterpret_cast<__cbuf__ int8_t *>(record.src), config, fracStride);

    __sanitizer_report_load_cbuf_to_cb_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ int8_t *>(record.dst),
        reinterpret_cast<__cbuf__ int8_t *>(record.src), record.indexId, record.repeat,
        record.srcStride, record.dstStride, record.addrMode, record.dstFracStride);

    __sanitizer_report_load_cbuf_to_cb_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ uint8_t *>(record.dst),
        reinterpret_cast<__cbuf__ uint8_t *>(record.src), config, fracStride);

    __sanitizer_report_load_cbuf_to_cb_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ uint8_t *>(record.dst),
        reinterpret_cast<__cbuf__ uint8_t *>(record.src), record.indexId, record.repeat,
        record.srcStride, record.dstStride, record.addrMode, record.dstFracStride);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DTransposeRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DTransposeRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DTransposeRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
}

TEST(Load2DTransposeInstructions, load_cbuf_to_cb_transpose_b32)
{
    auto record = CreateLoad2DTransposeRecord<MemType::L1, MemType::L0B, DataType::DATA_B32>();
    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoad2DTransposeConfigRecord(record);
    uint64_t fracStride = ExtractLoad2DTransposeFracStrideRecord(record);

    __sanitizer_report_load_cbuf_to_cb_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ float *>(record.dst),
        reinterpret_cast<__cbuf__ float *>(record.src), config, fracStride);

    __sanitizer_report_load_cbuf_to_cb_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ float *>(record.dst),
        reinterpret_cast<__cbuf__ float *>(record.src), record.indexId, record.repeat,
        record.srcStride, record.dstStride, record.addrMode, record.dstFracStride);

    __sanitizer_report_load_cbuf_to_cb_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ int32_t *>(record.dst),
        reinterpret_cast<__cbuf__ int32_t *>(record.src), config, fracStride);

    __sanitizer_report_load_cbuf_to_cb_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ int32_t *>(record.dst),
        reinterpret_cast<__cbuf__ int32_t *>(record.src), record.indexId, record.repeat,
        record.srcStride, record.dstStride, record.addrMode, record.dstFracStride);

    __sanitizer_report_load_cbuf_to_cb_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ uint32_t *>(record.dst),
        reinterpret_cast<__cbuf__ uint32_t *>(record.src), config, fracStride);

    __sanitizer_report_load_cbuf_to_cb_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ uint32_t *>(record.dst),
        reinterpret_cast<__cbuf__ uint32_t *>(record.src), record.indexId, record.repeat,
        record.srcStride, record.dstStride, record.addrMode, record.dstFracStride);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DTransposeRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DTransposeRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DTransposeRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DTransposeRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DTransposeRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
}

TEST(Load2DTransposeInstructions, load_cbuf_to_ca_transpose_b16)
{
    auto record = CreateLoad2DTransposeRecord<MemType::L1, MemType::L0A, DataType::DATA_B16>();
    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoad2DTransposeConfigRecord(record);
    uint64_t fracStride = ExtractLoad2DTransposeFracStrideRecord(record);
    
    __sanitizer_report_load_cbuf_to_ca_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ __bf16 *>(record.dst),
        reinterpret_cast<__cbuf__ __bf16 *>(record.src), config, fracStride);

    __sanitizer_report_load_cbuf_to_ca_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ __bf16 *>(record.dst),
        reinterpret_cast<__cbuf__ __bf16 *>(record.src), record.indexId, record.repeat,
        record.srcStride, record.dstStride, record.addrMode, record.dstFracStride);

    __sanitizer_report_load_cbuf_to_ca_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ half *>(record.dst),
        reinterpret_cast<__cbuf__ half *>(record.src), config, fracStride);

    __sanitizer_report_load_cbuf_to_ca_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ half *>(record.dst),
        reinterpret_cast<__cbuf__ half *>(record.src), record.indexId, record.repeat,
        record.srcStride, record.dstStride, record.addrMode, record.dstFracStride);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DTransposeRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DTransposeRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DTransposeRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
}

TEST(Load2DTransposeInstructions, load_cbuf_to_ca_transpose_b8)
{
    auto record = CreateLoad2DTransposeRecord<MemType::L1, MemType::L0A, DataType::DATA_B8>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoad2DTransposeConfigRecord(record);
    uint64_t fracStride = ExtractLoad2DTransposeFracStrideRecord(record);

    __sanitizer_report_load_cbuf_to_ca_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ int8_t *>(record.dst),
        reinterpret_cast<__cbuf__ int8_t *>(record.src), config, fracStride);

    __sanitizer_report_load_cbuf_to_ca_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ int8_t *>(record.dst),
        reinterpret_cast<__cbuf__ int8_t *>(record.src), record.indexId, record.repeat,
        record.srcStride, record.dstStride, record.addrMode, record.dstFracStride);

    __sanitizer_report_load_cbuf_to_ca_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ uint8_t *>(record.dst),
        reinterpret_cast<__cbuf__ uint8_t *>(record.src), config, fracStride);

    __sanitizer_report_load_cbuf_to_ca_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ uint8_t *>(record.dst),
        reinterpret_cast<__cbuf__ uint8_t *>(record.src), record.indexId, record.repeat,
        record.srcStride, record.dstStride, record.addrMode, record.dstFracStride);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DTransposeRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DTransposeRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DTransposeRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
}

TEST(Load2DTransposeInstructions, load_cbuf_to_ca_transpose_b32)
{
    auto record = CreateLoad2DTransposeRecord<MemType::L1, MemType::L0A, DataType::DATA_B32>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractLoad2DTransposeConfigRecord(record);
    uint64_t fracStride = ExtractLoad2DTransposeFracStrideRecord(record);

    __sanitizer_report_load_cbuf_to_ca_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ float *>(record.dst),
        reinterpret_cast<__cbuf__ float *>(record.src), config, fracStride);

    __sanitizer_report_load_cbuf_to_ca_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ float *>(record.dst),
        reinterpret_cast<__cbuf__ float *>(record.src), record.indexId, record.repeat,
        record.srcStride, record.dstStride, record.addrMode, record.dstFracStride);

    __sanitizer_report_load_cbuf_to_ca_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ int32_t *>(record.dst),
        reinterpret_cast<__cbuf__ int32_t *>(record.src), config, fracStride);

    __sanitizer_report_load_cbuf_to_ca_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ int32_t *>(record.dst),
        reinterpret_cast<__cbuf__ int32_t *>(record.src), record.indexId, record.repeat,
        record.srcStride, record.dstStride, record.addrMode, record.dstFracStride);

    __sanitizer_report_load_cbuf_to_ca_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ uint32_t *>(record.dst),
        reinterpret_cast<__cbuf__ uint32_t *>(record.src), config, fracStride);

    __sanitizer_report_load_cbuf_to_ca_transpose(memInfo.data(), record.location.fileNo,
        record.location.lineNo, record.location.pc, reinterpret_cast<__cb__ uint32_t *>(record.dst),
        reinterpret_cast<__cbuf__ uint32_t *>(record.src), record.indexId, record.repeat,
        record.srcStride, record.dstStride, record.addrMode, record.dstFracStride);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DTransposeRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DTransposeRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DTransposeRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DTransposeRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load2DTransposeRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_2D_TRANSPOSE>(ptr, record));
}
