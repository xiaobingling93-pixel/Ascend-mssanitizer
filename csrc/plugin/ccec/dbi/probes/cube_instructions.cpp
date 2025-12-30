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

SANITIZER_REPORT(mad_s8, __cc__ void *c, __ca__ void *a, __cb__ void *b, uint64_t config)
{
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3101
    RecordMmadA5<DetailedDataType::S8, DetailedDataType::S8>(EXTRA_PARAMS, c, a, b, config);
#endif
}

SANITIZER_REPORT(mad_f16_f32, __cc__ void *c, __ca__ void *a, __cb__ void *b, uint64_t config)
{
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3101
    RecordMmadA5<DetailedDataType::F16, DetailedDataType::F16>(EXTRA_PARAMS, c, a, b, config);
#endif
}

SANITIZER_REPORT(mad_bf16_f32, __cc__ void *c, __ca__ void *a, __cb__ void *b, uint64_t config)
{
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3101
    RecordMmadA5<DetailedDataType::BF16, DetailedDataType::BF16>(EXTRA_PARAMS, c, a, b, config);
#endif
}

SANITIZER_REPORT(mad_f32_f32, __cc__ void *c, __ca__ void *a, __cb__ void *b, uint64_t config)
{
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3101
    RecordMmadA5<DetailedDataType::FLOAT, DetailedDataType::FLOAT>(EXTRA_PARAMS, c, a, b, config);
#endif
}

SANITIZER_REPORT(mad_e4m3_e4m3, __cc__ void *c, __ca__ void *a, __cb__ void *b, uint64_t config)
{
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3101
    RecordMmadA5<DetailedDataType::E4M3, DetailedDataType::E4M3>(EXTRA_PARAMS, c, a, b, config);
#endif
}

SANITIZER_REPORT(mad_e4m3_e5m2, __cc__ void *c, __ca__ void *a, __cb__ void *b, uint64_t config)
{
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3101
    RecordMmadA5<DetailedDataType::E4M3, DetailedDataType::E5M2>(EXTRA_PARAMS, c, a, b, config);
#endif
}

SANITIZER_REPORT(mad_e5m2_e4m3, __cc__ void *c, __ca__ void *a, __cb__ void *b, uint64_t config)
{
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3101
    RecordMmadA5<DetailedDataType::E5M2, DetailedDataType::E4M3>(EXTRA_PARAMS, c, a, b, config);
#endif
}

SANITIZER_REPORT(mad_e5m2_e5m2, __cc__ void *c, __ca__ void *a, __cb__ void *b, uint64_t config)
{
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3101
    RecordMmadA5<DetailedDataType::E5M2, DetailedDataType::E5M2>(EXTRA_PARAMS, c, a, b, config);
#endif
}

SANITIZER_REPORT(mad_mx_e1m2_e1m2, __cc__ void *c, __ca__ void *a, __cb__ void *b, uint64_t config)
{
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3101
    RecordMmadA5<DetailedDataType::E1M2, DetailedDataType::E1M2>(EXTRA_PARAMS, c, a, b, config);
#endif
}

SANITIZER_REPORT(mad_mx_e1m2_e2m1, __cc__ void *c, __ca__ void *a, __cb__ void *b, uint64_t config)
{
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3101
    RecordMmadA5<DetailedDataType::E1M2, DetailedDataType::E2M1>(EXTRA_PARAMS, c, a, b, config);
#endif
}

SANITIZER_REPORT(mad_mx_e2m1_e1m2, __cc__ void *c, __ca__ void *a, __cb__ void *b, uint64_t config)
{
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3101
    RecordMmadA5<DetailedDataType::E2M1, DetailedDataType::E1M2>(EXTRA_PARAMS, c, a, b, config);
#endif
}

SANITIZER_REPORT(mad_mx_e2m1_e2m1, __cc__ void *c, __ca__ void *a, __cb__ void *b, uint64_t config)
{
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3101
    RecordMmadA5<DetailedDataType::E2M1, DetailedDataType::E2M1>(EXTRA_PARAMS, c, a, b, config);
#endif
}

// mmad_mx的L0A/L0B任一为e5m2/e4m3时，对应的alignment为1024，否则为512
SANITIZER_REPORT(mad_mx_e4m3_e4m3, __cc__ void *c, __ca__ void *a, __cb__ void *b, uint64_t config)
{
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3101
    RecordMmadA5<DetailedDataType::E4M3, DetailedDataType::E4M3>(EXTRA_PARAMS, c, a, b, config, 1024);
#endif
}

SANITIZER_REPORT(mad_mx_e4m3_e5m2, __cc__ void *c, __ca__ void *a, __cb__ void *b, uint64_t config)
{
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3101
    RecordMmadA5<DetailedDataType::E4M3, DetailedDataType::E5M2>(EXTRA_PARAMS, c, a, b, config, 1024);
#endif
}

SANITIZER_REPORT(mad_mx_e5m2_e4m3, __cc__ void *c, __ca__ void *a, __cb__ void *b, uint64_t config)
{
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3101
    RecordMmadA5<DetailedDataType::E5M2, DetailedDataType::E4M3>(EXTRA_PARAMS, c, a, b, config, 1024);
#endif
}

SANITIZER_REPORT(mad_mx_e5m2_e5m2, __cc__ void *c, __ca__ void *a, __cb__ void *b, uint64_t config)
{
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3101
    RecordMmadA5<DetailedDataType::E5M2, DetailedDataType::E5M2>(EXTRA_PARAMS, c, a, b, config, 1024);
#endif
}
