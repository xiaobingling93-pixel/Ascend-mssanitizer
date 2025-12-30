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
// #2976
SANITIZER_REPORT(vgatherb, __ubuf__ uint16_t *dst, __ubuf__ uint32_t *src, uint64_t config)
{
    RecordVgatherbConfigFunc(EXTRA_PARAMS, dst, src, config);
}

// #2977
SANITIZER_REPORT(vgatherb, __ubuf__ uint16_t *dst, __ubuf__ uint32_t *src, uint32_t offsetAddr,
    uint16_t dstRepeatStride, uint8_t dstBlockStride, uint8_t repeat)
{
    RecordVgatherOpFunc(EXTRA_PARAMS, dst, src, offsetAddr, dstRepeatStride, dstBlockStride,
                        repeat, 8U, false);
}

// #2978
SANITIZER_REPORT(vgatherb, __ubuf__ uint32_t *dst, __ubuf__ uint32_t *src, uint64_t config)
{
    RecordVgatherbConfigFunc(EXTRA_PARAMS, dst, src, config);
}

// #2979
SANITIZER_REPORT(vgatherb, __ubuf__ uint32_t *dst, __ubuf__ uint32_t *src, uint32_t offsetAddr,
    uint16_t dstRepeatStride, uint8_t dstBlockStride, uint8_t repeat)
{
    RecordVgatherOpFunc(EXTRA_PARAMS, dst, src, offsetAddr, dstRepeatStride, dstBlockStride,
                        repeat, 8U, false);
}

#endif
