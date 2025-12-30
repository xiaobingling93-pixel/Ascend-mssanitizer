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
#include <algorithm>
#include <gtest/gtest.h>
#include "record_format.h"

#include "../data_process.h"
#include "../../ccec_defs.h"
#include "plugin/ccec/move_instructions/load_3d.cpp"

using namespace Sanitizer;

template<MemType srcMemType, MemType dstMemType, DataType dataType>
Load3DRecord CreateLoad3DRecord(bool useRpt)
{
    Load3DRecord record{};
    record.srcMemType = srcMemType;
    record.dstMemType = dstMemType;
    record.dataType = dataType;
    record.dst = RandInt(0, 0x1000000);
    record.src = RandInt(0, 0x1000000);
    record.location.fileNo = RandInt(0, 0x100000);
    record.location.lineNo = RandInt(0, 0x1000);
    record.fMapW = RandInt(1, 0xfffe);
    record.fMapH = RandInt(1, 0xfffe);
    record.fMapC = RandInt(1, 0xfffe);
    record.filterW = RandInt(1, 0x1fe);
    record.filterH = RandInt(1, 0x1fe);
    record.filterWStride = RandInt(1, 0x3e);
    record.filterHStride = RandInt(1, 0x3e);
    record.filterWDilation = RandInt(0, 0xff);
    record.filterHDilation = RandInt(0, 0xff);
    record.fMapLeftPad = RandInt(0, 0xff);
    record.fMapRightPad = RandInt(0, 0xff);
    record.fMapTopPad = RandInt(0, 0xff);
    record.fMapBottomPad = RandInt(0, 0xff);
    record.matrixKStep = RandInt(1, 0xfffe);
    record.matrixMStep = RandInt(1, 0xfffe);
    record.matrixKPos = RandInt(0, 0xffff);
    record.matrixMPos = RandInt(0, 0xffff);
    record.matrixMode = RandInt(0, 0x1);
    if (useRpt) {
        record.matrixRptMode = RandInt(0, 0x1);
        uint32_t matrixRptStrideMin = record.matrixKStep > record.matrixMStep ? record.matrixKStep : record.matrixMStep;
        record.matrixRptStride = RandInt(matrixRptStrideMin, 0xffff);
        record.matrixRptTimes = RandInt(1, 0xff);
    } else {
        record.matrixRptMode = 0;
        record.matrixRptStride = 0;
        record.matrixRptTimes = 1;
    }
    return record;
}

void ExtractLoad3DRecordConfig(const Load3DRecord &record, uint64_t &config0, uint64_t &config1)
{
    constexpr uint64_t matrixKStepMask = 0xFFFFUL;
    constexpr uint64_t matrixMStepMask = 0xFFFFUL;
    constexpr uint64_t matrixKPosMask = 0xFFFFUL;
    constexpr uint64_t matrixMPosMask = 0xFFFFUL;

    constexpr uint64_t matrixKStepShift = 0;
    constexpr uint64_t matrixMStepShift = 16;
    constexpr uint64_t matrixKPosShift = 32;
    constexpr uint64_t matrixMPosShift = 48;

    config0 = ((record.matrixKStep & matrixKStepMask) << matrixKStepShift) |
              ((record.matrixMStep & matrixMStepMask) << matrixMStepShift) |
              ((record.matrixKPos & matrixKPosMask) << matrixKPosShift) |
              ((record.matrixMPos & matrixMPosMask) << matrixMPosShift);
    
    constexpr uint64_t filterWStrideMask = 0x3FUL;
    constexpr uint64_t filterHStrideMask = 0x3FUL;
    constexpr uint64_t filterWMask = 0xFFUL;
    constexpr uint64_t filterHMask = 0xFFUL;
    constexpr uint64_t filterWDilationMask = 0xFFUL;
    constexpr uint64_t filterHDilationMask = 0xFFUL;
    constexpr uint64_t filterWTopBitMask = 0x100UL;
    constexpr uint64_t filterHTopBitMask = 0x100UL;
    constexpr uint64_t matrixModeMask = 0x1UL;
    constexpr uint64_t fMapCMask = 0xFFFFUL;

    constexpr uint64_t filterWStrideShift = 0;
    constexpr uint64_t filterHStrideShift = 6;
    constexpr uint64_t filterWShift = 12;
    constexpr uint64_t filterHShift = 20;
    constexpr uint64_t filterWDilationShift = 28;
    constexpr uint64_t filterHDilationShift = 36;
    constexpr uint64_t filterWTopBitShift = 44;
    constexpr uint64_t filterHTopBitShift = 45;
    constexpr uint64_t matrixModeShift = 47;
    constexpr uint64_t fMapCShift = 48;

    config1 = ((record.filterWStride & filterWStrideMask) << filterWStrideShift) |
              ((record.filterHStride & filterHStrideMask) << filterHStrideShift) |
              ((record.filterW & filterWMask) << filterWShift) |
              ((record.filterH & filterHMask) << filterHShift) |
              ((record.filterWDilation & filterWDilationMask) << filterWDilationShift) |
              ((record.filterHDilation & filterHDilationMask) << filterHDilationShift) |
              (((record.filterW & filterWTopBitMask) >> 8U) << filterWTopBitShift) |
              (((record.filterH & filterHTopBitMask) >> 8U) << filterHTopBitShift) |
              ((record.matrixMode & matrixModeMask) << matrixModeShift) |
              ((record.fMapC & fMapCMask) << fMapCShift);
}

void ExtractLoad3DRecordFmatrix(const Load3DRecord &record, uint64_t &fmatrix)
{
    constexpr uint64_t fMapWMask = 0xFFFFUL;
    constexpr uint64_t fMapHMask = 0xFFFFUL;
    constexpr uint64_t fMapLeftPadMask = 0xFFUL;
    constexpr uint64_t fMapRightPadMask = 0xFFUL;
    constexpr uint64_t fMapTopMask = 0xFFUL;
    constexpr uint64_t fMapBottomWMask = 0xFFUL;

    constexpr uint64_t fMapWShift = 0;
    constexpr uint64_t fMapHShift = 16;
    constexpr uint64_t fMapLeftPadShift = 32;
    constexpr uint64_t fMapRightPadShift = 40;
    constexpr uint64_t fMapTopShift = 48;
    constexpr uint64_t fMapBottomWShift = 56;

    fmatrix = ((record.fMapW & fMapWMask) << fMapWShift) |
              ((record.fMapH & fMapHMask) << fMapHShift) |
              ((record.fMapLeftPad & fMapLeftPadMask) << fMapLeftPadShift) |
              ((record.fMapRightPad & fMapRightPadMask) << fMapRightPadShift) |
              ((record.fMapTopPad & fMapTopMask) << fMapTopShift) |
              ((record.fMapBottomPad & fMapBottomWMask) << fMapBottomWShift);
}

void ExtractLoad3DRpt(const Load3DRecord &record, uint64_t &rpt)
{
    constexpr uint64_t matrixRptStrideMask = 0xFFFFUL;
    constexpr uint64_t matrixRptTimesMask = 0xFFUL;
    constexpr uint64_t matrixRptModeMask = 0x1UL;

    constexpr uint64_t matrixRptStrideShift = 0;
    constexpr uint64_t matrixRptTimesShift = 16;
    constexpr uint64_t matrixRptModeShift = 24;

    rpt = ((record.matrixRptStride & matrixRptStrideMask) << matrixRptStrideShift) |
          ((record.matrixRptTimes & matrixRptTimesMask) << matrixRptTimesShift) |
          ((record.matrixRptMode & matrixRptModeMask) << matrixRptModeShift);
}

TEST(Load3DInstructions, img2colv2_cbuf_to_ca_b4)
{
    bool useRpt = true;
    auto record = CreateLoad3DRecord<MemType::L1, MemType::L0A, DataType::DATA_B4>(useRpt);

    std::vector<uint8_t> memInfo = CreateMemInfo();
    uint64_t config0 = 0, config1 = 0, fmatrix = 0, rpt = 0;
    
    ExtractLoad3DRecordConfig(record, config0, config1);
    ExtractLoad3DRecordFmatrix(record, fmatrix);
    ExtractLoad3DRpt(record, rpt);

    // 2126
    __sanitizer_report_set_fmatrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                   record.location.pc, fmatrix);
    // 2127
    __sanitizer_report_set_fmatrixB(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                    record.location.pc, fmatrix);
    // 2168
    __sanitizer_report_set_l3d_rpt(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                   record.location.pc, rpt);
    // 967
    __sanitizer_report_img2colv2_cbuf_to_ca_s4(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__ca__ void *>(record.dst), reinterpret_cast<__cbuf__ void *>(record.src),
        config0, config1);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
}

TEST(Load3DInstructions, img2colv2_cbuf_to_ca_b8)
{
    bool useRpt = true;
    auto record = CreateLoad3DRecord<MemType::L1, MemType::L0A, DataType::DATA_B8>(useRpt);

    std::vector<uint8_t> memInfo = CreateMemInfo();
    uint64_t config0 = 0, config1 = 0, fmatrix = 0, rpt = 0;
    
    ExtractLoad3DRecordConfig(record, config0, config1);
    ExtractLoad3DRecordFmatrix(record, fmatrix);
    ExtractLoad3DRpt(record, rpt);

    // 2126
    __sanitizer_report_set_fmatrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                   record.location.pc, fmatrix);
    // 2127
    __sanitizer_report_set_fmatrixB(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                    record.location.pc, fmatrix);
    // 2168
    __sanitizer_report_set_l3d_rpt(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                   record.location.pc, rpt);
    // 942
    __sanitizer_report_img2colv2_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__ca__ int8_t *>(record.dst), reinterpret_cast< __cbuf__ int8_t *>(record.src),
        config0, config1);
    // 956
    __sanitizer_report_img2colv2_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__ca__ uint8_t *>(record.dst), reinterpret_cast<__cbuf__ uint8_t *>(record.src),
        config0, config1);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load3DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
}

TEST(Load3DInstructions, img2colv2_cbuf_to_ca_b16)
{
    bool useRpt = true;
    auto record = CreateLoad3DRecord<MemType::L1, MemType::L0A, DataType::DATA_B16>(useRpt);

    std::vector<uint8_t> memInfo = CreateMemInfo();
    uint64_t config0 = 0, config1 = 0, fmatrix = 0, rpt = 0;
    
    ExtractLoad3DRecordConfig(record, config0, config1);
    ExtractLoad3DRecordFmatrix(record, fmatrix);
    ExtractLoad3DRpt(record, rpt);

    // 2126
    __sanitizer_report_set_fmatrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                   record.location.pc, fmatrix);
    // 2127
    __sanitizer_report_set_fmatrixB(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                    record.location.pc, fmatrix);
    // 2168
    __sanitizer_report_set_l3d_rpt(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                   record.location.pc, rpt);
    // 895
    __sanitizer_report_img2colv2_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__ca__ __bf16*>(record.dst), reinterpret_cast<__cbuf__ __bf16*>(record.src),
        config0, config1);
    // 897
    __sanitizer_report_img2colv2_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__ca__ half *>(record.dst), reinterpret_cast<__cbuf__ half *>(record.src),
        config0, config1);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load3DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
}

TEST(Load3DInstructions, img2colv2_cbuf_to_ca_b32)
{
    bool useRpt = true;
    auto record = CreateLoad3DRecord<MemType::L1, MemType::L0A, DataType::DATA_B32>(useRpt);

    std::vector<uint8_t> memInfo = CreateMemInfo();
    uint64_t config0 = 0, config1 = 0, fmatrix = 0, rpt = 0;
    
    ExtractLoad3DRecordConfig(record, config0, config1);
    ExtractLoad3DRecordFmatrix(record, fmatrix);
    ExtractLoad3DRpt(record, rpt);

    // 2126
    __sanitizer_report_set_fmatrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                   record.location.pc, fmatrix);
    // 2127
    __sanitizer_report_set_fmatrixB(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                    record.location.pc, fmatrix);
    // 2168
    __sanitizer_report_set_l3d_rpt(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                   record.location.pc, rpt);
    // 904
    __sanitizer_report_img2colv2_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__ca__ float *>(record.dst), reinterpret_cast<__cbuf__ float *>(record.src),
        config0, config1);
    // 935
    __sanitizer_report_img2colv2_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__ca__ int32_t *>(record.dst), reinterpret_cast<__cbuf__ int32_t *>(record.src),
        config0, config1);
    // 949
    __sanitizer_report_img2colv2_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__ca__ uint32_t *>(record.dst), reinterpret_cast<__cbuf__ uint32_t *>(record.src),
        config0, config1);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load3DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load3DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
}

TEST(Load3DInstructions, img2colv2_cbuf_to_ca_b4_without_rpt)
{
    bool useRpt = false;
    auto record = CreateLoad3DRecord<MemType::L1, MemType::L0A, DataType::DATA_B4>(useRpt);

    std::vector<uint8_t> memInfo = CreateMemInfo();
    uint64_t config0 = 0, config1 = 0, fmatrix = 0;
    
    ExtractLoad3DRecordConfig(record, config0, config1);
    ExtractLoad3DRecordFmatrix(record, fmatrix);

    // 2126
    __sanitizer_report_set_fmatrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                   record.location.pc, fmatrix);
    // 2127
    __sanitizer_report_set_fmatrixB(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                    record.location.pc, fmatrix);
    // 967
    __sanitizer_report_img2colv2_cbuf_to_ca_s4(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__ca__ void *>(record.dst), reinterpret_cast<__cbuf__ void *>(record.src),
        config0, config1);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
}

TEST(Load3DInstructions, img2colv2_cbuf_to_ca_b8_without_rpt)
{
    bool useRpt = false;
    auto record = CreateLoad3DRecord<MemType::L1, MemType::L0A, DataType::DATA_B8>(useRpt);

    std::vector<uint8_t> memInfo = CreateMemInfo();
    uint64_t config0 = 0, config1 = 0, fmatrix = 0;
    
    ExtractLoad3DRecordConfig(record, config0, config1);
    ExtractLoad3DRecordFmatrix(record, fmatrix);

    // 2126
    __sanitizer_report_set_fmatrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                   record.location.pc, fmatrix);
    // 2127
    __sanitizer_report_set_fmatrixB(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                    record.location.pc, fmatrix);
    // 942
    __sanitizer_report_img2colv2_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__ca__ int8_t *>(record.dst), reinterpret_cast< __cbuf__ int8_t *>(record.src),
        config0, config1);
    // 956
    __sanitizer_report_img2colv2_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__ca__ uint8_t *>(record.dst), reinterpret_cast<__cbuf__ uint8_t *>(record.src),
        config0, config1);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load3DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
}

TEST(Load3DInstructions, img2colv2_cbuf_to_ca_b16_without_rpt)
{
    bool useRpt = false;
    auto record = CreateLoad3DRecord<MemType::L1, MemType::L0A, DataType::DATA_B16>(useRpt);

    std::vector<uint8_t> memInfo = CreateMemInfo();
    uint64_t config0 = 0, config1 = 0, fmatrix = 0;
    
    ExtractLoad3DRecordConfig(record, config0, config1);
    ExtractLoad3DRecordFmatrix(record, fmatrix);

    // 2126
    __sanitizer_report_set_fmatrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                   record.location.pc, fmatrix);
    // 2127
    __sanitizer_report_set_fmatrixB(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                    record.location.pc, fmatrix);
    // 895
    __sanitizer_report_img2colv2_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__ca__ __bf16*>(record.dst), reinterpret_cast<__cbuf__ __bf16*>(record.src),
        config0, config1);
    // 897
    __sanitizer_report_img2colv2_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__ca__ half *>(record.dst), reinterpret_cast<__cbuf__ half *>(record.src),
        config0, config1);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load3DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
}

TEST(Load3DInstructions, img2colv2_cbuf_to_ca_b32_without_rpt)
{
    bool useRpt = false;
    auto record = CreateLoad3DRecord<MemType::L1, MemType::L0A, DataType::DATA_B32>(useRpt);

    std::vector<uint8_t> memInfo = CreateMemInfo();
    uint64_t config0 = 0, config1 = 0, fmatrix = 0;
    
    ExtractLoad3DRecordConfig(record, config0, config1);
    ExtractLoad3DRecordFmatrix(record, fmatrix);

    // 2126
    __sanitizer_report_set_fmatrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                   record.location.pc, fmatrix);
    // 2127
    __sanitizer_report_set_fmatrixB(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                    record.location.pc, fmatrix);
    // 904
    __sanitizer_report_img2colv2_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__ca__ float *>(record.dst), reinterpret_cast<__cbuf__ float *>(record.src),
        config0, config1);
    // 935
    __sanitizer_report_img2colv2_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__ca__ int32_t *>(record.dst), reinterpret_cast<__cbuf__ int32_t *>(record.src),
        config0, config1);
    // 949
    __sanitizer_report_img2colv2_cbuf_to_ca(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__ca__ uint32_t *>(record.dst), reinterpret_cast<__cbuf__ uint32_t *>(record.src),
        config0, config1);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load3DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load3DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
}

TEST(Load3DInstructions, img2colv2_cbuf_to_cb_b16)
{
    bool useRpt = true;
    auto record = CreateLoad3DRecord<MemType::L1, MemType::L0B, DataType::DATA_B16>(useRpt);

    std::vector<uint8_t> memInfo = CreateMemInfo();
    uint64_t config0 = 0, config1 = 0, fmatrix = 0, rpt = 0;
    ExtractLoad3DRecordConfig(record, config0, config1);
    ExtractLoad3DRecordFmatrix(record, fmatrix);
    ExtractLoad3DRpt(record, rpt);

    // 2126
    __sanitizer_report_set_fmatrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                   record.location.pc, fmatrix);
    // 2127
    __sanitizer_report_set_fmatrixB(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                    record.location.pc, fmatrix);
    // 2168
    __sanitizer_report_set_l3d_rpt(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                   record.location.pc, rpt);
    // 974
    __sanitizer_report_img2colv2_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cb__ __bf16*>(record.dst), reinterpret_cast<__cbuf__ __bf16*>(record.src),
        config0, config1);
    // 976
    __sanitizer_report_img2colv2_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cb__ half *>(record.dst), reinterpret_cast<__cbuf__ half *>(record.src),
        config0, config1);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load3DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
}

TEST(Load3DInstructions, img2colv2_cbuf_to_cb_b32)
{
    bool useRpt = true;
    auto record = CreateLoad3DRecord<MemType::L1, MemType::L0B, DataType::DATA_B32>(useRpt);

    std::vector<uint8_t> memInfo = CreateMemInfo();
    uint64_t config0 = 0, config1 = 0, fmatrix = 0, rpt = 0;
    ExtractLoad3DRecordConfig(record, config0, config1);
    ExtractLoad3DRecordFmatrix(record, fmatrix);
    ExtractLoad3DRpt(record, rpt);

    // 2126
    __sanitizer_report_set_fmatrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                   record.location.pc, fmatrix);
    // 2127
    __sanitizer_report_set_fmatrixB(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                    record.location.pc, fmatrix);
    // 2168
    __sanitizer_report_set_l3d_rpt(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                   record.location.pc, rpt);
    // 983
    __sanitizer_report_img2colv2_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cb__ float *>(record.dst), reinterpret_cast<__cbuf__ float *>(record.src),
        config0, config1);
    // 998
    __sanitizer_report_img2colv2_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cb__ int32_t *>(record.dst), reinterpret_cast<__cbuf__ int32_t *>(record.src),
        config0, config1);
    // 1012
    __sanitizer_report_img2colv2_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cb__ uint32_t *>(record.dst), reinterpret_cast<__cbuf__ uint32_t *>(record.src),
        config0, config1);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load3DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load3DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
}

TEST(Load3DInstructions, img2colv2_cbuf_to_cb_b16_without_rpt)
{
    bool useRpt = false;
    auto record = CreateLoad3DRecord<MemType::L1, MemType::L0B, DataType::DATA_B16>(useRpt);

    std::vector<uint8_t> memInfo = CreateMemInfo();
    uint64_t config0 = 0, config1 = 0, fmatrix = 0;
    ExtractLoad3DRecordConfig(record, config0, config1);
    ExtractLoad3DRecordFmatrix(record, fmatrix);

    // 2126
    __sanitizer_report_set_fmatrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                   record.location.pc, fmatrix);
    // 2127
    __sanitizer_report_set_fmatrixB(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                    record.location.pc, fmatrix);
    // 974
    __sanitizer_report_img2colv2_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cb__ __bf16*>(record.dst), reinterpret_cast<__cbuf__ __bf16*>(record.src),
        config0, config1);
    // 976
    __sanitizer_report_img2colv2_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cb__ half *>(record.dst), reinterpret_cast<__cbuf__ half *>(record.src),
        config0, config1);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load3DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
}

TEST(Load3DInstructions, img2colv2_cbuf_to_cb_b32_without_rpt)
{
    bool useRpt = false;
    auto record = CreateLoad3DRecord<MemType::L1, MemType::L0B, DataType::DATA_B32>(useRpt);

    std::vector<uint8_t> memInfo = CreateMemInfo();
    uint64_t config0 = 0, config1 = 0, fmatrix = 0;
    ExtractLoad3DRecordConfig(record, config0, config1);
    ExtractLoad3DRecordFmatrix(record, fmatrix);

    // 2126
    __sanitizer_report_set_fmatrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                   record.location.pc, fmatrix);
    // 2127
    __sanitizer_report_set_fmatrixB(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                    record.location.pc, fmatrix);
    // 983
    __sanitizer_report_img2colv2_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cb__ float *>(record.dst), reinterpret_cast<__cbuf__ float *>(record.src),
        config0, config1);
    // 998
    __sanitizer_report_img2colv2_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cb__ int32_t *>(record.dst), reinterpret_cast<__cbuf__ int32_t *>(record.src),
        config0, config1);
    // 1012
    __sanitizer_report_img2colv2_cbuf_to_cb(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc,
        reinterpret_cast<__cb__ uint32_t *>(record.dst), reinterpret_cast<__cbuf__ uint32_t *>(record.src),
        config0, config1);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load3DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(Load3DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::LOAD_3D>(ptr, record));
}
