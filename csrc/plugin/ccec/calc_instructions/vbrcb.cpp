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

// #2357
SANITIZER_REPORT(vbrcb, __ubuf__ uint16_t *dst, __ubuf__ uint16_t *src, uint64_t config)
{
    auto repeat = uint8_t{};
    auto dstBlockStride = uint16_t{};
    auto srcBlockStride = uint16_t{};
    auto dstRepeatStride = uint16_t{};
    auto srcRepeatStride = uint16_t{};
    ParseUnaryConfigByArch(config, repeat, dstBlockStride, srcBlockStride, dstRepeatStride, srcRepeatStride);
    RecordUnaryOpFunc<RecordType::UNARY_OP>(EXTRA_PARAMS, dst, src, repeat, dstBlockStride, 0,
        dstRepeatStride, 1, 8, 1, 32, 8 * sizeof(__ubuf__ uint16_t), false);
}

 // #2358
SANITIZER_REPORT(vbrcb, __ubuf__ uint16_t *dst, __ubuf__ uint16_t *src, uint16_t dstBlockStride,
    uint16_t dstRepeatStride, uint8_t repeat)
{
    RecordUnaryOpFunc<RecordType::UNARY_OP>(EXTRA_PARAMS, dst, src, repeat,
        dstBlockStride, 0, dstRepeatStride, 1, 8, 1, 32, 8 * sizeof(__ubuf__ uint16_t), false);
}

// #2359
SANITIZER_REPORT(vbrcb, __ubuf__ uint32_t *dst, __ubuf__ uint32_t *src, uint64_t config)
{
    auto repeat = uint8_t{};
    auto dstBlockStride = uint16_t{};
    auto srcBlockStride = uint16_t{};
    auto dstRepeatStride = uint16_t{};
    auto srcRepeatStride = uint16_t{};
    ParseUnaryConfigByArch(config, repeat, dstBlockStride, srcBlockStride, dstRepeatStride, srcRepeatStride);
    RecordUnaryOpFunc<RecordType::UNARY_OP>(EXTRA_PARAMS, dst, src, repeat, dstBlockStride, 0,
        dstRepeatStride, 1, 8, 1, 32, 8 * sizeof(__ubuf__ uint32_t), false);
}

 // #2360
SANITIZER_REPORT(vbrcb, __ubuf__ uint32_t *dst, __ubuf__ uint32_t *src, uint16_t dstBlockStride,
    uint16_t dstRepeatStride, uint8_t repeat)
{
    RecordUnaryOpFunc<RecordType::UNARY_OP>(EXTRA_PARAMS, dst, src, repeat,
        dstBlockStride, 0, dstRepeatStride, 1, 8, 1, 32, 8 * sizeof(__ubuf__ uint32_t), false);
}

#endif
