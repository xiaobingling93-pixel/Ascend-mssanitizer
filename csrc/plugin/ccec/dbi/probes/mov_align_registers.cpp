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

SANITIZER_REPORT(set_loop_size_ubtoout, uint64_t value)
{
    RecordRegister(EXTRA_PARAMS, &Register::sprLoopSizeUb2Out, value);
}

SANITIZER_REPORT(set_loop1_stride_ubtoout, uint64_t value)
{
    RecordRegister(EXTRA_PARAMS, &Register::sprLoop1StrideUb2Out, value);
}

SANITIZER_REPORT(set_loop2_stride_ubtoout, uint64_t value)
{
    RecordRegister(EXTRA_PARAMS, &Register::sprLoop2StrideUb2Out, value);
}

SANITIZER_REPORT(set_loop_size_outtoub, uint64_t value)
{
    RecordRegister(EXTRA_PARAMS, &Register::sprLoopSizeOut2Ub, value);
}

SANITIZER_REPORT(set_loop1_stride_outtoub, uint64_t value)
{
    RecordRegister(EXTRA_PARAMS, &Register::sprLoop1StrideOut2Ub, value);
}

SANITIZER_REPORT(set_loop2_stride_outtoub, uint64_t value)
{
    RecordRegister(EXTRA_PARAMS, &Register::sprLoop2StrideOut2Ub, value);
}

SANITIZER_REPORT(set_loop_size_outtol1, uint64_t value)
{
    RecordRegister(EXTRA_PARAMS, &Register::sprLoopSizeOut2L1, value);
}

SANITIZER_REPORT(set_loop1_stride_outtol1, uint64_t value)
{
    RecordRegister(EXTRA_PARAMS, &Register::sprLoop1StrideOut2L1, value);
}

SANITIZER_REPORT(set_loop2_stride_outtol1, uint64_t value)
{
    RecordRegister(EXTRA_PARAMS, &Register::sprLoop2StrideOut2L1, value);
}
