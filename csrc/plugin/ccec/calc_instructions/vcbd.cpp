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


#include "plugin/record_calc_instructions.h"

using namespace Sanitizer;

#define VCBD_DEF_REPORT_CONFIG(cmdName, dstType, srcType) \
SANITIZER_REPORT(cmdName, __ubuf__ dstType *dst, __ubuf__ srcType *src, uint64_t config) { \
    RecordUnaryOpConfigFunc<RecordType::UNARY_OP>(EXTRA_PARAMS, dst, src, config, 8, 8, 32, 32); \
}

#define VCBD_DEF_REPORT_PARAM(cmdName, dstType, srcType) \
SANITIZER_REPORT(cmdName, __ubuf__ dstType *dst, __ubuf__ srcType *src, uint8_t repeat, uint16_t dstBlockStride, \
    uint16_t srcBlockStride, uint8_t dstRepeatStride, uint8_t srcRepeatStride, bool repeatStrideMode, \
    bool strideSizeMode) { \
        RecordUnaryOpFunc<RecordType::UNARY_OP>(EXTRA_PARAMS, dst, src, repeat, dstBlockStride, \
                                                srcBlockStride, dstRepeatStride, srcRepeatStride, 8, 8, 32, 32); \
    }

#if !(defined(__CCE_IS_AICORE__) && __CCE_IS_AICORE__ == 1 && defined(__DAV_C220_CUBE__))


// #2375, #2376
VCBD_DEF_REPORT_CONFIG(vcbd_s162s32, int32_t, int16_t);
VCBD_DEF_REPORT_PARAM(vcbd_s162s32, int32_t, int16_t);
// #2377, #2378
VCBD_DEF_REPORT_CONFIG(vcbd_s162u32, uint32_t, int16_t);
VCBD_DEF_REPORT_PARAM(vcbd_s162u32, uint32_t, int16_t);
// #2379, #2380
VCBD_DEF_REPORT_CONFIG(vcbd_s162u8, uint8_t, int16_t);
VCBD_DEF_REPORT_PARAM(vcbd_s162u8, uint8_t, int16_t);
// #2381, #2382
VCBD_DEF_REPORT_CONFIG(vcbd_s322s16, int16_t, int32_t);
VCBD_DEF_REPORT_PARAM(vcbd_s322s16, int16_t, int32_t);
// #2383, #2384
VCBD_DEF_REPORT_CONFIG(vcbd_s322u16, uint16_t, int32_t);
VCBD_DEF_REPORT_PARAM(vcbd_s322u16, uint16_t, int32_t);
// #2385, #2386
VCBD_DEF_REPORT_CONFIG(vcbd_s322u8, uint8_t, int32_t);
VCBD_DEF_REPORT_PARAM(vcbd_s322u8, uint8_t, int32_t);
// #2387, #2388
VCBD_DEF_REPORT_CONFIG(vcbd_u162s32, int32_t, uint16_t);
VCBD_DEF_REPORT_PARAM(vcbd_u162s32, int32_t, uint16_t);
// #2389, #2390
VCBD_DEF_REPORT_CONFIG(vcbd_u162u32, uint32_t, uint16_t);
VCBD_DEF_REPORT_PARAM(vcbd_u162u32, uint32_t, uint16_t);
// #2391, #2392
VCBD_DEF_REPORT_CONFIG(vcbd_u162u8, uint8_t, uint16_t);
VCBD_DEF_REPORT_PARAM(vcbd_u162u8, uint8_t, uint16_t);
// #2393, #2394
VCBD_DEF_REPORT_CONFIG(vcbd_u322s16, int16_t, uint32_t);
VCBD_DEF_REPORT_PARAM(vcbd_u322s16, int16_t, uint32_t);
// #2395, #2396
VCBD_DEF_REPORT_CONFIG(vcbd_u322u16, uint16_t, uint32_t);
VCBD_DEF_REPORT_PARAM(vcbd_u322u16, uint16_t, uint32_t);
// #2397, #2398
VCBD_DEF_REPORT_CONFIG(vcbd_u322u8, uint8_t, uint32_t);
VCBD_DEF_REPORT_PARAM(vcbd_u322u8, uint8_t, uint32_t);
// #2399, #2400
VCBD_DEF_REPORT_CONFIG(vcbd_u82s16, int16_t, uint8_t);
VCBD_DEF_REPORT_PARAM(vcbd_u82s16, int16_t, uint8_t);
// #2401, #2402
VCBD_DEF_REPORT_CONFIG(vcbd_u82s32, int32_t, uint8_t);
VCBD_DEF_REPORT_PARAM(vcbd_u82s32, int32_t, uint8_t);
// #2403, #2404
VCBD_DEF_REPORT_CONFIG(vcbd_u82u16, uint16_t, uint8_t);
VCBD_DEF_REPORT_PARAM(vcbd_u82u16, uint16_t, uint8_t);
// #2405, #2406
VCBD_DEF_REPORT_CONFIG(vcbd_u82u32, uint32_t, uint8_t);
VCBD_DEF_REPORT_PARAM(vcbd_u82u32, uint32_t, uint8_t);

#endif