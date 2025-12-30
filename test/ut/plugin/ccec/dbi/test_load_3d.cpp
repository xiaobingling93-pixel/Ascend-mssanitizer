// Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.

#include <utility>
#include <gtest/gtest.h>

#include "../data_process.h"
#define BUILD_DYNAMIC_PROBE
#include "plugin/ccec/dbi/probes/load_3d.cpp"
#include "plugin/ccec/dbi/probes/cube_mte_registers.cpp"

TEST(Load3DInstructions, load_cbuf_to_ca_3dv2_b32)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    Load3DV2Record record{};
    record.dst = 0x1000;
    record.src = 0x1000;

    record.matrixKStep = 32;
    record.matrixMStep = 32;
    record.matrixKPos = 0;
    record.matrixMPos = 0;

    record.filterWStride = 1;
    record.filterHStride = 1;
    record.filterW = 64;
    record.filterH = 64;
    record.filterWDilation = 1;
    record.filterHDilation = 1;
    record.transposeMode = 0;
    record.matrixMode = 0;
    record.fMapC = 32;

    record.fMapW = 512;
    record.fMapH = 512;
    record.fMapLeftPad = 0;
    record.fMapRightPad = 0;
    record.fMapTopPad = 0;
    record.fMapBottomPad = 0;
    
    record.matrixRptStride = 64;
    record.matrixRptTimes = 64;
    record.matrixRptMode = 0;
    record.dstStride = 32;
    record.outputMPos = 0;

    record.dataType = DataType::DATA_B32;
    record.dstMemType = MemType::L0A;
    record.srcMemType = MemType::L1;

    uint64_t xm = 0;
    uint64_t xt = 0;
    SetConfByUint<15, 0>(xm, 32);//kstep
    SetConfByUint<31, 16>(xm, 32);//mstep
    SetConfByUint<47, 32>(xm, 0);//kstart
    SetConfByUint<63, 48>(xm, 0);//mstart
    SetConfByUint<5, 0>(xt, 1);//wstride
    SetConfByUint<11, 6>(xt, 1);//hstride
    SetConfByUint<19, 12>(xt, 64);//low bit of wsize
    SetConfByUint<44, 44>(xt, 0);//high bit of wsize
    SetConfByUint<27, 20>(xt, 64);//low bit of hsize
    SetConfByUint<45, 45>(xt, 0);//high bit of hsize
    SetConfByUint<35, 28>(xt, 1);//wDilation
    SetConfByUint<43, 36>(xt, 1);//hDilation
    SetConfByUint<46, 46>(xt, 0);//Transpose Control
    SetConfByUint<47, 47>(xt, 0);//SPR Control
    SetConfByUint<63, 48>(xt, 32);//channel size N

    uint64_t sprMatrix = 0;
    uint64_t sprL3dRpt = 0;
    SetConfByUint<15, 0>(sprMatrix, 512);//W
    SetConfByUint<31, 16>(sprMatrix, 512);//H
    SetConfByUint<39, 32>(sprMatrix, 0);//left padding
    SetConfByUint<47, 40>(sprMatrix, 0);//right padding
    SetConfByUint<55, 48>(sprMatrix, 0);//top padding
    SetConfByUint<63, 56>(sprMatrix, 0);//bottom padding

    SetConfByUint<15, 0>(sprL3dRpt, 64);//repeatStride
    SetConfByUint<23, 16>(sprL3dRpt, 64);//repeatTimes
    SetConfByUint<24, 24>(sprL3dRpt, 0);//repeatMode
    SetConfByUint<47, 32>(sprL3dRpt, 32);//dstStride
    SetConfByUint<63, 48>(sprL3dRpt, 0);//mStartPosition output
    
    __sanitizer_report_set_fmatrix(memInfo.data(), record.location.pc, 0, sprMatrix);
    __sanitizer_report_set_l3d_rpt(memInfo.data(), record.location.pc, 0, sprL3dRpt);

    auto dst = (__ca__ uint32_t *)(0x1000);
    auto src = (__cbuf__ uint32_t *)(0x1000);

    __sanitizer_report_img2colv2_cbuf_to_ca_b32(memInfo.data(),
        record.location.pc,
        0,
        dst,
        src,
        xm,
        xt);

    uint8_t *ptr = memInfo.data();
    ptr += sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    auto x = reinterpret_cast<Sanitizer::Load3DV2Record *>(ptr + sizeof(RecordType));
    record.location = x->location;
    ASSERT_TRUE(x->dstStride == record.dstStride);
}

TEST(Load3DInstructions, load_cbuf_to_ca_3dv2_b16)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    Load3DV2Record record{};
    record.dst = 0x1000;
    record.src = 0x1000;

    record.matrixKStep = 32;
    record.matrixMStep = 32;
    record.matrixKPos = 0;
    record.matrixMPos = 0;

    record.filterWStride = 1;
    record.filterHStride = 1;
    record.filterW = 64;
    record.filterH = 64;
    record.filterWDilation = 1;
    record.filterHDilation = 1;
    record.transposeMode = 0;
    record.matrixMode = 0;
    record.fMapC = 32;

    record.fMapW = 512;
    record.fMapH = 512;
    record.fMapLeftPad = 0;
    record.fMapRightPad = 0;
    record.fMapTopPad = 0;
    record.fMapBottomPad = 0;
    
    record.matrixRptStride = 64;
    record.matrixRptTimes = 64;
    record.matrixRptMode = 0;
    record.dstStride = 32;
    record.outputMPos = 0;

    record.dataType = DataType::DATA_B16;
    record.dstMemType = MemType::L0A;
    record.srcMemType = MemType::L1;

    uint64_t xm = 0;
    uint64_t xt = 0;
    SetConfByUint<15, 0>(xm, 32);//kstep
    SetConfByUint<31, 16>(xm, 32);//mstep
    SetConfByUint<47, 32>(xm, 0);//kstart
    SetConfByUint<63, 48>(xm, 0);//mstart
    SetConfByUint<5, 0>(xt, 1);//wstride
    SetConfByUint<11, 6>(xt, 1);//hstride
    SetConfByUint<19, 12>(xt, 64);//low bit of wsize
    SetConfByUint<44, 44>(xt, 0);//high bit of wsize
    SetConfByUint<27, 20>(xt, 64);//low bit of hsize
    SetConfByUint<45, 45>(xt, 0);//high bit of hsize
    SetConfByUint<35, 28>(xt, 1);//wDilation
    SetConfByUint<43, 36>(xt, 1);//hDilation
    SetConfByUint<46, 46>(xt, 0);//Transpose Control
    SetConfByUint<47, 47>(xt, 0);//SPR Control
    SetConfByUint<63, 48>(xt, 32);//channel size N

    uint64_t sprMatrix = 0;
    uint64_t sprL3dRpt = 0;
    SetConfByUint<15, 0>(sprMatrix, 512);//W
    SetConfByUint<31, 16>(sprMatrix, 512);//H
    SetConfByUint<39, 32>(sprMatrix, 0);//left padding
    SetConfByUint<47, 40>(sprMatrix, 0);//right padding
    SetConfByUint<55, 48>(sprMatrix, 0);//top padding
    SetConfByUint<63, 56>(sprMatrix, 0);//bottom padding

    SetConfByUint<15, 0>(sprL3dRpt, 64);//repeatStride
    SetConfByUint<23, 16>(sprL3dRpt, 64);//repeatTimes
    SetConfByUint<24, 24>(sprL3dRpt, 0);//repeatMode
    SetConfByUint<47, 32>(sprL3dRpt, 32);//dstStride
    SetConfByUint<63, 48>(sprL3dRpt, 0);//mStartPosition output
    
    __sanitizer_report_set_fmatrix(memInfo.data(), record.location.pc, 0, sprMatrix);
    __sanitizer_report_set_l3d_rpt(memInfo.data(), record.location.pc, 0, sprL3dRpt);

    auto dst = (__ca__ uint16_t *)(0x1000);
    auto src = (__cbuf__ uint16_t *)(0x1000);

    __sanitizer_report_img2colv2_cbuf_to_ca_b16(memInfo.data(),
        record.location.pc,
        0,
        dst,
        src,
        xm,
        xt);

    uint8_t *ptr = memInfo.data();
    ptr += sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    auto x = reinterpret_cast<Sanitizer::Load3DV2Record *>(ptr + sizeof(RecordType));
    record.location = x->location;
    ASSERT_TRUE(*x == record);
}


TEST(Load3DInstructions, load_cbuf_to_ca_3dv2_b8)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    Load3DV2Record record{};
    record.dst = 0x1000;
    record.src = 0x1000;

    record.matrixKStep = 32;
    record.matrixMStep = 32;
    record.matrixKPos = 0;
    record.matrixMPos = 0;

    record.filterWStride = 1;
    record.filterHStride = 1;
    record.filterW = 64;
    record.filterH = 64;
    record.filterWDilation = 1;
    record.filterHDilation = 1;
    record.transposeMode = 0;
    record.matrixMode = 0;
    record.fMapC = 32;

    record.fMapW = 512;
    record.fMapH = 512;
    record.fMapLeftPad = 0;
    record.fMapRightPad = 0;
    record.fMapTopPad = 0;
    record.fMapBottomPad = 0;
    
    record.matrixRptStride = 64;
    record.matrixRptTimes = 64;
    record.matrixRptMode = 0;
    record.dstStride = 32;
    record.outputMPos = 0;

    record.dataType = DataType::DATA_B8;
    record.dstMemType = MemType::L0A;
    record.srcMemType = MemType::L1;

    uint64_t xm = 0;
    uint64_t xt = 0;
    SetConfByUint<15, 0>(xm, 32);//kstep
    SetConfByUint<31, 16>(xm, 32);//mstep
    SetConfByUint<47, 32>(xm, 0);//kstart
    SetConfByUint<63, 48>(xm, 0);//mstart
    SetConfByUint<5, 0>(xt, 1);//wstride
    SetConfByUint<11, 6>(xt, 1);//hstride
    SetConfByUint<19, 12>(xt, 64);//low bit of wsize
    SetConfByUint<44, 44>(xt, 0);//high bit of wsize
    SetConfByUint<27, 20>(xt, 64);//low bit of hsize
    SetConfByUint<45, 45>(xt, 0);//high bit of hsize
    SetConfByUint<35, 28>(xt, 1);//wDilation
    SetConfByUint<43, 36>(xt, 1);//hDilation
    SetConfByUint<46, 46>(xt, 0);//Transpose Control
    SetConfByUint<47, 47>(xt, 0);//SPR Control
    SetConfByUint<63, 48>(xt, 32);//channel size N

    uint64_t sprMatrix = 0;
    uint64_t sprL3dRpt = 0;
    SetConfByUint<15, 0>(sprMatrix, 512);//W
    SetConfByUint<31, 16>(sprMatrix, 512);//H
    SetConfByUint<39, 32>(sprMatrix, 0);//left padding
    SetConfByUint<47, 40>(sprMatrix, 0);//right padding
    SetConfByUint<55, 48>(sprMatrix, 0);//top padding
    SetConfByUint<63, 56>(sprMatrix, 0);//bottom padding

    SetConfByUint<15, 0>(sprL3dRpt, 64);//repeatStride
    SetConfByUint<23, 16>(sprL3dRpt, 64);//repeatTimes
    SetConfByUint<24, 24>(sprL3dRpt, 0);//repeatMode
    SetConfByUint<47, 32>(sprL3dRpt, 32);//dstStride
    SetConfByUint<63, 48>(sprL3dRpt, 0);//mStartPosition output
    
    __sanitizer_report_set_fmatrix(memInfo.data(), record.location.pc, 0, sprMatrix);
    __sanitizer_report_set_l3d_rpt(memInfo.data(), record.location.pc, 0, sprL3dRpt);

    auto dst = (__ca__ uint8_t *)(0x1000);
    auto src = (__cbuf__ uint8_t *)(0x1000);

    __sanitizer_report_img2colv2_cbuf_to_ca_b8(memInfo.data(),
        record.location.pc,
        0,
        dst,
        src,
        xm,
        xt);

    uint8_t *ptr = memInfo.data();
    ptr += sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    auto x = reinterpret_cast<Sanitizer::Load3DV2Record *>(ptr + sizeof(RecordType));
    record.location = x->location;
    ASSERT_TRUE(*x == record);
}

TEST(Load3DInstructions, load_cbuf_to_cb_3dv2_b32)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    Load3DV2Record record{};
    record.dst = 0x1000;
    record.src = 0x1000;

    record.matrixKStep = 32;
    record.matrixMStep = 32;
    record.matrixKPos = 0;
    record.matrixMPos = 0;

    record.filterWStride = 1;
    record.filterHStride = 1;
    record.filterW = 64;
    record.filterH = 64;
    record.filterWDilation = 1;
    record.filterHDilation = 1;
    record.transposeMode = 0;
    record.matrixMode = 0;
    record.fMapC = 32;

    record.fMapW = 512;
    record.fMapH = 512;
    record.fMapLeftPad = 0;
    record.fMapRightPad = 0;
    record.fMapTopPad = 0;
    record.fMapBottomPad = 0;
    
    record.matrixRptStride = 64;
    record.matrixRptTimes = 64;
    record.matrixRptMode = 0;
    record.dstStride = 32;
    record.outputMPos = 0;

    record.dataType = DataType::DATA_B32;
    record.dstMemType = MemType::L0B;
    record.srcMemType = MemType::L1;

    uint64_t xm = 0;
    uint64_t xt = 0;
    SetConfByUint<15, 0>(xm, 32);//kstep
    SetConfByUint<31, 16>(xm, 32);//mstep
    SetConfByUint<47, 32>(xm, 0);//kstart
    SetConfByUint<63, 48>(xm, 0);//mstart
    SetConfByUint<5, 0>(xt, 1);//wstride
    SetConfByUint<11, 6>(xt, 1);//hstride
    SetConfByUint<19, 12>(xt, 64);//low bit of wsize
    SetConfByUint<44, 44>(xt, 0);//high bit of wsize
    SetConfByUint<27, 20>(xt, 64);//low bit of hsize
    SetConfByUint<45, 45>(xt, 0);//high bit of hsize
    SetConfByUint<35, 28>(xt, 1);//wDilation
    SetConfByUint<43, 36>(xt, 1);//hDilation
    SetConfByUint<46, 46>(xt, 0);//Transpose Control
    SetConfByUint<47, 47>(xt, 0);//SPR Control
    SetConfByUint<63, 48>(xt, 32);//channel size N

    uint64_t sprMatrix = 0;
    uint64_t sprL3dRpt = 0;
    SetConfByUint<15, 0>(sprMatrix, 512);//W
    SetConfByUint<31, 16>(sprMatrix, 512);//H
    SetConfByUint<39, 32>(sprMatrix, 0);//left padding
    SetConfByUint<47, 40>(sprMatrix, 0);//right padding
    SetConfByUint<55, 48>(sprMatrix, 0);//top padding
    SetConfByUint<63, 56>(sprMatrix, 0);//bottom padding

    SetConfByUint<15, 0>(sprL3dRpt, 64);//repeatStride
    SetConfByUint<23, 16>(sprL3dRpt, 64);//repeatTimes
    SetConfByUint<24, 24>(sprL3dRpt, 0);//repeatMode
    SetConfByUint<47, 32>(sprL3dRpt, 32);//dstStride
    SetConfByUint<63, 48>(sprL3dRpt, 0);//mStartPosition output
    
    __sanitizer_report_set_fmatrix(memInfo.data(), record.location.pc, 0, sprMatrix);
    __sanitizer_report_set_l3d_rpt(memInfo.data(), record.location.pc, 0, sprL3dRpt);

    auto dst = (__cb__ uint32_t *)(0x1000);
    auto src = (__cbuf__ uint32_t *)(0x1000);

    __sanitizer_report_img2colv2_cbuf_to_cb_b32(memInfo.data(),
        record.location.pc,
        0,
        dst,
        src,
        xm,
        xt);

    uint8_t *ptr = memInfo.data();
    ptr += sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    auto x = reinterpret_cast<Sanitizer::Load3DV2Record *>(ptr + sizeof(RecordType));
    record.location = x->location;
    ASSERT_TRUE(*x == record);
}

TEST(Load3DInstructions, load_cbuf_to_cb_3dv2_b16)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    Load3DV2Record record{};
    record.dst = 0x1000;
    record.src = 0x1000;

    record.matrixKStep = 32;
    record.matrixMStep = 32;
    record.matrixKPos = 0;
    record.matrixMPos = 0;

    record.filterWStride = 1;
    record.filterHStride = 1;
    record.filterW = 64;
    record.filterH = 64;
    record.filterWDilation = 1;
    record.filterHDilation = 1;
    record.transposeMode = 0;
    record.matrixMode = 0;
    record.fMapC = 32;

    record.fMapW = 512;
    record.fMapH = 512;
    record.fMapLeftPad = 0;
    record.fMapRightPad = 0;
    record.fMapTopPad = 0;
    record.fMapBottomPad = 0;
    
    record.matrixRptStride = 64;
    record.matrixRptTimes = 64;
    record.matrixRptMode = 0;
    record.dstStride = 32;
    record.outputMPos = 0;

    record.dataType = DataType::DATA_B16;
    record.dstMemType = MemType::L0B;
    record.srcMemType = MemType::L1;

    uint64_t xm = 0;
    uint64_t xt = 0;
    SetConfByUint<15, 0>(xm, 32);//kstep
    SetConfByUint<31, 16>(xm, 32);//mstep
    SetConfByUint<47, 32>(xm, 0);//kstart
    SetConfByUint<63, 48>(xm, 0);//mstart
    SetConfByUint<5, 0>(xt, 1);//wstride
    SetConfByUint<11, 6>(xt, 1);//hstride
    SetConfByUint<19, 12>(xt, 64);//low bit of wsize
    SetConfByUint<44, 44>(xt, 0);//high bit of wsize
    SetConfByUint<27, 20>(xt, 64);//low bit of hsize
    SetConfByUint<45, 45>(xt, 0);//high bit of hsize
    SetConfByUint<35, 28>(xt, 1);//wDilation
    SetConfByUint<43, 36>(xt, 1);//hDilation
    SetConfByUint<46, 46>(xt, 0);//Transpose Control
    SetConfByUint<47, 47>(xt, 0);//SPR Control
    SetConfByUint<63, 48>(xt, 32);//channel size N

    uint64_t sprMatrix = 0;
    uint64_t sprL3dRpt = 0;
    SetConfByUint<15, 0>(sprMatrix, 512);//W
    SetConfByUint<31, 16>(sprMatrix, 512);//H
    SetConfByUint<39, 32>(sprMatrix, 0);//left padding
    SetConfByUint<47, 40>(sprMatrix, 0);//right padding
    SetConfByUint<55, 48>(sprMatrix, 0);//top padding
    SetConfByUint<63, 56>(sprMatrix, 0);//bottom padding

    SetConfByUint<15, 0>(sprL3dRpt, 64);//repeatStride
    SetConfByUint<23, 16>(sprL3dRpt, 64);//repeatTimes
    SetConfByUint<24, 24>(sprL3dRpt, 0);//repeatMode
    SetConfByUint<47, 32>(sprL3dRpt, 32);//dstStride
    SetConfByUint<63, 48>(sprL3dRpt, 0);//mStartPosition output
    
    __sanitizer_report_set_fmatrix(memInfo.data(), record.location.pc, 0, sprMatrix);
    __sanitizer_report_set_l3d_rpt(memInfo.data(), record.location.pc, 0, sprL3dRpt);

    auto dst = (__cb__ uint16_t *)(0x1000);
    auto src = (__cbuf__ uint16_t *)(0x1000);

    __sanitizer_report_img2colv2_cbuf_to_cb_b16(memInfo.data(),
        record.location.pc,
        0,
        dst,
        src,
        xm,
        xt);

    uint8_t *ptr = memInfo.data();
    ptr += sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    auto x = reinterpret_cast<Sanitizer::Load3DV2Record *>(ptr + sizeof(RecordType));
    record.location = x->location;
    ASSERT_TRUE(*x == record);
}

TEST(Load3DInstructions, load_cbuf_to_cb_3dv2_b8)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    Load3DV2Record record{};
    record.dst = 0x1000;
    record.src = 0x1000;

    record.matrixKStep = 32;
    record.matrixMStep = 32;
    record.matrixKPos = 0;
    record.matrixMPos = 0;

    record.filterWStride = 1;
    record.filterHStride = 1;
    record.filterW = 64;
    record.filterH = 64;
    record.filterWDilation = 1;
    record.filterHDilation = 1;
    record.transposeMode = 0;
    record.matrixMode = 0;
    record.fMapC = 32;

    record.fMapW = 512;
    record.fMapH = 512;
    record.fMapLeftPad = 0;
    record.fMapRightPad = 0;
    record.fMapTopPad = 0;
    record.fMapBottomPad = 0;
    
    record.matrixRptStride = 64;
    record.matrixRptTimes = 64;
    record.matrixRptMode = 0;
    record.dstStride = 32;
    record.outputMPos = 0;

    record.dataType = DataType::DATA_B8;
    record.dstMemType = MemType::L0B;
    record.srcMemType = MemType::L1;

    uint64_t xm = 0;
    uint64_t xt = 0;
    SetConfByUint<15, 0>(xm, 32);//kstep
    SetConfByUint<31, 16>(xm, 32);//mstep
    SetConfByUint<47, 32>(xm, 0);//kstart
    SetConfByUint<63, 48>(xm, 0);//mstart
    SetConfByUint<5, 0>(xt, 1);//wstride
    SetConfByUint<11, 6>(xt, 1);//hstride
    SetConfByUint<19, 12>(xt, 64);//low bit of wsize
    SetConfByUint<44, 44>(xt, 0);//high bit of wsize
    SetConfByUint<27, 20>(xt, 64);//low bit of hsize
    SetConfByUint<45, 45>(xt, 0);//high bit of hsize
    SetConfByUint<35, 28>(xt, 1);//wDilation
    SetConfByUint<43, 36>(xt, 1);//hDilation
    SetConfByUint<46, 46>(xt, 0);//Transpose Control
    SetConfByUint<47, 47>(xt, 0);//SPR Control
    SetConfByUint<63, 48>(xt, 32);//channel size N

    uint64_t sprMatrix = 0;
    uint64_t sprL3dRpt = 0;
    SetConfByUint<15, 0>(sprMatrix, 512);//W
    SetConfByUint<31, 16>(sprMatrix, 512);//H
    SetConfByUint<39, 32>(sprMatrix, 0);//left padding
    SetConfByUint<47, 40>(sprMatrix, 0);//right padding
    SetConfByUint<55, 48>(sprMatrix, 0);//top padding
    SetConfByUint<63, 56>(sprMatrix, 0);//bottom padding

    SetConfByUint<15, 0>(sprL3dRpt, 64);//repeatStride
    SetConfByUint<23, 16>(sprL3dRpt, 64);//repeatTimes
    SetConfByUint<24, 24>(sprL3dRpt, 0);//repeatMode
    SetConfByUint<47, 32>(sprL3dRpt, 32);//dstStride
    SetConfByUint<63, 48>(sprL3dRpt, 0);//mStartPosition output
    
    __sanitizer_report_set_fmatrix(memInfo.data(), record.location.pc, 0, sprMatrix);
    __sanitizer_report_set_l3d_rpt(memInfo.data(), record.location.pc, 0, sprL3dRpt);

    auto dst = (__cb__ uint8_t *)(0x1000);
    auto src = (__cbuf__ uint8_t *)(0x1000);

    __sanitizer_report_img2colv2_cbuf_to_cb_b8(memInfo.data(),
        record.location.pc,
        0,
        dst,
        src,
        xm,
        xt);

    uint8_t *ptr = memInfo.data();
    ptr += sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    auto x = reinterpret_cast<Sanitizer::Load3DV2Record *>(ptr + sizeof(RecordType));
    record.location = x->location;
    ASSERT_TRUE(*x == record);
}
