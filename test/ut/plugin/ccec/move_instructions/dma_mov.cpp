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
#include "plugin/ccec/move_instructions/dma_mov.cpp"

using namespace Sanitizer;

template<MemType srcMemType, MemType dstMemType, PadMode padMode = PadMode::PAD_NONE,
         ByteMode byteMode = ByteMode::BM_DISABLE>
DmaMovRecord CreateRandomDmaMovRecord()
{
    DmaMovRecord record{};
    record.dst = RandInt(0, 0x1000000);
    record.src = RandInt(0, 0x1000000);
    record.nBurst = RandInt(0, 0xFFF);
    record.lenBurst = RandInt(0, 0xFFFF);
    record.srcStride = RandInt(0, 0xFFF);
    record.dstStride = RandInt(0, 0xFFF);
    record.location.fileNo = RandInt(0, 0x100000);
    record.location.lineNo = RandInt(0, 0x1000);

    record.padMode = padMode;
    record.byteMode = byteMode;
    record.srcMemType = srcMemType;
    record.dstMemType = dstMemType;
    return record;
}

uint64_t ExtractConfigFromDmaMovRecord(const DmaMovRecord &record)
{
    constexpr uint64_t strideMask = 0xFFFFUL;
    constexpr uint64_t lenBurstMask = 0xFFFFUL;
    constexpr uint64_t nBurstMask = 0xFFFUL;

    constexpr uint64_t dstStrideShift = 48;
    constexpr uint64_t srcStrideShift = 32;
    constexpr uint64_t lenBurstShift = 16;
    constexpr uint64_t nBurstShift = 4;
    return ((record.dstStride & strideMask) << dstStrideShift) |
        ((record.srcStride & strideMask) << srcStrideShift) |
        ((record.lenBurst & lenBurstMask) << lenBurstShift) |
        ((record.nBurst & nBurstMask) << nBurstShift);
}

TEST(DmaMovInstructions, dump_copy_ubuf_to_cbuf_expect_get_correct_records)
{
    auto record = CreateRandomDmaMovRecord<MemType::UB, MemType::L1>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    __sanitizer_report_copy_ubuf_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc,
                                         reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                         0, record.nBurst, record.lenBurst, record.srcStride, record.dstStride);

    uint64_t config = ExtractConfigFromDmaMovRecord(record);
    __sanitizer_report_copy_ubuf_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc,
                                         reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                         config);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV>(ptr, record));
}

TEST(DmaMovInstructions, dump_copy_cbuf_to_ubuf_expect_get_correct_records)
{
    auto record = CreateRandomDmaMovRecord<MemType::L1, MemType::UB>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    __sanitizer_report_copy_cbuf_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc,
                                         reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                         0, record.nBurst, record.lenBurst, record.srcStride, record.dstStride);

    uint64_t config = ExtractConfigFromDmaMovRecord(record);
    __sanitizer_report_copy_cbuf_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc,
                                         reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                         config);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV>(ptr, record));
}

template<DataType dataType>
DmaMovNd2nzRecord CreateRandomDmaMovNd2nzRecord()
{
    DmaMovNd2nzRecord record{};
    record.dst = RandInt(0, 0x1000000);
    record.src = RandInt(0, 0x1000000);
    record.location.fileNo = RandInt(0, 0x100000);
    record.location.lineNo = RandInt(0, 0x1000);
    record.ndNum = RandInt(0, 0xFFF);
    record.nValue = RandInt(0, 0xFFFF);
    record.dValue = RandInt(0, 0xFFFF);
    record.srcNdMatrixStride = RandInt(0, 0xFFFF);
    record.srcDValue = RandInt(0, 0xFFFF);
    record.dstNzC0Stride = RandInt(0, 0xFFFF);
    record.dstNzNStride = RandInt(0, 0xFFFF);
    record.dstNzMatrixStride = RandInt(0, 0xFFFF);

    record.srcMemType = MemType::GM;
    record.dstMemType = MemType::L1;
    record.dataType = dataType;

    return record;
}

void ExtractConfigFromDmaMovNd2nzRecord(const DmaMovNd2nzRecord &record, uint64_t &xm, uint64_t &xt)
{
    constexpr uint64_t mask12Biit = 0xfffUL;
    constexpr uint64_t mask16Biit = 0xffffUL;
    constexpr uint64_t ndNumShift = 4;
    constexpr uint64_t nValueShift = 16;
    constexpr uint64_t dValueShift = 32;
    constexpr uint64_t srcNdMatrixStrideShift = 48;
    constexpr uint64_t srcDValueShift = 0;
    constexpr uint64_t dstNzC0StrideShift = 16;
    constexpr uint64_t dstNzNStrideShift = 32;
    constexpr uint64_t dstNzMatrixStrideShift = 48;

    xm = (((record.ndNum & mask12Biit) << ndNumShift) | ((record.nValue & mask16Biit) << nValueShift) |
        ((record.dValue & mask16Biit) << dValueShift) |
        ((record.srcNdMatrixStride & mask16Biit) << srcNdMatrixStrideShift));
    xt = (((record.srcDValue & mask16Biit) << srcDValueShift) |
        ((record.dstNzC0Stride & mask16Biit) << dstNzC0StrideShift) |
        ((record.dstNzNStride & mask16Biit) << dstNzNStrideShift) |
        ((record.dstNzMatrixStride & mask16Biit) << dstNzMatrixStrideShift));
}

TEST(DmaMovInstructions, dump_copy_cbuf_to_gm_expect_get_correct_records)
{
    auto record = CreateRandomDmaMovRecord<MemType::L1, MemType::GM>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    __sanitizer_report_copy_cbuf_to_gm(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                       0, record.nBurst, record.lenBurst, record.srcStride, record.dstStride);

    uint64_t config = ExtractConfigFromDmaMovRecord(record);
    __sanitizer_report_copy_cbuf_to_gm(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                       config);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV>(ptr, record));
}

TEST(DmaMovInstructions, dump_copy_gm_to_cbuf_expect_get_correct_records)
{
    auto record = CreateRandomDmaMovRecord<MemType::GM, MemType::L1, PadMode::PAD_MODE1>();

    uint8_t sid = 0;
    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractConfigFromDmaMovRecord(record);
    __sanitizer_report_copy_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                       config, PadMode::PAD_MODE1);

    __sanitizer_report_copy_gm_to_cbuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                       sid, record.nBurst, record.lenBurst,
                                       record.srcStride, record.dstStride, PadMode::PAD_MODE1);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV>(ptr, record));
}

TEST(DmaMovInstructions, dump_copy_gm_to_cbuf_multi_nd2nz_b16_expect_get_correct_records)
{
    auto record = CreateRandomDmaMovNd2nzRecord<DataType::DATA_B16>();
    uint64_t xm;
    uint64_t xt;
    ExtractConfigFromDmaMovNd2nzRecord(record, xm, xt);
    uint8_t sid = 0;
    std::vector<uint8_t> memInfo = CreateMemInfo();

    __sanitizer_report_copy_gm_to_cbuf_multi_nd2nz_b16(memInfo.data(), record.location.fileNo,
                                                       record.location.lineNo, record.location.pc,
                                                       reinterpret_cast<__bf16 *>(record.dst),
                                                       reinterpret_cast<__bf16 *>(record.src),
                                                       xm, xt);

    __sanitizer_report_copy_gm_to_cbuf_multi_nd2nz_b16(memInfo.data(), record.location.fileNo,
                                                       record.location.lineNo, record.location.pc,
                                                       reinterpret_cast<__bf16 *>(record.dst),
                                                       reinterpret_cast<__bf16 *>(record.src),
                                                       sid, record.ndNum, record.nValue, record.dValue,
                                                       record.srcNdMatrixStride, record.srcDValue,
                                                       record.dstNzC0Stride, record.dstNzNStride,
                                                       record.dstNzMatrixStride);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_ND2NZ>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovNd2nzRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_ND2NZ>(ptr, record));
}

TEST(DmaMovInstructions, dump_copy_gm_to_cbuf_multi_nd2nz_b32s_expect_get_correct_records)
{
    auto record = CreateRandomDmaMovNd2nzRecord<DataType::DATA_B32>();
    uint64_t xm;
    uint64_t xt;
    ExtractConfigFromDmaMovNd2nzRecord(record, xm, xt);
    uint8_t sid = 0;
    std::vector<uint8_t> memInfo = CreateMemInfo();

    __sanitizer_report_copy_gm_to_cbuf_multi_nd2nz_b32s(memInfo.data(), record.location.fileNo,
                                                       record.location.lineNo, record.location.pc,
                                                        reinterpret_cast<float *>(record.dst),
                                                        reinterpret_cast<float *>(record.src),
                                                        xm, xt);

    __sanitizer_report_copy_gm_to_cbuf_multi_nd2nz_b32s(memInfo.data(), record.location.fileNo,
                                                       record.location.lineNo, record.location.pc,
                                                        reinterpret_cast<float *>(record.dst),
                                                        reinterpret_cast<float *>(record.src),
                                                        sid, record.ndNum, record.nValue, record.dValue,
                                                        record.srcNdMatrixStride, record.srcDValue,
                                                        record.dstNzC0Stride, record.dstNzNStride,
                                                        record.dstNzMatrixStride);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_ND2NZ>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovNd2nzRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_ND2NZ>(ptr, record));
}

TEST(DmaMovInstructions, dump_copy_gm_to_cbuf_multi_nd2nz_b8_expect_get_correct_records)
{
    auto record = CreateRandomDmaMovNd2nzRecord<DataType::DATA_B8>();
    uint64_t xm;
    uint64_t xt;
    ExtractConfigFromDmaMovNd2nzRecord(record, xm, xt);
    uint8_t sid = 0;
    std::vector<uint8_t> memInfo = CreateMemInfo();

    __sanitizer_report_copy_gm_to_cbuf_multi_nd2nz_b8(memInfo.data(), record.location.fileNo,
                                                       record.location.lineNo, record.location.pc,
                                                      reinterpret_cast<int8_t *>(record.dst),
                                                      reinterpret_cast<int8_t *>(record.src),
                                                      xm, xt);

    __sanitizer_report_copy_gm_to_cbuf_multi_nd2nz_b8(memInfo.data(), record.location.fileNo,
                                                       record.location.lineNo, record.location.pc,
                                                      reinterpret_cast<int8_t *>(record.dst),
                                                      reinterpret_cast<int8_t *>(record.src),
                                                      sid, record.ndNum, record.nValue, record.dValue,
                                                      record.srcNdMatrixStride, record.srcDValue,
                                                      record.dstNzC0Stride, record.dstNzNStride,
                                                      record.dstNzMatrixStride);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_ND2NZ>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovNd2nzRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_ND2NZ>(ptr, record));
}

TEST(DmaMovInstructions, dump_copy_gm_to_ubuf_expect_get_correct_records)
{
    auto record = CreateRandomDmaMovRecord<MemType::GM, MemType::UB>();

    uint8_t sid = 0;
    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractConfigFromDmaMovRecord(record);
    __sanitizer_report_copy_gm_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                       config);

    __sanitizer_report_copy_gm_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                       sid, record.nBurst, record.lenBurst, record.srcStride, record.dstStride);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV>(ptr, record));
}

TEST(DmaMovInstructions, dump_copy_ubuf_to_gm_expect_get_correct_records)
{
    auto record0 = CreateRandomDmaMovRecord<MemType::UB, MemType::GM, PadMode::PAD_NONE, ByteMode::BM_ENABLE>();
    auto record1 = CreateRandomDmaMovRecord<MemType::UB, MemType::GM, PadMode::PAD_NONE, ByteMode::BM_DISABLE>();

    uint8_t sid = 0;
    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config0 = ExtractConfigFromDmaMovRecord(record0);
    __sanitizer_report_copy_ubuf_to_gm(memInfo.data(), record0.location.fileNo, record0.location.lineNo,
                                       record0.location.pc,
                                       reinterpret_cast<void *>(record0.dst), reinterpret_cast<void *>(record0.src),
                                       config0, ByteMode::BM_ENABLE);

    __sanitizer_report_copy_ubuf_to_gm(memInfo.data(), record0.location.fileNo, record0.location.lineNo,
                                       record0.location.pc,
                                       reinterpret_cast<void *>(record0.dst), reinterpret_cast<void *>(record0.src),
                                       sid, record0.nBurst, record0.lenBurst, record0.srcStride, record0.dstStride,
                                       ByteMode::BM_ENABLE);

    uint64_t config2 = ExtractConfigFromDmaMovRecord(record1);
    __sanitizer_report_copy_ubuf_to_gm(memInfo.data(), record1.location.fileNo, record1.location.lineNo,
                                       record1.location.pc,
                                       reinterpret_cast<void *>(record1.dst), reinterpret_cast<void *>(record1.src),
                                       config2);

    __sanitizer_report_copy_ubuf_to_gm(memInfo.data(), record1.location.fileNo, record1.location.lineNo,
                                       record1.location.pc,
                                       reinterpret_cast<void *>(record1.dst), reinterpret_cast<void *>(record1.src),
                                       sid, record1.nBurst, record1.lenBurst, record1.srcStride, record1.dstStride);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV>(ptr, record0));
    ptr += sizeof(RecordType) + sizeof(DmaMovRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV>(ptr, record0));
    ptr += sizeof(RecordType) + sizeof(DmaMovRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV>(ptr, record1));
    ptr += sizeof(RecordType) + sizeof(DmaMovRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV>(ptr, record1));
}

TEST(DmaMovInstructions, dump_copy_ubuf_to_ubuf_expect_get_correct_records)
{
    auto record = CreateRandomDmaMovRecord<MemType::UB, MemType::UB>();

    uint8_t sid = 0;
    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config = ExtractConfigFromDmaMovRecord(record);
    __sanitizer_report_copy_ubuf_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                       config);

    __sanitizer_report_copy_ubuf_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                       record.location.pc,
                                       reinterpret_cast<void *>(record.dst), reinterpret_cast<void *>(record.src),
                                       sid, record.nBurst, record.lenBurst, record.srcStride, record.dstStride);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV>(ptr, record));
}

ConvRelu RandCrMode(DataType srcDataType)
{
    std::vector<int> numbers;
    if (srcDataType == DataType::DATA_B16) {
        numbers = {0, 5, 6};
    } else if (srcDataType == DataType::DATA_B32) {
        numbers = {0, 1, 2, 3, 5, 7, 8, 9, 10, 11, 12, 13};
    }
    static std::mt19937 mersenneEngine(std::random_device{}());
    std::uniform_int_distribution<> dist(0, numbers.size() - 1);
    int randomNumber = dist(mersenneEngine);
    return static_cast<ConvRelu>(numbers[randomNumber]);
}

template<MemType srcMemType, MemType dstMemType, DataType srcDataType, DataType dstDataType>
DmaMovConvReluRecord CreateRandomDmaMovConvReluRecord()
{
    DmaMovConvReluRecord record{};
    record.dst = RandInt(0, 0x1000000);
    record.src = RandInt(0, 0x1000000);
    record.nBurst = RandInt(0, 0xFFF);
    record.lenBurst = RandInt(0, 0xFFFF);
    record.srcStride = RandInt(0, 0xFFF);
    record.dstStride = RandInt(0, 0xFFF);
    record.location.fileNo = RandInt(0, 0x100000);
    record.location.lineNo = RandInt(0, 0x1000);

    record.srcMemType = srcMemType;
    record.dstMemType = dstMemType;
    record.srcDataType = srcDataType;
    record.dstDataType = dstDataType;
    record.crMode = RandCrMode(srcDataType);
    return record;
}

uint64_t ExtractConfigFromDmaMovConvReluRecord(const DmaMovConvReluRecord &record)
{
    constexpr uint64_t strideMask = 0xFFFFUL;
    constexpr uint64_t lenBurstMask = 0xFFFFUL;
    constexpr uint64_t nBurstMask = 0xFFFUL;

    constexpr uint64_t dstStrideShift = 48;
    constexpr uint64_t srcStrideShift = 32;
    constexpr uint64_t lenBurstShift = 16;
    constexpr uint64_t nBurstShift = 4;
    return ((record.dstStride & strideMask) << dstStrideShift) |
        ((record.srcStride & strideMask) << srcStrideShift) |
        ((record.lenBurst & lenBurstMask) << lenBurstShift) |
        ((record.nBurst & nBurstMask) << nBurstShift);
}

TEST(DmaMovInstructions, dump_copy_vector_cc_to_ubuf_b16_to_b16_expect_get_correct_records)
{
    auto record = CreateRandomDmaMovConvReluRecord<MemType::L0C, MemType::UB, DataType::DATA_B16, DataType::DATA_B16>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    __sanitizer_report_copy_vector_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc, reinterpret_cast<half *>(record.dst),
                                         reinterpret_cast<half *>(record.src), 0, record.nBurst, record.lenBurst,
                                         record.srcStride, record.dstStride, record.crMode);

    uint64_t config = ExtractConfigFromDmaMovConvReluRecord(record);
    __sanitizer_report_copy_vector_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc,
                                         reinterpret_cast<half *>(record.dst), reinterpret_cast<half *>(record.src),
                                         config, record.crMode);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_CONV_RELU>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovConvReluRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_CONV_RELU>(ptr, record));
}

TEST(DmaMovInstructions, dump_copy_vector_cc_to_ubuf_b32_to_b8_expect_get_correct_records)
{
    auto record = CreateRandomDmaMovConvReluRecord<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B8>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    __sanitizer_report_copy_vector_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc, reinterpret_cast<int8_t *>(record.dst),
                                         reinterpret_cast<int32_t *>(record.src), 0, record.nBurst, record.lenBurst,
                                         record.srcStride, record.dstStride, record.crMode);

    __sanitizer_report_copy_vector_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc, reinterpret_cast<uint8_t *>(record.dst),
                                         reinterpret_cast<int32_t *>(record.src), 0, record.nBurst, record.lenBurst,
                                         record.srcStride, record.dstStride, record.crMode);

    uint64_t config = ExtractConfigFromDmaMovConvReluRecord(record);
    __sanitizer_report_copy_vector_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc, reinterpret_cast<int8_t *>(record.dst),
                                         reinterpret_cast<int32_t *>(record.src), config, record.crMode);

    __sanitizer_report_copy_vector_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc, reinterpret_cast<uint8_t *>(record.dst),
                                         reinterpret_cast<int32_t *>(record.src), config, record.crMode);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_CONV_RELU>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovConvReluRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_CONV_RELU>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovConvReluRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_CONV_RELU>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovConvReluRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_CONV_RELU>(ptr, record));
}

TEST(DmaMovInstructions, dump_copy_vector_cc_to_ubuf_b32_to_b16_expect_get_correct_records)
{
    auto record = CreateRandomDmaMovConvReluRecord<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B16>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    __sanitizer_report_copy_vector_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc, reinterpret_cast<half *>(record.dst),
                                         reinterpret_cast<float *>(record.src), 0, record.nBurst, record.lenBurst,
                                         record.srcStride, record.dstStride, record.crMode);

    __sanitizer_report_copy_vector_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc, reinterpret_cast<half *>(record.dst),
                                         reinterpret_cast<int32_t *>(record.src), 0, record.nBurst, record.lenBurst,
                                         record.srcStride, record.dstStride, record.crMode);

    __sanitizer_report_copy_vector_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc, reinterpret_cast<int16_t *>(record.dst),
                                         reinterpret_cast<int32_t *>(record.src), 0, record.nBurst, record.lenBurst,
                                         record.srcStride, record.dstStride, record.crMode);

    uint64_t config = ExtractConfigFromDmaMovConvReluRecord(record);
    __sanitizer_report_copy_vector_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc, reinterpret_cast<half *>(record.dst),
                                         reinterpret_cast<float *>(record.src), config, record.crMode);

    __sanitizer_report_copy_vector_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc, reinterpret_cast<int16_t *>(record.dst),
                                         reinterpret_cast<int32_t *>(record.src), config, record.crMode);

    __sanitizer_report_copy_vector_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc, reinterpret_cast<half *>(record.dst),
                                         reinterpret_cast<int32_t *>(record.src), config, record.crMode);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_CONV_RELU>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovConvReluRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_CONV_RELU>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovConvReluRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_CONV_RELU>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovConvReluRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_CONV_RELU>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovConvReluRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_CONV_RELU>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovConvReluRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_CONV_RELU>(ptr, record));
}

TEST(DmaMovInstructions, dump_copy_vector_cc_to_ubuf_b32_to_b32_expect_get_correct_records)
{
    auto record = CreateRandomDmaMovConvReluRecord<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B32>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    __sanitizer_report_copy_vector_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc, reinterpret_cast<float *>(record.dst),
                                         reinterpret_cast<float *>(record.src), 0, record.nBurst, record.lenBurst,
                                         record.srcStride, record.dstStride, record.crMode);

    __sanitizer_report_copy_vector_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc, reinterpret_cast<int32_t *>(record.dst),
                                         reinterpret_cast<int32_t *>(record.src), 0, record.nBurst, record.lenBurst,
                                         record.srcStride, record.dstStride, record.crMode);

    __sanitizer_report_copy_vector_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc, reinterpret_cast<uint32_t *>(record.dst),
                                         reinterpret_cast<uint32_t *>(record.src), 0, record.nBurst, record.lenBurst,
                                         record.srcStride, record.dstStride, record.crMode);

    uint64_t config = ExtractConfigFromDmaMovConvReluRecord(record);
    __sanitizer_report_copy_vector_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc,
                                         reinterpret_cast<float *>(record.dst), reinterpret_cast<float *>(record.src),
                                         config, record.crMode);

    __sanitizer_report_copy_vector_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc, reinterpret_cast<int32_t *>(record.dst),
                                         reinterpret_cast<int32_t *>(record.src), config, record.crMode);

    __sanitizer_report_copy_vector_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc, reinterpret_cast<uint32_t *>(record.dst),
                                         reinterpret_cast<uint32_t *>(record.src), config, record.crMode);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_CONV_RELU>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovConvReluRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_CONV_RELU>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovConvReluRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_CONV_RELU>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovConvReluRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_CONV_RELU>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovConvReluRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_CONV_RELU>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovConvReluRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_CONV_RELU>(ptr, record));
}

TEST(DmaMovInstructions, dump_copy_depthwise_cc_to_ubuf_b16_to_b16_expect_get_correct_records)
{
    auto record = CreateRandomDmaMovConvReluRecord<MemType::L0C, MemType::UB, DataType::DATA_B16, DataType::DATA_B16>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    __sanitizer_report_copy_depthwise_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc, reinterpret_cast<half *>(record.dst),
                                         reinterpret_cast<half *>(record.src), 0, record.nBurst, record.lenBurst,
                                         record.srcStride, record.dstStride, record.crMode);

    uint64_t config = ExtractConfigFromDmaMovConvReluRecord(record);
    __sanitizer_report_copy_depthwise_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc,
                                         reinterpret_cast<half *>(record.dst), reinterpret_cast<half *>(record.src),
                                         config, record.crMode);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_DEPTH_WISE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovConvReluRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_DEPTH_WISE>(ptr, record));
}

TEST(DmaMovInstructions, dump_copy_depthwise_cc_to_ubuf_b16_to_b32_expect_get_correct_records)
{
    auto record = CreateRandomDmaMovConvReluRecord<MemType::L0C, MemType::UB, DataType::DATA_B16, DataType::DATA_B32>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    __sanitizer_report_copy_depthwise_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc, reinterpret_cast<float *>(record.dst),
                                         reinterpret_cast<half *>(record.src), 0, record.nBurst, record.lenBurst,
                                         record.srcStride, record.dstStride, record.crMode);

    uint64_t config = ExtractConfigFromDmaMovConvReluRecord(record);
    __sanitizer_report_copy_depthwise_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc,
                                         reinterpret_cast<float *>(record.dst), reinterpret_cast<half *>(record.src),
                                         config, record.crMode);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_DEPTH_WISE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovConvReluRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_DEPTH_WISE>(ptr, record));
}

TEST(DmaMovInstructions, dump_copy_depthwise_cc_to_ubuf_b32_to_b16_expect_get_correct_records)
{
    auto record = CreateRandomDmaMovConvReluRecord<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B16>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    __sanitizer_report_copy_depthwise_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc, reinterpret_cast<half *>(record.dst),
                                         reinterpret_cast<float *>(record.src), 0, record.nBurst, record.lenBurst,
                                         record.srcStride, record.dstStride, record.crMode);

    uint64_t config = ExtractConfigFromDmaMovConvReluRecord(record);
    __sanitizer_report_copy_depthwise_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc, reinterpret_cast<half *>(record.dst),
                                         reinterpret_cast<float *>(record.src), config, record.crMode);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_DEPTH_WISE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovConvReluRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_DEPTH_WISE>(ptr, record));
}

TEST(DmaMovInstructions, dump_copy_depthwise_cc_to_ubuf_b32_to_b32_expect_get_correct_records)
{
    auto record = CreateRandomDmaMovConvReluRecord<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B32>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    __sanitizer_report_copy_depthwise_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc, reinterpret_cast<float *>(record.dst),
                                         reinterpret_cast<float *>(record.src), 0, record.nBurst, record.lenBurst,
                                         record.srcStride, record.dstStride, record.crMode);

    uint64_t config = ExtractConfigFromDmaMovConvReluRecord(record);
    __sanitizer_report_copy_depthwise_cc_to_ubuf(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                         record.location.pc,
                                         reinterpret_cast<float *>(record.dst), reinterpret_cast<float *>(record.src),
                                         config, record.crMode);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_DEPTH_WISE>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(DmaMovConvReluRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::DMA_MOV_DEPTH_WISE>(ptr, record));
}
