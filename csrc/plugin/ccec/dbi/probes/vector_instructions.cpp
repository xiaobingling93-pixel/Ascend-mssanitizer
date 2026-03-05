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


// for all Vector_Interface_Instructions
#include "plugin/record_ctrl_instructions.h"
#include "plugin/record_calc_instructions.h"

using namespace Sanitizer;

SANITIZER_REPORT(moveva, uint64_t addr, uint64_t bitOffset, uint64_t xn, uint64_t xm)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3101 || __NPU_ARCH__ == 3510)
    RecordVAdPara(EXTRA_PARAMS, static_cast<ub_addr8_t>(addr), bitOffset, xn, xm);
#endif
}

SANITIZER_REPORT(ldva, uint64_t dst, uint64_t src, uint64_t h)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3101 || __NPU_ARCH__ == 3510)
    // h is 0 or 1
    RecordLdva(EXTRA_PARAMS, static_cast<ub_addr8_t>(dst), src, static_cast<bool>(h));
#endif
}

SANITIZER_REPORT(vtranspose, __ubuf__ void *dst, __ubuf__ void *src)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3101 || __NPU_ARCH__ == 3510)
    RecordUnaryOpFunc<RecordType::UNARY_OP>(EXTRA_PARAMS, dst, src, 1, 0, 0, 0, 0, 1, 1, 512, 512, false);
#endif
}

SANITIZER_REPORT(scatter_vnchwconv_b16, uint64_t dst, uint64_t src, uint64_t config)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3101 || __NPU_ARCH__ == 3510)
    RecordScatterVnchwconvA5(EXTRA_PARAMS, static_cast<ub_addr8_t>(dst), static_cast<ub_addr8_t>(src),
                             config, DataType::DATA_B16);
#endif
}

SANITIZER_REPORT(scatter_vnchwconv_b32, uint64_t dst, uint64_t src, uint64_t config)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3101 || __NPU_ARCH__ == 3510)
    RecordScatterVnchwconvA5(EXTRA_PARAMS, static_cast<ub_addr8_t>(dst), static_cast<ub_addr8_t>(src),
                             config, DataType::DATA_B32);
#endif
}

SANITIZER_REPORT(scatter_vnchwconv_b8, uint64_t dst, uint64_t src, uint64_t config,
                 uint64_t dstHighHalf, uint64_t srcHighHalf)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3101 || __NPU_ARCH__ == 3510)
    RecordScatterVnchwconvA5(EXTRA_PARAMS, static_cast<ub_addr8_t>(dst), static_cast<ub_addr8_t>(src), config,
                             DataType::DATA_B8, static_cast<bool>(dstHighHalf), static_cast<bool>(srcHighHalf));
#endif
}

SANITIZER_REPORT(vbs32_f16, __ubuf__ void *dst, __ubuf__ void *src0, __ubuf__ void *src1, uint64_t config)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3101 || __NPU_ARCH__ == 3510)
    RecordVbs32A5<DataType::DATA_B16>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src0),
                                      reinterpret_cast<uint64_t>(src1), config);
#endif
}

SANITIZER_REPORT(vbs32_f32, __ubuf__ void *dst, __ubuf__ void *src0, __ubuf__ void *src1, uint64_t config)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3101 || __NPU_ARCH__ == 3510)
    RecordVbs32A5<DataType::DATA_B32>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src0),
                                      reinterpret_cast<uint64_t>(src1), config);
#endif
}

SANITIZER_REPORT(vmrgsort4_f16, __ubuf__ void *dst, __ubuf__ void *src, uint64_t xm, uint64_t xt)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3101 || __NPU_ARCH__ == 3510)
    RecordVmrgsort4C310<DetailedDataType::F16>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                               reinterpret_cast<uint64_t>(src), xm, xt);
#endif
}

SANITIZER_REPORT(vmrgsort4_f32, __ubuf__ void *dst, __ubuf__ void *src, uint64_t xm, uint64_t xt)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3101 || __NPU_ARCH__ == 3510)
    RecordVmrgsort4C310<DetailedDataType::FLOAT>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                 reinterpret_cast<uint64_t>(src), xm, xt);
#endif
}
