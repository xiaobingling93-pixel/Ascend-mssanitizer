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
 
// #2718
SANITIZER_REPORT(scatter_vnchwconv_b16, ub_addr8_t dst, ub_addr8_t src, uint64_t config)
{
    RecordScatterVnchwconvEvent(EXTRA_PARAMS, dst, src, config, DataType::DATA_B16);
}
 
// #2723
SANITIZER_REPORT(scatter_vnchwconv_b16, ub_addr8_t dst, ub_addr8_t src,
                 uint8_t repeat, uint16_t dstStride, uint16_t srcStride)
{
    RecordScatterVnchwconvEvent(EXTRA_PARAMS, dst, src, repeat, dstStride, srcStride, DataType::DATA_B16);
}
 
// #2732
SANITIZER_REPORT(scatter_vnchwconv_b32, ub_addr8_t dst, ub_addr8_t src, uint64_t config)
{
    RecordScatterVnchwconvEvent(EXTRA_PARAMS, dst, src, config, DataType::DATA_B32);
}
 
// #2737
SANITIZER_REPORT(scatter_vnchwconv_b32, ub_addr8_t dst, ub_addr8_t src,
                 uint8_t repeat, uint16_t dstStride, uint16_t srcStride)
{
    RecordScatterVnchwconvEvent(EXTRA_PARAMS, dst, src, repeat, dstStride, srcStride, DataType::DATA_B32);
}
 
// #2746
SANITIZER_REPORT(scatter_vnchwconv_b8, ub_addr8_t dst, ub_addr8_t src,
                 uint64_t config, bool dstHighHalf, bool srcHighHalf)
{
    RecordScatterVnchwconvEvent(EXTRA_PARAMS, dst, src, config, DataType::DATA_B8, dstHighHalf, srcHighHalf);
}
 
// #2751
SANITIZER_REPORT(scatter_vnchwconv_b8, ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dstStride,
                 uint16_t srcStride, bool dstHighHalf, bool srcHighHalf)
{
    RecordScatterVnchwconvEvent(EXTRA_PARAMS, dst, src, repeat, dstStride, srcStride, DataType::DATA_B8, dstHighHalf, srcHighHalf);
}
