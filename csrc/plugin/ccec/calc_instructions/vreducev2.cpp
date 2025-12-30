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
// #3159
SANITIZER_REPORT(vreducev2, __ubuf__ uint16_t *dst, __ubuf__ uint16_t *src0, __ubuf__ uint16_t *src1, uint64_t config)
{
    auto repeat = uint16_t{};
    auto src0BlockStride = uint8_t{};
    auto patternMode = uint8_t{};
    auto src0RepeatStride = uint16_t{};
    auto src1RepeatStride = uint16_t{};
    ParseVredecev2Config(config, repeat, src0BlockStride, patternMode, src0RepeatStride, src1RepeatStride);
    RecordVreducev2Func(EXTRA_PARAMS, dst, src0, src1, repeat, src0BlockStride, patternMode,
                        src0RepeatStride, src1RepeatStride);
}

// #3160
SANITIZER_REPORT(vreducev2, __ubuf__ uint16_t *dst, __ubuf__ uint16_t *src0, __ubuf__ uint16_t *src1, uint16_t repeat,
    uint8_t src0BlockStride, uint8_t patternMode, uint16_t src0RepeatStride, uint16_t src1RepeatStride)
{
    RecordVreducev2Func(EXTRA_PARAMS, dst, src0, src1, repeat, src0BlockStride, patternMode,
                        src0RepeatStride, src1RepeatStride);
}

// #3161
SANITIZER_REPORT(vreducev2, __ubuf__ uint32_t *dst, __ubuf__ uint32_t *src0, __ubuf__ uint32_t *src1, uint64_t config)
{
    auto repeat = uint16_t{};
    auto src0BlockStride = uint8_t{};
    auto patternMode = uint8_t{};
    auto src0RepeatStride = uint16_t{};
    auto src1RepeatStride = uint16_t{};
    ParseVredecev2Config(config, repeat, src0BlockStride, patternMode, src0RepeatStride, src1RepeatStride);
    RecordVreducev2Func(EXTRA_PARAMS, dst, src0, src1, repeat, src0BlockStride, patternMode,
                        src0RepeatStride, src1RepeatStride);
}

// #3162
SANITIZER_REPORT(vreducev2, __ubuf__ uint32_t *dst, __ubuf__ uint32_t *src0, __ubuf__ uint32_t *src1, uint16_t repeat,
    uint8_t src0BlockStride, uint8_t patternMode, uint16_t src0RepeatStride, uint16_t src1RepeatStride)
{
    RecordVreducev2Func(EXTRA_PARAMS, dst, src0, src1, repeat, src0BlockStride, patternMode,
                        src0RepeatStride, src1RepeatStride);
}

#endif
