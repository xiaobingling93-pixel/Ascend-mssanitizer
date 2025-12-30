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


#include "plugin/utils.h"
#include "plugin/record_ctrl_instructions.h"

using namespace Sanitizer;

// #2176
SANITIZER_REPORT(set_mask_count)
{
    RecordRegister(EXTRA_PARAMS, &Register::maskMode, MaskMode::MASK_COUNT);
}

// #2177
SANITIZER_REPORT(set_mask_norm)
{
    RecordRegister(EXTRA_PARAMS, &Register::maskMode, MaskMode::MASK_NORM);
}

// #2208
SANITIZER_REPORT(set_vector_mask, uint64_t reg_idx, uint64_t reg_value)
{
    RecordVectorMask(EXTRA_PARAMS, reg_idx, reg_value);
}

// #3023
SANITIZER_REPORT(set_nd_para, uint64_t config) {
    RecordNdPara(EXTRA_PARAMS, config);
}

SANITIZER_REPORT(MOVEVA, ub_addr8_t addr, uint32_t bitOffset, uint64_t xn, uint64_t xm) {
    RecordVAdPara(EXTRA_PARAMS, addr, bitOffset, xn, xm);
}