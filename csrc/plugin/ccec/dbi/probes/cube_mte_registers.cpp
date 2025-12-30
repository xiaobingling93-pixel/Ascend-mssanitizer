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

SANITIZER_REPORT(set_mte2_src_para, uint64_t value) {
    RecordRegister(EXTRA_PARAMS, &Register::sprMTE2SrcPara, value);
}

SANITIZER_REPORT(set_loop3_para, uint64_t value) {
    RecordRegister(EXTRA_PARAMS, &Register::sprLoop3Para, value);
}

SANITIZER_REPORT(set_channel_para, uint64_t value) {
    RecordRegister(EXTRA_PARAMS, &Register::sprChannelPara, value);
}

SANITIZER_REPORT(set_fmatrix, uint64_t value) {
    RecordRegister(EXTRA_PARAMS, &Register::sprFmatrix, value);
}

SANITIZER_REPORT(set_fmatrix_b, uint64_t value) {
    RecordRegister(EXTRA_PARAMS, &Register::sprFmatrixB, value);
}

SANITIZER_REPORT(set_fmatrix_dual_0, uint64_t value) {
    RecordRegister(EXTRA_PARAMS, &Register::sprFmatrixDual0, value);
}

SANITIZER_REPORT(set_fmatrix_dual_1, uint64_t value) {
    RecordRegister(EXTRA_PARAMS, &Register::sprFmatrixDual1, value);
}

SANITIZER_REPORT(set_l3d_rpt, uint64_t value) {
    RecordRegister(EXTRA_PARAMS, &Register::sprL3dRpt, value);
}

SANITIZER_REPORT(set_l3d_rpt_b, uint64_t value) {
    RecordRegister(EXTRA_PARAMS, &Register::sprL3dRptB, value);
}

SANITIZER_REPORT(set_padding, uint64_t value) {
    RecordRegister(EXTRA_PARAMS, &Register::sprPadding, value);
}

SANITIZER_REPORT(set_padding_b, uint64_t value) {
    RecordRegister(EXTRA_PARAMS, &Register::sprPaddingB, value);
}
