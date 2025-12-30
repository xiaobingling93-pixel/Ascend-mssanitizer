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
#include "plugin/ccec/calc_instructions/mad.cpp"

using namespace Sanitizer;

struct MadRecord {
    uint64_t dst;
    uint64_t src0;
    uint64_t src1;
    Location location;
    uint16_t m;
    uint16_t k;
    uint16_t n;
};
constexpr uint16_t MADRECORD_M = 54;
constexpr uint16_t MADRECORD_K = 43;
constexpr uint16_t MADRECORD_N = 70;
static MadRecord CreateMadRecord()
{
    MadRecord madRecord;
    madRecord.dst = RandInt(0, 0x1000000);
    madRecord.src0 = RandInt(0, 0x1000000);
    madRecord.src1 = RandInt(0, 0x1000000);
    madRecord.location.fileNo = RandInt(0, 0x100000);
    madRecord.location.lineNo = RandInt(0, 0x1000);
    madRecord.m = MADRECORD_M;
    madRecord.k = MADRECORD_K;
    madRecord.n = MADRECORD_N;
    return madRecord;
}

TEST(MadCalcInstructions, dump_mad_part1_records_and_parse_last_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    __gm__  RecordBlockHead* recordBlockHead = reinterpret_cast<__gm__ RecordBlockHead *>(memInfo.data() + sizeof(RecordGlobalHead));
    MadRecord madRecord = CreateMadRecord();

    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ float *>(madRecord.dst), reinterpret_cast<__ca__ __bf16 *>(madRecord.src0),
        reinterpret_cast<__ca__ __bf16 *>(madRecord.src1), 42342342);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ half *>(madRecord.dst), reinterpret_cast<__ca__ half *>(madRecord.src0),
        reinterpret_cast<__ca__ half *>(madRecord.src1), 42342342);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ float *>(madRecord.dst), reinterpret_cast<__ca__ __bf16 *>(madRecord.src0),
        reinterpret_cast<__ca__ __bf16 *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 0, 0, 0);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ float *>(madRecord.dst), reinterpret_cast<__ca__ __bf16 *>(madRecord.src0),
        reinterpret_cast<__ca__ __bf16 *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 0, 0, 1, 0, 1, 0);
    MarixMulOpRecord const *mtrixOp = reinterpret_cast<MarixMulOpRecord const*>(memInfo.data() + sizeof(RecordGlobalHead) +
        sizeof(RecordBlockHead) + sizeof(RecordType) * 4 + sizeof(MarixMulOpRecord) * 3);
    ASSERT_EQ(recordBlockHead->recordCount, 4);
    ASSERT_EQ(mtrixOp->dstBlockNum, 20);
    ASSERT_EQ(mtrixOp->dstBlockSize, 1024);
    uint64_t expectDst = madRecord.dst & 0xffffffff;
    ASSERT_EQ(mtrixOp->dstAlignSize, 1024);
    ASSERT_EQ(mtrixOp->dst, expectDst);
    ASSERT_EQ(mtrixOp->src0BlockNum, 3);
    ASSERT_EQ(mtrixOp->src0BlockSize, 512);
    ASSERT_EQ(mtrixOp->src1BlockNum, 15);
    ASSERT_EQ(mtrixOp->src1BlockSize, 512);
    ASSERT_EQ(mtrixOp->src0Repeat, 4);
    ASSERT_EQ(mtrixOp->src0RepeatStride, 3);
}

TEST(MadCalcInstructions, dump_mad_part2_records_and_parse_last_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    __gm__  RecordBlockHead* recordBlockHead = reinterpret_cast<__gm__ RecordBlockHead *>(memInfo.data() + sizeof(RecordGlobalHead));
    MadRecord madRecord = CreateMadRecord();
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ half *>(madRecord.dst), reinterpret_cast<__ca__ half *>(madRecord.src0),
        reinterpret_cast<__ca__ half *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ half *>(madRecord.dst), reinterpret_cast<__ca__ half *>(madRecord.src0),
        reinterpret_cast<__ca__ half *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 0, 0, 0, 0, 0, 0);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ half *>(madRecord.dst), reinterpret_cast<__ca__ half *>(madRecord.src0),
        reinterpret_cast<__ca__ half *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 0, 1, 0);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ float *>(madRecord.dst), reinterpret_cast<__ca__ half *>(madRecord.src0),
        reinterpret_cast<__ca__ half *>(madRecord.src1), 42342342);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ float *>(madRecord.dst), reinterpret_cast<__ca__ half *>(madRecord.src0),
        reinterpret_cast<__ca__ half *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ float *>(madRecord.dst), reinterpret_cast<__ca__ half *>(madRecord.src0),
        reinterpret_cast<__ca__ half *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 1, 0, 1);
    MarixMulOpRecord const *mtrixOp = reinterpret_cast<MarixMulOpRecord const*>(memInfo.data() + sizeof(RecordGlobalHead) +
        sizeof(RecordBlockHead) + sizeof(RecordType) * 6 + sizeof(MarixMulOpRecord) * 5);
    ASSERT_EQ(recordBlockHead->recordCount, 6);
    ASSERT_EQ(mtrixOp->dstBlockNum, 20);
    ASSERT_EQ(mtrixOp->dstBlockSize, 1024);
    uint64_t expectDst = madRecord.dst & 0xffffffff;
    ASSERT_EQ(mtrixOp->dstAlignSize, 1024);
    ASSERT_EQ(mtrixOp->dst, expectDst);
    ASSERT_EQ(mtrixOp->src0BlockNum, 3);
    ASSERT_EQ(mtrixOp->src0BlockSize, 512);
    ASSERT_EQ(mtrixOp->src1BlockNum, 15);
    ASSERT_EQ(mtrixOp->src1BlockSize, 512);
    ASSERT_EQ(mtrixOp->src0Repeat, 4);
    ASSERT_EQ(mtrixOp->src0RepeatStride, 3);
}

TEST(MadCalcInstructions, dump_mad_part3_records_and_parse_last_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    __gm__  RecordBlockHead* recordBlockHead = reinterpret_cast<__gm__ RecordBlockHead *>(memInfo.data() + sizeof(RecordGlobalHead));
    MadRecord madRecord = CreateMadRecord();

#if defined(__DAV_C220__) || defined(__DAV_C220_CUBE__)
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ float *>(madRecord.dst), reinterpret_cast<__ca__ half *>(madRecord.src0),
        reinterpret_cast<__ca__ half *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 0, 0, 1, 0, 1, 0);
#else
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ float *>(madRecord.dst), reinterpret_cast<__ca__ half *>(madRecord.src0),
        reinterpret_cast<__ca__ half *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 0, 0, 1, 0, 1, 0);
#endif
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ float *>(madRecord.dst), reinterpret_cast<__ca__ float *>(madRecord.src0),
        reinterpret_cast<__ca__ float *>(madRecord.src1), 42342342);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ float *>(madRecord.dst), reinterpret_cast<__ca__ float *>(madRecord.src0),
        reinterpret_cast<__ca__ float *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 1, 0, 1);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ float *>(madRecord.dst), reinterpret_cast<__ca__ float *>(madRecord.src0),
        reinterpret_cast<__ca__ float *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 0, 0, 1, 0, 1, 0);
    MarixMulOpRecord const *mtrixOp = reinterpret_cast<MarixMulOpRecord const*>(memInfo.data() + sizeof(RecordGlobalHead) +
        sizeof(RecordBlockHead) + sizeof(RecordType) * 4 + sizeof(MarixMulOpRecord) * 3);
    ASSERT_EQ(recordBlockHead->recordCount, 4);
    ASSERT_EQ(mtrixOp->dstBlockNum, 20);
    ASSERT_EQ(mtrixOp->dstBlockSize, 1024);
    uint64_t expectDst = madRecord.dst & 0xffffffff;
    ASSERT_EQ(mtrixOp->dstAlignSize, 512);
    ASSERT_EQ(mtrixOp->dst, expectDst);
    ASSERT_EQ(mtrixOp->src0BlockNum, 6);
    ASSERT_EQ(mtrixOp->src0BlockSize, 512);
    ASSERT_EQ(mtrixOp->src1BlockNum, 30);
    ASSERT_EQ(mtrixOp->src1BlockSize, 512);
    ASSERT_EQ(mtrixOp->src0Repeat, 4);
    ASSERT_EQ(mtrixOp->src0RepeatStride, 6);
}

TEST(MadCalcInstructions, dump_mad_part4_records_and_parse_last_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    __gm__  RecordBlockHead* recordBlockHead = reinterpret_cast<__gm__ RecordBlockHead *>(memInfo.data() + sizeof(RecordGlobalHead));
    MadRecord madRecord = CreateMadRecord();
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ int32_t *>(madRecord.dst), reinterpret_cast<__ca__ int8_t *>(madRecord.src0),
        reinterpret_cast<__ca__ int8_t *>(madRecord.src1), 42342343);
#if defined(__DAV_C220__) || defined(__DAV_C220_CUBE__)
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ int32_t *>(madRecord.dst), reinterpret_cast<__ca__ int8_t *>(madRecord.src0),
        reinterpret_cast<__ca__ int8_t *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 0, 0, 0, 0, 0, 0);
#else
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ int32_t *>(madRecord.dst), reinterpret_cast<__ca__ int8_t *>(madRecord.src0),
        reinterpret_cast<__ca__ int8_t *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 0, 0, 0, 0, 0, 0);
#endif
    MarixMulOpRecord const *mtrixOp = reinterpret_cast<MarixMulOpRecord const*>(memInfo.data() + sizeof(RecordGlobalHead) +
        sizeof(RecordBlockHead) + sizeof(RecordType) * 2 + sizeof(MarixMulOpRecord) * 1);
    ASSERT_EQ(recordBlockHead->recordCount, 2);
    ASSERT_EQ(mtrixOp->dstBlockNum, 20);
    ASSERT_EQ(mtrixOp->dstBlockSize, 1024);
    uint64_t expectDst = madRecord.dst & 0xffffffff;
    ASSERT_EQ(mtrixOp->dstAlignSize, 1024);
    ASSERT_EQ(mtrixOp->dst, expectDst);
    ASSERT_EQ(mtrixOp->src0BlockNum, 2);
    ASSERT_EQ(mtrixOp->src0BlockSize, 512);
    ASSERT_EQ(mtrixOp->src1BlockNum, 10);
    ASSERT_EQ(mtrixOp->src1BlockSize, 512);
    ASSERT_EQ(mtrixOp->src0Repeat, 4);
    ASSERT_EQ(mtrixOp->src0RepeatStride, 2);
}

TEST(MadCalcInstructions, dump_mad_part5_records_and_parse_last_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    __gm__  RecordBlockHead* recordBlockHead = reinterpret_cast<__gm__ RecordBlockHead *>(memInfo.data() + sizeof(RecordGlobalHead));
    MadRecord madRecord = CreateMadRecord();

    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ int32_t *>(madRecord.dst), reinterpret_cast<__ca__ int16_t *>(madRecord.src0),
        reinterpret_cast<__ca__ int8_t *>(madRecord.src1), 42342342);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ int32_t *>(madRecord.dst), reinterpret_cast<__ca__ int16_t *>(madRecord.src0),
        reinterpret_cast<__ca__ int8_t *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, UintFlag::UF_MODE0,
        0, 0);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ int32_t *>(madRecord.dst), reinterpret_cast<__ca__ int16_t *>(madRecord.src0),
        reinterpret_cast<__ca__ int8_t *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 0, 0, 0, 1, 0);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ int32_t *>(madRecord.dst), reinterpret_cast<__ca__ int8_t *>(madRecord.src0),
        reinterpret_cast<__ca__ int8_t *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ int32_t *>(madRecord.dst), reinterpret_cast<__ca__ int8_t *>(madRecord.src0),
        reinterpret_cast<__ca__ int8_t *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 1, 1, 0);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ int32_t *>(madRecord.dst), reinterpret_cast<__ca__ int8_t *>(madRecord.src0),
        reinterpret_cast<__ca__ int8_t *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 0, 0, 0, 1, 0);
    MarixMulOpRecord const *mtrixOp = reinterpret_cast<MarixMulOpRecord const*>(memInfo.data() + sizeof(RecordGlobalHead) +
        sizeof(RecordBlockHead) + sizeof(RecordType) * 6 + sizeof(MarixMulOpRecord) * 5);
    ASSERT_EQ(recordBlockHead->recordCount, 6);
    ASSERT_EQ(mtrixOp->dstBlockNum, 20);
    ASSERT_EQ(mtrixOp->dstBlockSize, 1024);
    uint64_t expectDst = madRecord.dst & 0xffffffff;
    ASSERT_EQ(mtrixOp->dstAlignSize, 1024);
    ASSERT_EQ(mtrixOp->dst, expectDst);
    ASSERT_EQ(mtrixOp->src0BlockNum, 2);
    ASSERT_EQ(mtrixOp->src0BlockSize, 512);
    ASSERT_EQ(mtrixOp->src1BlockNum, 10);
    ASSERT_EQ(mtrixOp->src1BlockSize, 512);
    ASSERT_EQ(mtrixOp->src0Repeat, 4);
    ASSERT_EQ(mtrixOp->src0RepeatStride, 2);
}

TEST(MadCalcInstructions, dump_mad_part6_records_and_parse_last_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    __gm__  RecordBlockHead* recordBlockHead = reinterpret_cast<__gm__ RecordBlockHead *>(memInfo.data() + sizeof(RecordGlobalHead));
    MadRecord madRecord = CreateMadRecord();

    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ uint32_t *>(madRecord.dst), reinterpret_cast<__ca__ uint8_t *>(madRecord.src0),
        reinterpret_cast<__ca__ uint8_t *>(madRecord.src1), 42342342);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ uint32_t *>(madRecord.dst), reinterpret_cast<__ca__ uint8_t *>(madRecord.src0),
        reinterpret_cast<__ca__ uint8_t *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ uint32_t *>(madRecord.dst), reinterpret_cast<__ca__ uint8_t *>(madRecord.src0),
        reinterpret_cast<__ca__ uint8_t *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 0, 0, 0, 0, 0, 0);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ uint32_t *>(madRecord.dst), reinterpret_cast<__ca__ uint8_t *>(madRecord.src0),
        reinterpret_cast<__ca__ uint8_t *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 0, 0, 0, 1, 0);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ int32_t *>(madRecord.dst), reinterpret_cast<__ca__ uint8_t *>(madRecord.src0),
        reinterpret_cast<__ca__ uint8_t *>(madRecord.src1), 42342342);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ int32_t *>(madRecord.dst), reinterpret_cast<__ca__ uint8_t *>(madRecord.src0),
        reinterpret_cast<__ca__ uint8_t *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0);
    
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ int32_t *>(madRecord.dst), reinterpret_cast<__ca__ uint8_t *>(madRecord.src0),
        reinterpret_cast<__ca__ uint8_t *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 1, 0, 0);
    MarixMulOpRecord const *mtrixOp = reinterpret_cast<MarixMulOpRecord const*>(memInfo.data() + sizeof(RecordGlobalHead) +
        sizeof(RecordBlockHead) + sizeof(RecordType) * 7 + sizeof(MarixMulOpRecord) * 6);
    ASSERT_EQ(recordBlockHead->recordCount, 7);
    ASSERT_EQ(mtrixOp->dstBlockNum, 20);
    ASSERT_EQ(mtrixOp->dstBlockSize, 1024);
    uint64_t expectDst = madRecord.dst & 0xffffffff;
    ASSERT_EQ(mtrixOp->dstAlignSize, 1024);
    ASSERT_EQ(mtrixOp->dst, expectDst);
    ASSERT_EQ(mtrixOp->src0BlockNum, 2);
    ASSERT_EQ(mtrixOp->src0BlockSize, 512);
    ASSERT_EQ(mtrixOp->src1BlockNum, 10);
    ASSERT_EQ(mtrixOp->src1BlockSize, 512);
    ASSERT_EQ(mtrixOp->src0Repeat, 4);
    ASSERT_EQ(mtrixOp->src0RepeatStride, 2);
}

TEST(MadCalcInstructions, dump_mad_part7_records_and_parse_last_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    __gm__  RecordBlockHead* recordBlockHead = reinterpret_cast<__gm__ RecordBlockHead *>(memInfo.data() + sizeof(RecordGlobalHead));
    MadRecord madRecord = CreateMadRecord();
#if defined(__DAV_C220__) || defined(__DAV_C220_CUBE__)
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ int32_t *>(madRecord.dst), reinterpret_cast<__ca__ uint8_t *>(madRecord.src0),
        reinterpret_cast<__ca__ uint8_t *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 0, 0, 0, 0, 0, 0);
#else
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ int32_t *>(madRecord.dst), reinterpret_cast<__ca__ uint8_t *>(madRecord.src0),
        reinterpret_cast<__ca__ uint8_t *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 0, 0, 0, 0, 0, 0);
#endif
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ int32_t *>(madRecord.dst), reinterpret_cast<__ca__ uint8_t *>(madRecord.src0),
        reinterpret_cast<__ca__ uint8_t *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 0, 0, 0, 1, 0);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ int32_t *>(madRecord.dst), reinterpret_cast<__ca__ uint8_t *>(madRecord.src0),
        reinterpret_cast<__ca__ int8_t *>(madRecord.src1), 42342342);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ int32_t *>(madRecord.dst), reinterpret_cast<__ca__ uint8_t *>(madRecord.src0),
        reinterpret_cast<__ca__ int8_t *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ int32_t *>(madRecord.dst), reinterpret_cast<__ca__ uint8_t *>(madRecord.src0),
        reinterpret_cast<__ca__ int8_t *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 0, 0, 0, 0, 0, 0);
    __sanitizer_report_mad(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo, madRecord.location.pc,
        reinterpret_cast<__cc__ int32_t *>(madRecord.dst), reinterpret_cast<__ca__ uint8_t *>(madRecord.src0),
        reinterpret_cast<__ca__ int8_t *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 0, 0, 0, 1, 0);
    MarixMulOpRecord const *mtrixOp = reinterpret_cast<MarixMulOpRecord const*>(memInfo.data() + sizeof(RecordGlobalHead) +
        sizeof(RecordBlockHead) + sizeof(RecordType) * 6 + sizeof(MarixMulOpRecord) * 5);
    ASSERT_EQ(recordBlockHead->recordCount, 6);
    ASSERT_EQ(mtrixOp->dstBlockNum, 20);
    ASSERT_EQ(mtrixOp->dstBlockSize, 1024);
    uint64_t expectDst = madRecord.dst & 0xffffffff;
    ASSERT_EQ(mtrixOp->dstAlignSize, 1024);
    ASSERT_EQ(mtrixOp->dst, expectDst);
    ASSERT_EQ(mtrixOp->src0BlockNum, 2);
    ASSERT_EQ(mtrixOp->src0BlockSize, 512);
    ASSERT_EQ(mtrixOp->src1BlockNum, 10);
    ASSERT_EQ(mtrixOp->src1BlockSize, 512);
    ASSERT_EQ(mtrixOp->src0Repeat, 4);
    ASSERT_EQ(mtrixOp->src0RepeatStride, 2);
}

TEST(MadCalcInstructions, dump_mad_part8_records_and_parse_last_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    __gm__  RecordBlockHead* recordBlockHead = reinterpret_cast<__gm__ RecordBlockHead *>(memInfo.data() + sizeof(RecordGlobalHead));
    MadRecord madRecord = CreateMadRecord();

    __sanitizer_report_mad_b8u2(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo,
        madRecord.location.pc, reinterpret_cast<__cc__ int32_t *>(madRecord.dst),
        reinterpret_cast<__ca__ int8_t *>(madRecord.src0), reinterpret_cast<__ca__ void *>(madRecord.src1), 42342342);
    __sanitizer_report_mad_b8u2(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo,
        madRecord.location.pc, reinterpret_cast<__cc__ int32_t *>(madRecord.dst),
        reinterpret_cast<__ca__ int8_t *>(madRecord.src0), reinterpret_cast<__ca__ void *>(madRecord.src1),
        madRecord.m, madRecord.k, madRecord.n, 0);
    __sanitizer_report_mad_b8u2(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo,
        madRecord.location.pc, reinterpret_cast<__cc__ int32_t *>(madRecord.dst),
        reinterpret_cast<__ca__ int8_t *>(madRecord.src0), reinterpret_cast<__ca__ void *>(madRecord.src1),
        madRecord.m, madRecord.k, madRecord.n, 0, 0, 0, 0, 0, 0, 0);
    __sanitizer_report_mad_b8u2(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo,
        madRecord.location.pc, reinterpret_cast<__cc__ int32_t *>(madRecord.dst),
        reinterpret_cast<__ca__ uint8_t *>(madRecord.src0), reinterpret_cast<__ca__ void *>(madRecord.src1), 42342342);
    __sanitizer_report_mad_b8u2(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo,
        madRecord.location.pc, reinterpret_cast<__cc__ int32_t *>(madRecord.dst),
        reinterpret_cast<__ca__ uint8_t *>(madRecord.src0), reinterpret_cast<__ca__ void *>(madRecord.src1),
        madRecord.m, madRecord.k, madRecord.n, 0);
    __sanitizer_report_mad_b8u2(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo,
        madRecord.location.pc, reinterpret_cast<__cc__ int32_t *>(madRecord.dst),
        reinterpret_cast<__ca__ uint8_t *>(madRecord.src0), reinterpret_cast<__ca__ void *>(madRecord.src1),
        madRecord.m, madRecord.k, madRecord.n, 0, 0, 0, 0, 0, 0, 0);
    __sanitizer_report_mad_f16u2(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo,
        madRecord.location.pc, reinterpret_cast<__cc__ half *>(madRecord.dst),
        reinterpret_cast<__ca__ half *>(madRecord.src0), reinterpret_cast<__ca__ void *>(madRecord.src1), 42342342);
    __sanitizer_report_mad_f16u2(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo,
        madRecord.location.pc, reinterpret_cast<__cc__ half *>(madRecord.dst),
        reinterpret_cast<__ca__ half *>(madRecord.src0), reinterpret_cast<__ca__ void *>(madRecord.src1),
        madRecord.m, madRecord.k, madRecord.n, 0);
    __sanitizer_report_mad_f16u2(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo,
        madRecord.location.pc, reinterpret_cast<__cc__ half *>(madRecord.dst),
        reinterpret_cast<__ca__ half *>(madRecord.src0), reinterpret_cast<__ca__ void *>(madRecord.src1),
        madRecord.m, madRecord.k, madRecord.n, 0, 0, 0, 0, 0, 0, 0);

    MarixMulOpRecord const *mtrixOp = reinterpret_cast<MarixMulOpRecord const*>(memInfo.data() + sizeof(RecordGlobalHead) +
        sizeof(RecordBlockHead) + sizeof(RecordType) * 9 + sizeof(MarixMulOpRecord) * 8);
    ASSERT_EQ(recordBlockHead->recordCount, 9);
    ASSERT_EQ(mtrixOp->dstBlockNum, 20);
    ASSERT_EQ(mtrixOp->dstBlockSize, 512);
    uint64_t expectDst = madRecord.dst & 0xffffffff;
    ASSERT_EQ(mtrixOp->dstAlignSize, 512);
    ASSERT_EQ(mtrixOp->dst, expectDst);
    ASSERT_EQ(mtrixOp->src0BlockNum, 3);
    ASSERT_EQ(mtrixOp->src0BlockSize, 512);
    ASSERT_EQ(mtrixOp->src1BlockNum, 15);
    ASSERT_EQ(mtrixOp->src1BlockSize, 64);
    ASSERT_EQ(mtrixOp->src0Repeat, 4);
    ASSERT_EQ(mtrixOp->src0RepeatStride, 3);
}

TEST(MadCalcInstructions, dump_mad_part9_records_and_parse_last_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    __gm__  RecordBlockHead* recordBlockHead = reinterpret_cast<__gm__ RecordBlockHead *>(memInfo.data() + sizeof(RecordGlobalHead));
    MadRecord madRecord = CreateMadRecord();

    __sanitizer_report_mad_s4(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo,
        madRecord.location.pc, reinterpret_cast<__cc__ int32_t *>(madRecord.dst),
        reinterpret_cast<__ca__ void *>(madRecord.src0), reinterpret_cast<__ca__ void *>(madRecord.src1), 42342342);
    __sanitizer_report_mad_s4(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo,
        madRecord.location.pc, reinterpret_cast<__cc__ int32_t *>(madRecord.dst),
        reinterpret_cast<__ca__ void *>(madRecord.src0), reinterpret_cast<__ca__ void *>(madRecord.src1),
        madRecord.m, madRecord.k, madRecord.n, 0);

#if defined(__DAV_C220__) || defined(__DAV_C220_CUBE__)
    __sanitizer_report_mad_s4(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo,
        madRecord.location.pc, reinterpret_cast<__cc__ int32_t *>(madRecord.dst),
        reinterpret_cast<__ca__ void *>(madRecord.src0), reinterpret_cast<__ca__ void *>(madRecord.src1),
        madRecord.m, madRecord.k, madRecord.n, 0, 1, 1, 0);
    __sanitizer_report_mad_s4(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo,
        madRecord.location.pc, reinterpret_cast<__cc__ int32_t *>(madRecord.dst),
        reinterpret_cast<__ca__ void *>(madRecord.src0), reinterpret_cast<__ca__ void *>(madRecord.src1),
        madRecord.m, madRecord.k, madRecord.n, 0, 0, 0, 0, 0, 0, 0);
#else
    __sanitizer_report_mad_s4(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo,
        madRecord.location.pc, reinterpret_cast<__cc__ int32_t *>(madRecord.dst),
        reinterpret_cast<__ca__ void *>(madRecord.src0), reinterpret_cast<__ca__ void *>(madRecord.src1),
        madRecord.m, madRecord.k, madRecord.n, 0, 1, 1, 0);
    __sanitizer_report_mad_s4(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo,
        madRecord.location.pc, reinterpret_cast<__cc__ int32_t *>(madRecord.dst),
        reinterpret_cast<__ca__ void *>(madRecord.src0), reinterpret_cast<__ca__ void *>(madRecord.src1),
        madRecord.m, madRecord.k, madRecord.n, 0, 0, 0, 0, 0, 0, 0);
#endif
    MarixMulOpRecord const *mtrixOp = reinterpret_cast<MarixMulOpRecord const*>(memInfo.data() + sizeof(RecordGlobalHead) +
        sizeof(RecordBlockHead) + sizeof(RecordType) * 4 + sizeof(MarixMulOpRecord) * 3);
    ASSERT_EQ(recordBlockHead->recordCount, 4);
    ASSERT_EQ(mtrixOp->dstBlockNum, 20);
    ASSERT_EQ(mtrixOp->dstBlockSize, 1024);
    uint64_t expectDst = madRecord.dst & 0xffffffff;
    ASSERT_EQ(mtrixOp->dstAlignSize, 1024);
    ASSERT_EQ(mtrixOp->dst, expectDst);
    ASSERT_EQ(mtrixOp->src0BlockNum, 1);
    ASSERT_EQ(mtrixOp->src0BlockSize, 512);
    ASSERT_EQ(mtrixOp->src1BlockNum, 5);
    ASSERT_EQ(mtrixOp->src1BlockSize, 512);
    ASSERT_EQ(mtrixOp->src0Repeat, 4);
    ASSERT_EQ(mtrixOp->src0RepeatStride, 1);
}

TEST(MadCalcInstructions, dump_mad_part10_records_and_parse_last_expect_success)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    __gm__  RecordBlockHead* recordBlockHead = reinterpret_cast<__gm__ RecordBlockHead *>(memInfo.data() + sizeof(RecordGlobalHead));
    MadRecord madRecord = CreateMadRecord();

    __sanitizer_report_mad_s8s4(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo,
        madRecord.location.pc, reinterpret_cast<__cc__ int32_t *>(madRecord.dst),
        reinterpret_cast<__ca__ int8_t *>(madRecord.src0), reinterpret_cast<__ca__ void *>(madRecord.src1), 42342342);
    __sanitizer_report_mad_s8s4(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo,
        madRecord.location.pc, reinterpret_cast<__cc__ int32_t *>(madRecord.dst),
        reinterpret_cast<__ca__ int8_t *>(madRecord.src0), reinterpret_cast<__ca__ void *>(madRecord.src1),
        madRecord.m, madRecord.k, madRecord.n, 0, 0, 1, 0);
    __sanitizer_report_mad_sp(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo,
        madRecord.location.pc, reinterpret_cast<__cc__ int32_t *>(madRecord.dst),
        reinterpret_cast<__ca__ int8_t *>(madRecord.src0), reinterpret_cast<__ca__ int8_t *>(madRecord.src1), 42342342);
    __sanitizer_report_mad_sp(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo,
        madRecord.location.pc, reinterpret_cast<__cc__ int32_t *>(madRecord.dst),
        reinterpret_cast<__ca__ int8_t *>(madRecord.src0), reinterpret_cast<__ca__ int8_t *>(madRecord.src1),
        madRecord.m, madRecord.k, madRecord.n, 0, 1, 0);

    __sanitizer_report_mad_tf322f32(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo,
        madRecord.location.pc,
        reinterpret_cast<__cc__ float *>(madRecord.dst), reinterpret_cast<__ca__ float *>(madRecord.src0),
        reinterpret_cast<__ca__ float *>(madRecord.src1), 42342342);
    __sanitizer_report_mad_tf322f32(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo,
        madRecord.location.pc,
        reinterpret_cast<__cc__ float *>(madRecord.dst), reinterpret_cast<__ca__ float *>(madRecord.src0),
        reinterpret_cast<__ca__ float *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 1, 1, 0);
    __sanitizer_report_mad_tf322f32(memInfo.data(), madRecord.location.fileNo, madRecord.location.lineNo,
        madRecord.location.pc,
        reinterpret_cast<__cc__ float *>(madRecord.dst), reinterpret_cast<__ca__ float *>(madRecord.src0),
        reinterpret_cast<__ca__ float *>(madRecord.src1), madRecord.m, madRecord.k, madRecord.n, 0, 0, 0, 1, 0, 1, 0);
    MarixMulOpRecord const *mtrixOp = reinterpret_cast<MarixMulOpRecord const*>(memInfo.data() + sizeof(RecordGlobalHead) +
        sizeof(RecordBlockHead) + sizeof(RecordType) * 7 + sizeof(MarixMulOpRecord) * 6);
    ASSERT_EQ(recordBlockHead->recordCount, 7);
    ASSERT_EQ(mtrixOp->dstBlockNum, 20);
    ASSERT_EQ(mtrixOp->dstBlockSize, 1024);
    ASSERT_EQ(mtrixOp->src0BlockNum, 6);
    ASSERT_EQ(mtrixOp->src0BlockSize, 512);
    ASSERT_EQ(mtrixOp->src1BlockNum, 30);
    ASSERT_EQ(mtrixOp->src1BlockSize, 512);
    ASSERT_EQ(mtrixOp->src0Repeat, 4);
    ASSERT_EQ(mtrixOp->src0RepeatStride, 6);
}
