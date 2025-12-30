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

#if !(defined(__CCE_IS_AICORE__) && __CCE_IS_AICORE__ == 1 && defined(__DAV_C220_CUBE__))

// #2347
SANITIZER_REPORT(VBS16_f16, __ubuf__ half *dst, __ubuf__ half *src, uint64_t config)
{
    RecordUnaryOpConfigFunc<RecordType::UNARY_OP>(EXTRA_PARAMS, dst, src, config, 1, 1, 256, 256);
}

SANITIZER_REPORT(VBS16_f32, __ubuf__ float *dst, __ubuf__ float *src, uint64_t config)
{
    RecordUnaryOpConfigFunc<RecordType::UNARY_OP>(EXTRA_PARAMS, dst, src, config, 1, 1, 256, 256);
}


// #2353
SANITIZER_REPORT(VBS32_f16, __ubuf__ half *dst, __ubuf__ half *src0, __ubuf__ uint32_t *src1, uint64_t config)
{
    constexpr uint64_t repeatShift = 56;
    uint8_t repeat = (config >> repeatShift) & 0xFF;
    RecordBinaryOpFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, repeat,
        0, 0, 0, 1, 1, 1, 1, 1, 1, 256, 64, 128, false);
}

// #2354
SANITIZER_REPORT(VBS32_f16, __ubuf__ half *dst, __ubuf__ half *src0, __ubuf__ uint32_t *src1, uint8_t repeat,
    uint8_t dstBlockStride, uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride,
    uint8_t src0RepeatStride, uint8_t src1RepeatStride)
{
    RecordBinaryOpFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, repeat,
        0, 0, 0, 1, 1, 1, 1, 1, 1, 256, 64, 128, false);
}

// #2355
SANITIZER_REPORT(VBS32_f32, __ubuf__ float *dst, __ubuf__ float *src0, __ubuf__ uint32_t *src1, uint64_t config)
{
    constexpr uint64_t repeatShift = 56;
    uint8_t repeat = (config >> repeatShift) & 0xFF;
    RecordBinaryOpFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, repeat,
        0, 0, 0, 1, 1, 1, 1, 1, 1, 256, 128, 128, false);
}

// #2356
SANITIZER_REPORT(VBS32_f32, __ubuf__ float *dst, __ubuf__ float *src0, __ubuf__ uint32_t *src1, uint8_t repeat,
    uint8_t dstBlockStride, uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride,
    uint8_t src0RepeatStride, uint8_t src1RepeatStride)
{
    RecordBinaryOpFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, repeat,
        0, 0, 0, 1, 1, 1, 1, 1, 1, 256, 128, 128, false);
}

#endif
