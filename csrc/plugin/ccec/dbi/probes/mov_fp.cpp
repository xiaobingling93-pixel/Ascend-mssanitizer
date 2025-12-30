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


#include "plugin/record_move_instructions.h"

using namespace Sanitizer;

SANITIZER_REPORT(copy_matrix_cc_to_gm_f32, __gm__ void *dst, __cc__ float *src, uint64_t xm, uint64_t xt)
{
    RecordMovFpEvent(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                     reinterpret_cast<uint64_t>(src), xm, xt, true);
}

SANITIZER_REPORT(copy_matrix_cc_to_gm_s32, __gm__ void *dst, __cc__ int32_t *src, uint64_t xm, uint64_t xt)
{
    RecordMovFpEvent(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                     reinterpret_cast<uint64_t>(src), xm, xt, false);
}

SANITIZER_REPORT(copy_matrix_cc_to_gm_f32_a5, __gm__ void *dst, __cc__ float *src, uint64_t xm, uint64_t xt)
{
    RecordMovFpV2Event(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                       reinterpret_cast<uint64_t>(src), xm, xt, true);
}

SANITIZER_REPORT(copy_matrix_cc_to_gm_s32_a5, __gm__ void *dst, __cc__ int32_t *src, uint64_t xm, uint64_t xt)
{
    RecordMovFpV2Event(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                       reinterpret_cast<uint64_t>(src), xm, xt, false);
}

SANITIZER_REPORT(copy_matrix_cc_to_cbuf_f32, __cbuf__ void *dst, __cc__ float *src, uint64_t xm, uint64_t xt)
{
    RecordFixL0CToL1Event(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                       reinterpret_cast<uint64_t>(src), xm, xt, true);
}
 
SANITIZER_REPORT(copy_matrix_cc_to_cbuf_s32, __cbuf__ void *dst, __cc__ int32_t *src, uint64_t xm, uint64_t xt)
{
    RecordFixL0CToL1Event(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                       reinterpret_cast<uint64_t>(src), xm, xt, false);
}
 
SANITIZER_REPORT(copy_matrix_cc_to_ubuf_f32, __ubuf__ void *dst, __cc__ float *src, uint64_t xm, uint64_t xt)
{
    RecordFixL0CToUBEvent(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                       reinterpret_cast<uint64_t>(src), xm, xt, true);
}
 
SANITIZER_REPORT(copy_matrix_cc_to_ubuf_s32, __ubuf__ void *dst, __cc__ int32_t *src, uint64_t xm, uint64_t xt)
{
    RecordFixL0CToUBEvent(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                       reinterpret_cast<uint64_t>(src), xm, xt, false);
}