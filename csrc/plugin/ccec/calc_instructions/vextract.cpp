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

SANITIZER_REPORT(vextract, __ubuf__ half *dst, __ubuf__ half *src, uint64_t config)
{
    RecordVRegPropCoorOpFunc<RecordType::VEC_REGPROPCOOR_OP, DataType::DATA_B16>(EXTRA_PARAMS, dst, src, config, true);
}

SANITIZER_REPORT(vextract, __ubuf__ half *dst, __ubuf__ half *src, uint8_t repeat, uint8_t regionRange)
{
    RecordVRegPropCoorOpFunc<RecordType::VEC_REGPROPCOOR_OP, DataType::DATA_B16>(EXTRA_PARAMS, dst, src,
                                                                                 repeat, regionRange, true);
}

SANITIZER_REPORT(vextract, __ubuf__ float *dst, __ubuf__ float *src, uint64_t config)
{
    RecordVRegPropCoorOpFunc<RecordType::VEC_REGPROPCOOR_OP, DataType::DATA_B32>(EXTRA_PARAMS, dst, src, config, true);
}

SANITIZER_REPORT(vextract, __ubuf__ float *dst, __ubuf__ float *src, uint8_t repeat, uint8_t regionRange)
{
    RecordVRegPropCoorOpFunc<RecordType::VEC_REGPROPCOOR_OP, DataType::DATA_B32>(EXTRA_PARAMS, dst, src,
                                                                                 repeat, regionRange, true);
}

#endif