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
#include "plugin/recorder.h"

using namespace Sanitizer;

SANITIZER_REPORT(copy_gm_to_ubuf_align_b16, __ubuf__ void *dst, __gm__ void *src, uint64_t config, uint64_t gapConfig)
{
    RecordMovAlignEvent<MemType::GM, MemType::UB, DataType::DATA_B16>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                                      reinterpret_cast<uint64_t>(src),
                                                                      config, gapConfig);
}

SANITIZER_REPORT(copy_gm_to_ubuf_align_b32, __ubuf__ void *dst, __gm__ void *src, uint64_t config, uint64_t gapConfig)
{
    RecordMovAlignEvent<MemType::GM, MemType::UB, DataType::DATA_B32>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                                      reinterpret_cast<uint64_t>(src),
                                                                      config, gapConfig);
}

SANITIZER_REPORT(copy_ubuf_to_gm_align_b32, __gm__ void *dst, __ubuf__ void *src, uint64_t config, uint64_t gapConfig)
{
    RecordMovAlignEvent<MemType::UB, MemType::GM, DataType::DATA_B32>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                                      reinterpret_cast<uint64_t>(src),
                                                                      config, gapConfig);
}

SANITIZER_REPORT(copy_gm_to_ubuf_align_b8, __ubuf__ void *dst, __gm__ void *src, uint64_t config, uint64_t gapConfig)
{
    RecordMovAlignEvent<MemType::GM, MemType::UB, DataType::DATA_B8>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                                     reinterpret_cast<uint64_t>(src),
                                                                     config, gapConfig);
}

SANITIZER_REPORT(copy_ubuf_to_gm_align_b16, __gm__ void *dst, __ubuf__ void *src, uint64_t config, uint64_t gapConfig)
{
    RecordMovAlignEvent<MemType::UB, MemType::GM, DataType::DATA_B16>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                                      reinterpret_cast<uint64_t>(src),
                                                                      config, gapConfig);
}

SANITIZER_REPORT(copy_ubuf_to_gm_align_b8, __gm__ void *dst, __ubuf__ void *src, uint64_t config, uint64_t gapConfig)
{
    RecordMovAlignEvent<MemType::UB, MemType::GM, DataType::DATA_B8>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                                     reinterpret_cast<uint64_t>(src),
                                                                     config, gapConfig);
}

SANITIZER_REPORT(copy_ubuf_to_gm_align_v2, __gm__ void *dst, __ubuf__ void *src, uint64_t config0, uint64_t config1)
{
    RecordMovAlignEventV2<MemType::UB, MemType::GM, DataType::DATA_B32>(
        EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config0, config1);
}

SANITIZER_REPORT(copy_gm_to_ubuf_align_v2_b8, __ubuf__ void *dst, __gm__ void *src, uint64_t config0, uint64_t config1)
{
    RecordMovAlignEventV2<MemType::GM, MemType::UB, DataType::DATA_B8>(
        EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config0, config1);
}

SANITIZER_REPORT(copy_gm_to_ubuf_align_v2_b16, __ubuf__ void *dst, __gm__ void *src, uint64_t config0, uint64_t config1)
{
    RecordMovAlignEventV2<MemType::GM, MemType::UB, DataType::DATA_B16>(
        EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config0, config1);
}

SANITIZER_REPORT(copy_gm_to_ubuf_align_v2_b32, __ubuf__ void *dst, __gm__ void *src, uint64_t config0, uint64_t config1)
{
    RecordMovAlignEventV2<MemType::GM, MemType::UB, DataType::DATA_B32>(
        EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config0, config1);
}

SANITIZER_REPORT(copy_gm_to_cbuf_align_v2_b8, __cbuf__ void *dst, __gm__ void *src, uint64_t config0, uint64_t config1)
{
    RecordMovAlignEventV2<MemType::GM, MemType::L1, DataType::DATA_B8>(
        EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config0, config1);
}

SANITIZER_REPORT(copy_gm_to_cbuf_align_v2_b16, __cbuf__ void *dst, __gm__ void *src, uint64_t config0, uint64_t config1)
{
    RecordMovAlignEventV2<MemType::GM, MemType::L1, DataType::DATA_B16>(
        EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config0, config1);
}

SANITIZER_REPORT(copy_gm_to_cbuf_align_v2_b32, __cbuf__ void *dst, __gm__ void *src, uint64_t config0, uint64_t config1)
{
    RecordMovAlignEventV2<MemType::GM, MemType::L1, DataType::DATA_B32>(
        EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config0, config1);
}
