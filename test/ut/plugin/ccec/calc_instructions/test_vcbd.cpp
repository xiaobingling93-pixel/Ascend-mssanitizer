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


#include <vector>
#include <gtest/gtest.h>
#include "ccec_defs.h"
#include "ccec/data_process.h"
#include "plugin/ccec/calc_instructions/vcbd.cpp"
 
using namespace Sanitizer;

void AssertUnaryOPRecord(UnaryOpRecord* unaryOp, UnaryOpRecord& record0)
{
    ASSERT_EQ(unaryOp->dst, record0.dst);
    ASSERT_EQ(unaryOp->src, record0.src);
    ASSERT_EQ(unaryOp->location.fileNo, record0.location.fileNo);
    ASSERT_EQ(unaryOp->location.lineNo, record0.location.lineNo);
    ASSERT_EQ(unaryOp->repeat, record0.repeat);
    ASSERT_EQ(unaryOp->dstBlockStride, record0.dstBlockStride);
    ASSERT_EQ(unaryOp->srcBlockStride, record0.srcBlockStride);
    ASSERT_EQ(unaryOp->dstRepeatStride, record0.dstRepeatStride);
    ASSERT_EQ(unaryOp->srcRepeatStride, record0.srcRepeatStride);
}
 
// 定义宏函数用于减少手动生成重复的代码
#define CALL_VCBD(cvtType, dstType, srcType) { \
    __sanitizer_report_vcbd_##cvtType(memInfo, record.location.fileNo, record.location.lineNo, \
    record.location.lineNo, reinterpret_cast<__ubuf__ dstType*>(record.dst), \
         reinterpret_cast<__ubuf__ srcType*>(record.src), 1); \
\
    __sanitizer_report_vcbd_##cvtType(memInfo, record.location.fileNo, record.location.lineNo, record.location.lineNo, \
        reinterpret_cast<__ubuf__ dstType*>(record.dst), reinterpret_cast<__ubuf__ srcType*>(record.src), \
        record.repeat, record.dstBlockStride, record.srcBlockStride, \
        record.dstRepeatStride, record.srcRepeatStride, true, true); \
}

#define VCBD_TEST_CASE_DEFINE(cvtType, dstType, srcType) \
TEST(VcbdCalcInstructions, dump_##cvtType##_type_records_and_parse_last_expect_success) { \
    std::vector<uint8_t> memVec = CreateMemInfo(); \
    auto *memInfo = memVec.data(); \
    auto record = CreateRandomUnaryOpRecord(); \
    auto unaryOp = reinterpret_cast<UnaryOpRecord*>(memInfo + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead)  \
        + sizeof(RecordType) * 2 + sizeof(UnaryOpRecord)); \
    CALL_VCBD(cvtType, dstType, srcType); \
    AssertUnaryOPRecord(unaryOp, record); \
}

// #2375, #2376
VCBD_TEST_CASE_DEFINE(s162s32, int32_t, int16_t);
// #2377, #2378
VCBD_TEST_CASE_DEFINE(s162u32, uint32_t, int16_t);
// #2379, #2380
VCBD_TEST_CASE_DEFINE(s162u8, uint8_t, int16_t);
// #2381, #2382
VCBD_TEST_CASE_DEFINE(s322s16, int16_t, int32_t);
// #2383, #2384
VCBD_TEST_CASE_DEFINE(s322u16, uint16_t, int32_t);
// #2385, #2386
VCBD_TEST_CASE_DEFINE(s322u8, uint8_t, int32_t);
// #2387, #2388
VCBD_TEST_CASE_DEFINE(u162s32, int32_t, uint16_t);
// #2389, #2390
VCBD_TEST_CASE_DEFINE(u162u32, uint32_t, uint16_t);
// #2391, #2392
VCBD_TEST_CASE_DEFINE(u162u8, uint8_t, uint16_t);
// #2393, #2394
VCBD_TEST_CASE_DEFINE(u322s16, int16_t, uint32_t);
// #2395, #2396
VCBD_TEST_CASE_DEFINE(u322u16, uint16_t, uint32_t);
// #2397, #2398
VCBD_TEST_CASE_DEFINE(u322u8, uint8_t, uint32_t);
// #2399, #2400
VCBD_TEST_CASE_DEFINE(u82s16, int16_t, uint8_t);
// #2401, #2402
VCBD_TEST_CASE_DEFINE(u82s32, int32_t, uint8_t);
// #2403, #2404
VCBD_TEST_CASE_DEFINE(u82u16, uint16_t, uint8_t);
// #2405, #2406
VCBD_TEST_CASE_DEFINE(u82u32, uint32_t, uint8_t);
