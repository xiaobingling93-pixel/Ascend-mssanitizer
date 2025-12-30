
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
#include "plugin/ccec/move_instructions/load_winograd.cpp"

template <MemType srcMemType, MemType dstMemType>
LoadAWinogradRecord CreateLoadAWinograd()
{
    constexpr int8_t fmSizeHWidth = 15;
    constexpr int8_t fmSizeWWidth = 15;
    constexpr int8_t fmSizeChWidth = 11;
    constexpr int8_t innerDstGapWidth = 5;

    constexpr int8_t dstStartPointKWidth = 11;
    constexpr int8_t dstStartPointMWidth = 11;
    constexpr int8_t extStepKWidth = 11;
    constexpr int8_t extStepMWidth = 11;

    // make sure the parameters are valid.
    LoadAWinogradRecord record{};
    record.dst = RandInt(0, 0x1000000);
    record.src = RandInt(0, 0x1000000);
    record.dataType = DataType::DATA_B8;
    record.location.fileNo = RandInt(0, 0x100000);
    record.location.lineNo = RandInt(0, 0x1000);

    record.fmSizeH = RandInt(1, 1 << fmSizeHWidth);
    record.fmSizeW = RandInt(1, 1 << fmSizeWWidth);
    record.fmSizeCh = RandInt(1, 1 << fmSizeChWidth);
    record.innerDstGap = RandInt(0, 1 << innerDstGapWidth);

    record.dstStartPointK = RandInt(0, 1 << dstStartPointKWidth);
    record.dstStartPointM = RandInt(0, 1 << dstStartPointMWidth);
    record.extStepK = RandInt(0, 1 << extStepKWidth);
    record.extStepM = RandInt(0, 1 << extStepMWidth);
    record.srcMemType = srcMemType;
    record.dstMemType = dstMemType;

    return record;
}

template <MemType srcMemType, MemType dstMemType>
LoadBWinogradRecord CreateLoadBWinograd()
{
    constexpr int8_t innerDstStrideWidth = 7;
    constexpr int8_t dstRptStrideWidth = 11;
    constexpr int8_t srcRptStrideWidth = 15;

    LoadBWinogradRecord record{};
    record.dst = RandInt(0, 0x1000000);
    record.src = RandInt(0, 0x1000000);
    record.repeat = RandInt(0, 0xFFF);
    record.location.fileNo = RandInt(0, 0x100000);
    record.location.lineNo = RandInt(0, 0x1000);

    record.innerDstStride = RandInt(0, 1 << innerDstStrideWidth);
    record.dstRptStride = RandInt(0, 1 << dstRptStrideWidth);
    record.srcRptStride = RandInt(0, 1 << srcRptStrideWidth);

    record.srcMemType = srcMemType;
    record.dstMemType = dstMemType;

    return record;
}

void ExtractLoadAWinogradRecord(const LoadAWinogradRecord &record, uint64_t &config0, uint64_t &config1)
{
    constexpr int8_t fmSizeHWidth = 16;
    constexpr int8_t fmSizeChWidth = 32;
    constexpr int8_t innerDstGapWidth = 48;
    constexpr int8_t extStepKWidth = 8;
    constexpr int8_t extStepMWidth = 32;
    constexpr int8_t dstStartPointKWidth = 20;
    constexpr int8_t dstStartPointMWidth = 48;

    config0 = (static_cast<uint64_t>(record.fmSizeW) & 0xFFFF) |
              ((static_cast<uint64_t>(record.fmSizeH) & 0xFFFF) << fmSizeHWidth) |
              ((static_cast<uint64_t>(record.fmSizeCh) & 0x0FFF) << fmSizeChWidth) |
              ((static_cast<uint64_t>(record.innerDstGap) & 0x3F) << innerDstGapWidth);
    config1 = (static_cast<uint64_t>(record.extStepK) & 0xFFF) << extStepKWidth|
              (static_cast<uint64_t>(record.extStepM) & 0xFFFF) << extStepMWidth|
              (static_cast<uint64_t>(record.dstStartPointK) & 0x0FFF) << dstStartPointKWidth|
              (static_cast<uint64_t>(record.dstStartPointM) & 0x0FFF) << dstStartPointMWidth;
}

void ExtractLoadBWinogradRecord(const LoadBWinogradRecord &record, uint64_t &config)
{
    constexpr int8_t repeatWidth = 56;
    constexpr int8_t innerDstStrideWidth = 8;
    constexpr int8_t srcRptStrideWidth = 16;
    constexpr int8_t dstRptStrideWidth = 32;

    config = (static_cast<uint64_t>(record.repeat) & 0xFF) << repeatWidth|
             ((static_cast<uint64_t>(record.innerDstStride) & 0xFF) << innerDstStrideWidth) |
             ((static_cast<uint64_t>(record.srcRptStride) & 0xFFFF) << srcRptStrideWidth) |
             ((static_cast<uint64_t>(record.dstRptStride) & 0xFF) << dstRptStrideWidth);
}

TEST(LoadWinograd, loadWinogradA)
{
    auto recordB16 = CreateLoadAWinograd<MemType::L1, MemType::L0A>();
    recordB16.dataType = DataType::DATA_B16;
    auto recordB8 = CreateLoadAWinograd<MemType::L1, MemType::L0A>();
    recordB8.dataType = DataType::DATA_B8;
    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t config0B16, config1B16;
    uint64_t config0B8, config1B8;
    ExtractLoadAWinogradRecord(recordB16, config0B16, config1B16);
    ExtractLoadAWinogradRecord(recordB8, config0B8, config1B8);

    __sanitizer_report_load_cbuf_to_ca_winograd(memInfo.data(), recordB16.location.fileNo, recordB16.location.lineNo,
                                                recordB16.location.pc,
                                                reinterpret_cast<__cbuf__ half *>(recordB16.dst),
                                                reinterpret_cast<__cb__ half *>(recordB16.src), config0B16, config1B16);
    __sanitizer_report_load_cbuf_to_ca_winograd(memInfo.data(), recordB8.location.fileNo, recordB8.location.lineNo,
                                                recordB8.location.pc,
                                                reinterpret_cast<__cbuf__ uint8_t *>(recordB8.dst),
                                                reinterpret_cast<__cb__ uint8_t *>(recordB8.src), config0B8, config1B8);
    __sanitizer_report_load_cbuf_to_ca_winograd(memInfo.data(), recordB8.location.fileNo, recordB8.location.lineNo,
                                                recordB8.location.pc,
                                                reinterpret_cast<__cbuf__ int8_t *>(recordB8.dst),
                                                reinterpret_cast<__cb__ int8_t *>(recordB8.src), config0B8, config1B8);
    __sanitizer_report_load_cbuf_to_ca_winograd(
        memInfo.data(), recordB16.location.fileNo, recordB16.location.lineNo, recordB16.location.pc,
        reinterpret_cast<__cbuf__ half *>(recordB16.dst), reinterpret_cast<__cb__ half *>(recordB16.src),
        recordB16.fmSizeW, recordB16.fmSizeH, recordB16.fmSizeCh, recordB16.innerDstGap, 0, 0, 0, recordB16.extStepK,
        recordB16.dstStartPointK, recordB16.extStepM, recordB16.dstStartPointM);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_A_WINOGRAD>(ptr, recordB16));
    ptr += sizeof(RecordType) + sizeof(LoadAWinogradRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_A_WINOGRAD>(ptr, recordB8));
    ptr += sizeof(RecordType) + sizeof(LoadAWinogradRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_A_WINOGRAD>(ptr, recordB8));
    ptr += sizeof(RecordType) + sizeof(LoadAWinogradRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_A_WINOGRAD>(ptr, recordB16));
}

TEST(LoadWinograd, loadWinogradB)
{
    auto recordB16 = CreateLoadBWinograd<MemType::L1, MemType::L0B>();
    auto recordB8 = CreateLoadBWinograd<MemType::L1, MemType::L0B>();
    std::vector<uint8_t> memInfo = CreateMemInfo();

    uint64_t configB16;
    ExtractLoadBWinogradRecord(recordB16, configB16);
    uint64_t configB8;
    ExtractLoadBWinogradRecord(recordB8, configB8);

    __sanitizer_report_load_cbuf_to_cb_winograd(memInfo.data(), recordB16.location.fileNo, recordB16.location.lineNo,
                                                recordB16.location.pc,
                                                reinterpret_cast<__cbuf__ half *>(recordB16.dst),
                                                reinterpret_cast<__cb__ half *>(recordB16.src), configB16);
    __sanitizer_report_load_cbuf_to_cb_winograd(memInfo.data(), recordB8.location.fileNo, recordB8.location.lineNo,
                                                recordB8.location.pc,
                                                reinterpret_cast<__cbuf__ int8_t *>(recordB8.dst),
                                                reinterpret_cast<__cb__ int8_t *>(recordB8.src), configB8);
    __sanitizer_report_load_cbuf_to_cb_winograd(
        memInfo.data(), recordB16.location.fileNo, recordB16.location.lineNo, recordB16.location.pc,
        reinterpret_cast<__cbuf__ half *>(recordB16.dst), reinterpret_cast<__cb__ half *>(recordB16.src),
        recordB16.innerDstStride, recordB16.srcRptStride, recordB16.dstRptStride, 0, 0, false, false, recordB16.repeat);
    __sanitizer_report_load_cbuf_to_cb_winograd(
        memInfo.data(), recordB8.location.fileNo, recordB8.location.lineNo, recordB8.location.pc,
        reinterpret_cast<__cbuf__ int8_t *>(recordB8.dst), reinterpret_cast<__cb__ int8_t *>(recordB8.src),
        recordB8.innerDstStride, recordB8.srcRptStride, recordB8.dstRptStride, 0, 0, false, false, recordB8.repeat);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_B_WINOGRAD>(ptr, recordB16));
    ptr += sizeof(RecordType) + sizeof(LoadBWinogradRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_B_WINOGRAD>(ptr, recordB8));
    ptr += sizeof(RecordType) + sizeof(LoadBWinogradRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_B_WINOGRAD>(ptr, recordB16));
    ptr += sizeof(RecordType) + sizeof(LoadBWinogradRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_B_WINOGRAD>(ptr, recordB8));
}
