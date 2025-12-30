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
// #2513
SANITIZER_REPORT(vcmpv_eq, __ubuf__ uint8_t *dst, __ubuf__ half *src0, __ubuf__ half *src1, uint64_t config)
{
    RecordBinaryOpConfigFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, config, 1, 8, 8, 16, 32, 32, false);
}
 
// #2514
SANITIZER_REPORT(vcmpv_eq, __ubuf__ uint8_t *dst, __ubuf__ half *src0, __ubuf__ half *src1, uint8_t repeat, \
    uint8_t dstBlockStride, uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, \
    uint8_t src0RepeatStride, uint8_t src1RepeatStride)
{
    RecordBinaryOpFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, repeat, \
        dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride, src0RepeatStride, src1RepeatStride,
        1, 8, 8, 16, 32, 32, false);
}
 
// #2515
SANITIZER_REPORT(vcmpv_eq, __ubuf__ uint8_t *dst, __ubuf__ float *src0, __ubuf__ float *src1, uint64_t config)
{
    RecordBinaryOpConfigFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, config, 1, 8, 8, 8, 32, 32, false);
}
 
// #2516
SANITIZER_REPORT(vcmpv_eq, __ubuf__ uint8_t *dst, __ubuf__ float *src0, __ubuf__ float *src1, uint8_t repeat, \
    uint8_t dstBlockStride, uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, \
    uint8_t src0RepeatStride, uint8_t src1RepeatStride)
{
    RecordBinaryOpFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, repeat, \
        dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride, src0RepeatStride, src1RepeatStride,
        1, 8, 8, 8, 32, 32, false);
}
 
// #2517
SANITIZER_REPORT(vcmpv_eq, __ubuf__ uint8_t *dst, __ubuf__ int32_t *src0, __ubuf__ int32_t *src1, uint64_t config)
{
    RecordBinaryOpConfigFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, config, 1, 8, 8, 8, 32, 32, false);
}
 
// #2518
SANITIZER_REPORT(vcmpv_eq, __ubuf__ uint8_t *dst, __ubuf__ int32_t *src0, __ubuf__ int32_t *src1, uint8_t repeat, \
    uint8_t dstBlockStride, uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, \
    uint8_t src0RepeatStride, uint8_t src1RepeatStride)
{
    RecordBinaryOpFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, repeat, \
        dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride, src0RepeatStride, src1RepeatStride,
        1, 8, 8, 8, 32, 32, false);
}
 
// #2519
SANITIZER_REPORT(vcmpv_ge, __ubuf__ uint8_t *dst, __ubuf__ half *src0, __ubuf__ half *src1, uint64_t config)
{
    RecordBinaryOpConfigFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, config, 1, 8, 8, 16, 32, 32, false);
}
 
// #2520
SANITIZER_REPORT(vcmpv_ge, __ubuf__ uint8_t *dst, __ubuf__ half *src0, __ubuf__ half *src1, uint8_t repeat, \
    uint8_t dstBlockStride, uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, \
    uint8_t src0RepeatStride, uint8_t src1RepeatStride)
{
    RecordBinaryOpFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, repeat, \
        dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride, src0RepeatStride, src1RepeatStride,
        1, 8, 8, 16, 32, 32, false);
}
 
// #2521
SANITIZER_REPORT(vcmpv_ge, __ubuf__ uint8_t *dst, __ubuf__ float *src0, __ubuf__ float *src1, uint64_t config)
{
    RecordBinaryOpConfigFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, config, 1, 8, 8, 8, 32, 32, false);
}
 
// #2522
SANITIZER_REPORT(vcmpv_ge, __ubuf__ uint8_t *dst, __ubuf__ float *src0, __ubuf__ float *src1, uint8_t repeat, \
    uint8_t dstBlockStride, uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, \
    uint8_t src0RepeatStride, uint8_t src1RepeatStride)
{
    RecordBinaryOpFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, repeat, \
        dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride, src0RepeatStride, src1RepeatStride,
        1, 8, 8, 8, 32, 32, false);
}
 
// #2523
SANITIZER_REPORT(vcmpv_gt, __ubuf__ uint8_t *dst, __ubuf__ half *src0, __ubuf__ half *src1, uint64_t config)
{
    RecordBinaryOpConfigFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, config, 1, 8, 8, 16, 32, 32, false);
}
 
// #2524
SANITIZER_REPORT(vcmpv_gt, __ubuf__ uint8_t *dst, __ubuf__ half *src0, __ubuf__ half *src1, uint8_t repeat, \
    uint8_t dstBlockStride, uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, \
    uint8_t src0RepeatStride, uint8_t src1RepeatStride)
{
    RecordBinaryOpFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, repeat, \
        dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride, src0RepeatStride, src1RepeatStride,
        1, 8, 8, 16, 32, 32, false);
}
 
// #2525
SANITIZER_REPORT(vcmpv_gt, __ubuf__ uint8_t *dst, __ubuf__ float *src0, __ubuf__ float *src1, uint64_t config)
{
    RecordBinaryOpConfigFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, config, 1, 8, 8, 8, 32, 32, false);
}
 
// #2526
SANITIZER_REPORT(vcmpv_gt, __ubuf__ uint8_t *dst, __ubuf__ float *src0, __ubuf__ float *src1, uint8_t repeat, \
    uint8_t dstBlockStride, uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, \
    uint8_t src0RepeatStride, uint8_t src1RepeatStride)
{
    RecordBinaryOpFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, repeat, \
        dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride, src0RepeatStride, src1RepeatStride,
        1, 8, 8, 8, 32, 32, false);
}
 
// #2527
SANITIZER_REPORT(vcmpv_le, __ubuf__ uint8_t *dst, __ubuf__ half *src0, __ubuf__ half *src1, uint64_t config)
{
    RecordBinaryOpConfigFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, config, 1, 8, 8, 16, 32, 32, false);
}
 
// #2528
SANITIZER_REPORT(vcmpv_le, __ubuf__ uint8_t *dst, __ubuf__ half *src0, __ubuf__ half *src1, uint8_t repeat, \
    uint8_t dstBlockStride, uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, \
    uint8_t src0RepeatStride, uint8_t src1RepeatStride)
{
    RecordBinaryOpFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, repeat, \
        dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride, src0RepeatStride, src1RepeatStride,
        1, 8, 8, 16, 32, 32, false);
}
 
// #2529
SANITIZER_REPORT(vcmpv_le, __ubuf__ uint8_t *dst, __ubuf__ float *src0, __ubuf__ float *src1, uint64_t config)
{
    RecordBinaryOpConfigFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, config, 1, 8, 8, 8, 32, 32, false);
}
 
// #2530
SANITIZER_REPORT(vcmpv_le, __ubuf__ uint8_t *dst, __ubuf__ float *src0, __ubuf__ float *src1, uint8_t repeat, \
    uint8_t dstBlockStride, uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, \
    uint8_t src0RepeatStride, uint8_t src1RepeatStride)
{
    RecordBinaryOpFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, repeat, \
        dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride, src0RepeatStride, src1RepeatStride,
        1, 8, 8, 8, 32, 32, false);
}
 
// #2531
SANITIZER_REPORT(vcmpv_lt, __ubuf__ uint8_t *dst, __ubuf__ half *src0, __ubuf__ half *src1, uint64_t config)
{
    RecordBinaryOpConfigFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, config, 1, 8, 8, 16, 32, 32, false);
}
 
// #2532
SANITIZER_REPORT(vcmpv_lt, __ubuf__ uint8_t *dst, __ubuf__ half *src0, __ubuf__ half *src1, uint8_t repeat, \
    uint8_t dstBlockStride, uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, \
    uint8_t src0RepeatStride, uint8_t src1RepeatStride)
{
    RecordBinaryOpFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, repeat, \
        dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride, src0RepeatStride, src1RepeatStride,
        1, 8, 8, 16, 32, 32, false);
}
 
// #2533
SANITIZER_REPORT(vcmpv_lt, __ubuf__ uint8_t *dst, __ubuf__ float *src0, __ubuf__ float *src1, uint64_t config)
{
    RecordBinaryOpConfigFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, config, 1, 8, 8, 8, 32, 32, false);
}
 
// #2534
SANITIZER_REPORT(vcmpv_lt, __ubuf__ uint8_t *dst, __ubuf__ float *src0, __ubuf__ float *src1, uint8_t repeat, \
    uint8_t dstBlockStride, uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, \
    uint8_t src0RepeatStride, uint8_t src1RepeatStride)
{
    RecordBinaryOpFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, repeat, \
        dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride, src0RepeatStride, src1RepeatStride,
        1, 8, 8, 8, 32, 32, false);
}
 
// #2535
SANITIZER_REPORT(vcmpv_ne, __ubuf__ uint8_t *dst, __ubuf__ half *src0, __ubuf__ half *src1, uint64_t config)
{
    RecordBinaryOpConfigFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, config, 1, 8, 8, 16, 32, 32, false);
}
 
// #2536
SANITIZER_REPORT(vcmpv_ne, __ubuf__ uint8_t *dst, __ubuf__ half *src0, __ubuf__ half *src1, uint8_t repeat, \
    uint8_t dstBlockStride, uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, \
    uint8_t src0RepeatStride, uint8_t src1RepeatStride)
{
    RecordBinaryOpFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, repeat, \
        dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride, src0RepeatStride, src1RepeatStride,
        1, 8, 8, 16, 32, 32, false);
}
 
// #2537
SANITIZER_REPORT(vcmpv_ne, __ubuf__ uint8_t *dst, __ubuf__ float *src0, __ubuf__ float *src1, uint64_t config)
{
    RecordBinaryOpConfigFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, config, 1, 8, 8, 8, 32, 32, false);
}
 
// #2538
SANITIZER_REPORT(vcmpv_ne, __ubuf__ uint8_t *dst, __ubuf__ float *src0, __ubuf__ float *src1, uint8_t repeat, \
    uint8_t dstBlockStride, uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, \
    uint8_t src0RepeatStride, uint8_t src1RepeatStride)
{
    RecordBinaryOpFunc<RecordType::BINARY_OP>(EXTRA_PARAMS, dst, src0, src1, repeat, \
        dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride, src0RepeatStride, src1RepeatStride,
        1, 8, 8, 8, 32, 32, false);
}


#endif
