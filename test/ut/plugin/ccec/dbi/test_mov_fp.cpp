/* -------------------------------------------------------------------------
 * This file is part of the MindStudio project.
 * Copyright (c) 2026 Huawei Technologies Co.,Ltd.
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
 ------------------------------------------------------------------------- */


#include <utility>
#include <gtest/gtest.h>

#include "../data_process.h"
#define BUILD_DYNAMIC_PROBE
#include "plugin/ccec/dbi/probes/mov_fp.cpp"

MovFpRecord CreateMovFpRecord(uint64_t dst, uint64_t src, uint16_t nSize, uint16_t mSize,
                              uint32_t dstStride, uint16_t srcStride, bool enUnitFlag,
                              uint16_t quantPreBits, bool channelSplit, bool enNZ2ND,
                              bool enNZ2DN, bool isC310)
{
    MovFpRecord record{};
    record.dst = dst;
    record.src = src;
    record.nSize = nSize;
    record.mSize = mSize;
    record.dstStride = dstStride;
    record.srcStride = srcStride;
    record.enUnitFlag = enUnitFlag;
    record.quantPreBits = quantPreBits;
    record.channelSplit = channelSplit;
    record.enNZ2ND = enNZ2ND;
    record.enNZ2DN = enNZ2DN;
    record.isC310 = isC310;
    return record;
}

TEST(MovFpInstructions, copy_matrix_cc_to_gm_f32_expect_correct_record)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    MovFpRecord record = CreateMovFpRecord(0x1000, 0x2000, 256, 512, 1024, 32,
                                           true, 32, true, false, false, false);

    __gm__ void *dst = (__gm__ void *)0x1000;
    __cc__ float *src = (__cc__ float *)0x2000;
    uint64_t xm = 0;
    uint64_t xt = 0;

    SetConfByUint<15, 4>(xm, 256); // nSize
    SetConfByUint<31, 16>(xm, 512); // mSize
    SetConfByUint<63, 32>(xm, 1024); // dstStrideDstD

    SetConfByUint<15, 0>(xt, 32); // srcStride
    SetConfByUint<33, 32>(xt, 2); // unitFlag
    SetConfByUint<38, 34>(xt, 0); // quantPRE
    SetConfByUint<42, 42>(xt, 1); // channelSplit
    SetConfByUint<43, 43>(xt, 0); // enNZ2ND

    __sanitizer_report_copy_matrix_cc_to_gm_f32(memInfo.data(), record.location.pc, 0, dst, src, xm, xt);

    uint8_t *ptr = memInfo.data();
    ptr += sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    auto x = reinterpret_cast<Sanitizer::MovFpRecord *>(ptr + sizeof(RecordType));
    record.location = x->location;
    ASSERT_TRUE(IsEqual(*x, record));
}

TEST(MovFpInstructions, copy_matrix_cc_to_gm_s32_expect_correct_record)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    MovFpRecord record = CreateMovFpRecord(0x1000, 0x2000, 128, 256, 512, 16,
                                           false, 32, false, false, false, false);

    __gm__ void *dst = (__gm__ void *)0x1000;
    __cc__ int32_t *src = (__cc__ int32_t *)0x2000;
    uint64_t xm = 0;
    uint64_t xt = 0;

    SetConfByUint<15, 4>(xm, 128); // nSize
    SetConfByUint<31, 16>(xm, 256); // mSize
    SetConfByUint<63, 32>(xm, 512); // dstStrideDstD

    SetConfByUint<15, 0>(xt, 16); // srcStride
    SetConfByUint<33, 32>(xt, 0); // unitFlag
    SetConfByUint<38, 34>(xt, 0); // quantPRE
    SetConfByUint<42, 42>(xt, 0); // channelSplit
    SetConfByUint<43, 43>(xt, 0); // enNZ2ND

    __sanitizer_report_copy_matrix_cc_to_gm_s32(memInfo.data(), record.location.pc, 0, dst, src, xm, xt);

    uint8_t *ptr = memInfo.data();
    ptr += sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    auto x = reinterpret_cast<Sanitizer::MovFpRecord *>(ptr + sizeof(RecordType));
    record.location = x->location;
    ASSERT_TRUE(IsEqual(*x, record));
}

TEST(MovFpInstructions, copy_matrix_cc_to_gm_f32_with_nz2nd_expect_correct_record)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    MovFpRecord record = CreateMovFpRecord(0x1000, 0x2000, 64, 128, 256, 8,
                                           true, 32, false, true, false, false);

    __gm__ void *dst = (__gm__ void *)0x1000;
    __cc__ float *src = (__cc__ float *)0x2000;
    uint64_t xm = 0;
    uint64_t xt = 0;

    SetConfByUint<15, 4>(xm, 64); // nSize
    SetConfByUint<31, 16>(xm, 128); // mSize
    SetConfByUint<63, 32>(xm, 256); // dstStrideDstD

    SetConfByUint<15, 0>(xt, 8); // srcStride
    SetConfByUint<33, 32>(xt, 2); // unitFlag
    SetConfByUint<38, 34>(xt, 0); // quantPRE
    SetConfByUint<42, 42>(xt, 0); // channelSplit
    SetConfByUint<43, 43>(xt, 1); // enNZ2ND

    __sanitizer_report_copy_matrix_cc_to_gm_f32(memInfo.data(), record.location.pc, 0, dst, src, xm, xt);

    uint8_t *ptr = memInfo.data();
    ptr += sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    auto x = reinterpret_cast<Sanitizer::MovFpRecord *>(ptr + sizeof(RecordType));
    record.location = x->location;
    ASSERT_TRUE(IsEqual(*x, record));
}

TEST(MovFpInstructions, copy_matrix_cc_to_gm_f32_a5_expect_correct_record)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    MovFpRecord record = CreateMovFpRecord(0x1000, 0x2000, 256, 512, 1024, 32,
                                           true, 32, true, false, false, true);

    __gm__ void *dst = (__gm__ void *)0x1000;
    __cc__ float *src = (__cc__ float *)0x2000;
    uint64_t xm = 0;
    uint64_t xt = 0;

    SetConfByUint<15, 4>(xm, 256); // nSize
    SetConfByUint<31, 16>(xm, 512); // mSize
    SetConfByUint<63, 32>(xm, 1024); // dstStride

    SetConfByUint<15, 0>(xt, 32); // srcStride
    SetConfByUint<33, 32>(xt, 2); // unitFlag
    SetConfByUint<29, 29>(xt, 0); // quantPRE bit29
    SetConfByUint<38, 34>(xt, 0); // quantPRE bits34-38
    SetConfByUint<42, 42>(xt, 1); // channelSplit
    SetConfByUint<43, 43>(xt, 0); // enNZ2ND
    SetConfByUint<62, 62>(xt, 0); // enNZ2DN

    __sanitizer_report_copy_matrix_cc_to_gm_f32_a5(memInfo.data(), record.location.pc, 0, dst, src, xm, xt);

    uint8_t *ptr = memInfo.data();
    ptr += sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    auto x = reinterpret_cast<Sanitizer::MovFpRecord *>(ptr + sizeof(RecordType));
    record.location = x->location;
    ASSERT_TRUE(IsEqual(*x, record));
}

TEST(MovFpInstructions, copy_matrix_cc_to_gm_s32_a5_expect_correct_record)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    MovFpRecord record = CreateMovFpRecord(0x1000, 0x2000, 128, 256, 512, 16,
                                           false, 32, false, false, false, true);

    __gm__ void *dst = (__gm__ void *)0x1000;
    __cc__ int32_t *src = (__cc__ int32_t *)0x2000;
    uint64_t xm = 0;
    uint64_t xt = 0;

    SetConfByUint<15, 4>(xm, 128); // nSize
    SetConfByUint<31, 16>(xm, 256); // mSize
    SetConfByUint<63, 32>(xm, 512); // dstStride

    SetConfByUint<15, 0>(xt, 16); // srcStride
    SetConfByUint<33, 32>(xt, 0); // unitFlag
    SetConfByUint<29, 29>(xt, 0); // quantPRE bit29
    SetConfByUint<38, 34>(xt, 0); // quantPRE bits34-38
    SetConfByUint<42, 42>(xt, 0); // channelSplit
    SetConfByUint<43, 43>(xt, 0); // enNZ2ND
    SetConfByUint<62, 62>(xt, 0); // enNZ2DN

    __sanitizer_report_copy_matrix_cc_to_gm_s32_a5(memInfo.data(), record.location.pc, 0, dst, src, xm, xt);

    uint8_t *ptr = memInfo.data();
    ptr += sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    auto x = reinterpret_cast<Sanitizer::MovFpRecord *>(ptr + sizeof(RecordType));
    record.location = x->location;
    ASSERT_TRUE(IsEqual(*x, record));
}

TEST(MovFpInstructions, copy_matrix_cc_to_cbuf_f32_expect_correct_record)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    MovFpRecord record = CreateMovFpRecord(0x1000, 0x2000, 64, 128, 256, 8,
                                           true, 32, false, false, false, true);

    __cbuf__ void *dst = (__cbuf__ void *)0x1000;
    __cc__ float *src = (__cc__ float *)0x2000;
    uint64_t xm = 0;
    uint64_t xt = 0;

    SetConfByUint<15, 4>(xm, 64); // nSize
    SetConfByUint<31, 16>(xm, 128); // mSize
    SetConfByUint<63, 32>(xm, 256); // dstStride

    SetConfByUint<15, 0>(xt, 8); // srcStride
    SetConfByUint<33, 32>(xt, 2); // unitFlag
    SetConfByUint<29, 29>(xt, 0); // quantPRE bit29
    SetConfByUint<38, 34>(xt, 0); // quantPRE bits34-38
    SetConfByUint<42, 42>(xt, 0); // channelSplit
    SetConfByUint<43, 43>(xt, 0); // enNZ2ND
    SetConfByUint<62, 62>(xt, 0); // enNZ2DN

    __sanitizer_report_copy_matrix_cc_to_cbuf_f32(memInfo.data(), record.location.pc, 0, dst, src, xm, xt);

    uint8_t *ptr = memInfo.data();
    ptr += sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    auto x = reinterpret_cast<Sanitizer::MovFpRecord *>(ptr + sizeof(RecordType));
    record.location = x->location;
    ASSERT_TRUE(IsEqual(*x, record));
}

TEST(MovFpInstructions, copy_matrix_cc_to_cbuf_s32_expect_correct_record)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    MovFpRecord record = CreateMovFpRecord(0x1000, 0x2000, 32, 64, 128, 4,
                                           false, 32, false, false, false, true);

    __cbuf__ void *dst = (__cbuf__ void *)0x1000;
    __cc__ int32_t *src = (__cc__ int32_t *)0x2000;
    uint64_t xm = 0;
    uint64_t xt = 0;

    SetConfByUint<15, 4>(xm, 32); // nSize
    SetConfByUint<31, 16>(xm, 64); // mSize
    SetConfByUint<63, 32>(xm, 128); // dstStride

    SetConfByUint<15, 0>(xt, 4); // srcStride
    SetConfByUint<33, 32>(xt, 0); // unitFlag
    SetConfByUint<29, 29>(xt, 0); // quantPRE bit29
    SetConfByUint<38, 34>(xt, 0); // quantPRE bits34-38
    SetConfByUint<42, 42>(xt, 0); // channelSplit
    SetConfByUint<43, 43>(xt, 0); // enNZ2ND
    SetConfByUint<62, 62>(xt, 0); // enNZ2DN

    __sanitizer_report_copy_matrix_cc_to_cbuf_s32(memInfo.data(), record.location.pc, 0, dst, src, xm, xt);

    uint8_t *ptr = memInfo.data();
    ptr += sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    auto x = reinterpret_cast<Sanitizer::MovFpRecord *>(ptr + sizeof(RecordType));
    record.location = x->location;
    ASSERT_TRUE(IsEqual(*x, record));
}

TEST(MovFpInstructions, copy_matrix_cc_to_ubuf_f32_expect_correct_record)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    MovFpRecord record = CreateMovFpRecord(0x1000, 0x2000, 16, 32, 64, 2,
                                           true, 32, false, false, false, true);

    __ubuf__ void *dst = (__ubuf__ void *)0x1000;
    __cc__ float *src = (__cc__ float *)0x2000;
    uint64_t xm = 0;
    uint64_t xt = 0;

    SetConfByUint<15, 4>(xm, 16); // nSize
    SetConfByUint<31, 16>(xm, 32); // mSize
    SetConfByUint<63, 32>(xm, 64); // dstStride

    SetConfByUint<15, 0>(xt, 2); // srcStride
    SetConfByUint<33, 32>(xt, 2); // unitFlag
    SetConfByUint<29, 29>(xt, 0); // quantPRE bit29
    SetConfByUint<38, 34>(xt, 0); // quantPRE bits34-38
    SetConfByUint<42, 42>(xt, 0); // channelSplit
    SetConfByUint<43, 43>(xt, 0); // enNZ2ND
    SetConfByUint<62, 62>(xt, 0); // enNZ2DN

    __sanitizer_report_copy_matrix_cc_to_ubuf_f32(memInfo.data(), record.location.pc, 0, dst, src, xm, xt);

    uint8_t *ptr = memInfo.data();
    ptr += sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    auto x = reinterpret_cast<Sanitizer::MovFpRecord *>(ptr + sizeof(RecordType));
    record.location = x->location;
    ASSERT_TRUE(IsEqual(*x, record));
}

TEST(MovFpInstructions, copy_matrix_cc_to_ubuf_s32_expect_correct_record)
{
    std::vector<uint8_t> memInfo = CreateMemInfo();
    MovFpRecord record = CreateMovFpRecord(0x1000, 0x2000, 8, 16, 32, 1,
                                           false, 32, false, false, false, true);

    __ubuf__ void *dst = (__ubuf__ void *)0x1000;
    __cc__ int32_t *src = (__cc__ int32_t *)0x2000;
    uint64_t xm = 0;
    uint64_t xt = 0;

    SetConfByUint<15, 4>(xm, 8); // nSize
    SetConfByUint<31, 16>(xm, 16); // mSize
    SetConfByUint<63, 32>(xm, 32); // dstStride

    SetConfByUint<15, 0>(xt, 1); // srcStride
    SetConfByUint<33, 32>(xt, 0); // unitFlag
    SetConfByUint<29, 29>(xt, 0); // quantPRE bit29
    SetConfByUint<38, 34>(xt, 0); // quantPRE bits34-38
    SetConfByUint<42, 42>(xt, 0); // channelSplit
    SetConfByUint<43, 43>(xt, 0); // enNZ2ND
    SetConfByUint<62, 62>(xt, 0); // enNZ2DN

    __sanitizer_report_copy_matrix_cc_to_ubuf_s32(memInfo.data(), record.location.pc, 0, dst, src, xm, xt);

    uint8_t *ptr = memInfo.data();
    ptr += sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    auto x = reinterpret_cast<Sanitizer::MovFpRecord *>(ptr + sizeof(RecordType));
    record.location = x->location;
    ASSERT_TRUE(IsEqual(*x, record));
}
