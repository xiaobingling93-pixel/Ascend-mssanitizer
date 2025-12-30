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


#include "plugin/kernel_pub_func.h"
#include "plugin/utils.h"
#include "plugin/record_calc_instructions.h"

using namespace Sanitizer;
// 现版本不对dst的二级指针进行追踪
#if !(defined(__CCE_IS_AICORE__) && __CCE_IS_AICORE__ == 1 && defined(__DAV_C220_CUBE__))
SANITIZER_REPORT(vscatter, __ubuf__ uint32_t *dst, __ubuf__ uint16_t *src, uint64_t config)
{
    RecordVscatterOpConfFunc(EXTRA_PARAMS, dst, src, config);
}

SANITIZER_REPORT(vscatter, __ubuf__ uint32_t *dst, __ubuf__ uint16_t *src, uint32_t offset, bool strideSizeMode,
                 bool repeatUpdateMode, uint8_t repeatTimes, uint8_t repeatStrideSize)
{
    RecordVscatterOpFunc(EXTRA_PARAMS, dst, src, offset, strideSizeMode, repeatUpdateMode,
                         repeatTimes, repeatStrideSize);
}

SANITIZER_REPORT(vscatter, __ubuf__ uint32_t *dst, __ubuf__ uint32_t *src, uint64_t config)
{
    RecordVscatterOpConfFunc(EXTRA_PARAMS, dst, src, config);
}

SANITIZER_REPORT(vscatter, __ubuf__ uint32_t *dst, __ubuf__ uint32_t *src, uint32_t offset, bool strideSizeMode,
                 bool repeatUpdateMode, uint8_t repeatTimes, uint8_t repeatStrideSize)
{
    RecordVscatterOpFunc(EXTRA_PARAMS, dst, src, offset, strideSizeMode, repeatUpdateMode,
                         repeatTimes, repeatStrideSize);
}
#endif