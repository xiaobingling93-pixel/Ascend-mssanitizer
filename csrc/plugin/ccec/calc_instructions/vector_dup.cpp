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


#include <cstdint>

#include "plugin/kernel_pub_func.h"
#include "plugin/record_calc_instructions.h"
#include "plugin/utils.h"

using namespace Sanitizer;
#if !(defined(__CCE_IS_AICORE__) && __CCE_IS_AICORE__ == 1 && defined(__DAV_C220_CUBE__))
// #2932
SANITIZER_REPORT(vector_dup, __ubuf__ half *dst, half src, uint64_t config)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, config);
}

// #2936
SANITIZER_REPORT(vector_dup, __ubuf__ float *dst, float src, uint64_t config)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, config);
}

// #2940
SANITIZER_REPORT(vector_dup, __ubuf__ int16_t *dst, int16_t src, uint64_t config)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, config);
}

// #2944
SANITIZER_REPORT(vector_dup, __ubuf__ int32_t *dst, int32_t src, uint64_t config)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, config);
}

// #2948
SANITIZER_REPORT(vector_dup, __ubuf__ uint16_t *dst, uint16_t src, uint64_t config)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, config);
}

// #2952
SANITIZER_REPORT(vector_dup, __ubuf__ uint32_t *dst, uint32_t src, uint64_t config)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, config);
}

// #2952
SANITIZER_REPORT(vector_dup, __ubuf__ __bf16 *dst, __bf16 src, uint64_t config)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, config);
}

// #2934
SANITIZER_REPORT(vector_dup, __ubuf__ half *dst, half src, uint8_t repeat, uint16_t dstBlockStride,
                 uint16_t srcBlockStride, uint8_t dstRepeatStride, uint8_t srcRepeatStride,
                 bool repeatStrideMode, bool strideSizeMode)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, repeat, dstBlockStride, dstRepeatStride);
}

// #2938
SANITIZER_REPORT(vector_dup, __ubuf__ float *dst, float src, uint8_t repeat, uint16_t dstBlockStride,
                 uint16_t srcBlockStride, uint8_t dstRepeatStride, uint8_t srcRepeatStride,
                 bool repeatStrideMode, bool strideSizeMode)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, repeat, dstBlockStride, dstRepeatStride);
}

// #2942
SANITIZER_REPORT(vector_dup, __ubuf__ int16_t *dst, int16_t src, uint8_t repeat, uint16_t dstBlockStride,
                 uint16_t srcBlockStride, uint8_t dstRepeatStride, uint8_t srcRepeatStride,
                 bool repeatStrideMode, bool strideSizeMode)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, repeat, dstBlockStride, dstRepeatStride);
}

// #2946
SANITIZER_REPORT(vector_dup, __ubuf__ int32_t *dst, int32_t src, uint8_t repeat, uint16_t dstBlockStride,
                 uint16_t srcBlockStride, uint8_t dstRepeatStride, uint8_t srcRepeatStride,
                 bool repeatStrideMode, bool strideSizeMode)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, repeat, dstBlockStride, dstRepeatStride);
}

// #2950
SANITIZER_REPORT(vector_dup, __ubuf__ uint16_t *dst, uint16_t src, uint8_t repeat, uint16_t dstBlockStride,
                 uint16_t srcBlockStride, uint8_t dstRepeatStride, uint8_t srcRepeatStride,
                 bool repeatStrideMode, bool strideSizeMode)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, repeat, dstBlockStride, dstRepeatStride);
}

// #2954
SANITIZER_REPORT(vector_dup, __ubuf__ uint32_t *dst, uint32_t src, uint8_t repeat, uint16_t dstBlockStride,
                 uint16_t srcBlockStride, uint8_t dstRepeatStride, uint8_t srcRepeatStride,
                 bool repeatStrideMode, bool strideSizeMode)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, repeat, dstBlockStride, dstRepeatStride);
}

SANITIZER_REPORT(vector_dup, __ubuf__ __bf16 *dst, __bf16 src, uint8_t repeat, uint16_t dstBlockStride,
                 uint16_t srcBlockStride, uint8_t dstRepeatStride, uint8_t srcRepeatStride,
                 bool repeatStrideMode, bool strideSizeMode)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, repeat, dstBlockStride, dstRepeatStride);
}

#if defined(__DAV_C220__) || defined(__DAV_C220_VEC__)
// #2935
SANITIZER_REPORT(vector_dup, __ubuf__ half *dst, half src, uint8_t repeat, uint16_t dstBlockStride,
                 uint16_t srcBlockStride, uint16_t dstRepeatStride, uint16_t srcRepeatStride)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, repeat, dstBlockStride, dstRepeatStride);
}

// #2939
SANITIZER_REPORT(vector_dup, __ubuf__ float *dst, float src, uint8_t repeat, uint16_t dstBlockStride,
                 uint16_t srcBlockStride, uint16_t dstRepeatStride, uint16_t srcRepeatStride)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, repeat, dstBlockStride, dstRepeatStride);
}

// #2943
SANITIZER_REPORT(vector_dup, __ubuf__ int16_t *dst, int16_t src, uint8_t repeat, uint16_t dstBlockStride,
                 uint16_t srcBlockStride, uint16_t dstRepeatStride, uint16_t srcRepeatStride)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, repeat, dstBlockStride, dstRepeatStride);
}

// #2947
SANITIZER_REPORT(vector_dup, __ubuf__ int32_t *dst, int32_t src, uint8_t repeat, uint16_t dstBlockStride,
                 uint16_t srcBlockStride, uint16_t dstRepeatStride, uint16_t srcRepeatStride)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, repeat, dstBlockStride, dstRepeatStride);
}

// #2951
SANITIZER_REPORT(vector_dup, __ubuf__ uint16_t *dst, uint16_t src, uint8_t repeat, uint16_t dstBlockStride,
                 uint16_t srcBlockStride, uint16_t dstRepeatStride, uint16_t srcRepeatStride)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, repeat, dstBlockStride, dstRepeatStride);
}

// #2955
SANITIZER_REPORT(vector_dup, __ubuf__ uint32_t *dst, uint32_t src, uint8_t repeat, uint16_t dstBlockStride,
                 uint16_t srcBlockStride, uint16_t dstRepeatStride, uint16_t srcRepeatStride)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, repeat, dstBlockStride, dstRepeatStride);
}

SANITIZER_REPORT(vector_dup, __ubuf__ __bf16 *dst, __bf16 src, uint8_t repeat, uint16_t dstBlockStride,
                 uint16_t srcBlockStride, uint16_t dstRepeatStride, uint16_t srcRepeatStride)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, repeat, dstBlockStride, dstRepeatStride);
}

#else

// #2933
SANITIZER_REPORT(vector_dup, __ubuf__ half *dst, half src, uint8_t repeat, uint16_t dstBlockStride,
                 uint16_t srcBlockStride, uint8_t dstRepeatStride, uint8_t srcRepeatStride)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, repeat, dstBlockStride, dstRepeatStride);
}

// #2937
SANITIZER_REPORT(vector_dup, __ubuf__ float *dst, float src, uint8_t repeat, uint16_t dstBlockStride,
                 uint16_t srcBlockStride, uint8_t dstRepeatStride, uint8_t srcRepeatStride)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, repeat, dstBlockStride, dstRepeatStride);
}

// #2941
SANITIZER_REPORT(vector_dup, __ubuf__ int16_t *dst, int16_t src, uint8_t repeat, uint16_t dstBlockStride,
                 uint16_t srcBlockStride, uint8_t dstRepeatStride, uint8_t srcRepeatStride)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, repeat, dstBlockStride, dstRepeatStride);
}

// #2945
SANITIZER_REPORT(vector_dup, __ubuf__ int32_t *dst, int32_t src, uint8_t repeat, uint16_t dstBlockStride,
                 uint16_t srcBlockStride, uint8_t dstRepeatStride, uint8_t srcRepeatStride)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, repeat, dstBlockStride, dstRepeatStride);
}

// #2949
SANITIZER_REPORT(vector_dup, __ubuf__ uint16_t *dst, uint16_t src, uint8_t repeat, uint16_t dstBlockStride,
                 uint16_t srcBlockStride, uint8_t dstRepeatStride, uint8_t srcRepeatStride)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, repeat, dstBlockStride, dstRepeatStride);
}

// #2953
SANITIZER_REPORT(vector_dup, __ubuf__ uint32_t *dst, uint32_t src, uint8_t repeat, uint16_t dstBlockStride,
                 uint16_t srcBlockStride, uint8_t dstRepeatStride, uint8_t srcRepeatStride)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, repeat, dstBlockStride, dstRepeatStride);
}

SANITIZER_REPORT(vector_dup, __ubuf__ __bf16 *dst, __bf16 src, uint8_t repeat, uint16_t dstBlockStride,
                 uint16_t srcBlockStride, uint8_t dstRepeatStride, uint8_t srcRepeatStride)
{
    RecordVecDupEvent(EXTRA_PARAMS, dst, repeat, dstBlockStride, dstRepeatStride);
}


#endif


#endif