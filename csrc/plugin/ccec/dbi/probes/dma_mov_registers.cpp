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

#include "plugin/record_ctrl_instructions.h"
#include "core/framework/record_defs.h"

using namespace Sanitizer;

SANITIZER_REPORT(set_pad_cnt_nddma, uint64_t value)
{
    RecordRegister(EXTRA_PARAMS, &Register::sprPadCntNdDma, value);
}

SANITIZER_REPORT(set_loop0_stride_nddma, uint64_t value)
{
    RecordRegister(EXTRA_PARAMS, &Register::sprLoop0StrideNdDma, value);
}

SANITIZER_REPORT(set_loop1_stride_nddma, uint64_t value)
{
    RecordRegister(EXTRA_PARAMS, &Register::sprLoop1StrideNdDma, value);
}

SANITIZER_REPORT(set_loop2_stride_nddma, uint64_t value)
{
    RecordRegister(EXTRA_PARAMS, &Register::sprLoop2StrideNdDma, value);
}

SANITIZER_REPORT(set_loop3_stride_nddma, uint64_t value)
{
    RecordRegister(EXTRA_PARAMS, &Register::sprLoop3StrideNdDma, value);
}

SANITIZER_REPORT(set_loop4_stride_nddma, uint64_t value)
{
    RecordRegister(EXTRA_PARAMS, &Register::sprLoop4StrideNdDma, value);
}

SANITIZER_REPORT(set_mte2_nz_para, uint64_t value)
{
    RecordRegister(EXTRA_PARAMS, &Register::sprMte2NzPara, value);
}
