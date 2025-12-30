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

// #47
SANITIZER_REPORT(copy_cbuf_to_bt, uint64_t dst, __cbuf__ void *src, uint64_t config)
{
    RecordMovBtEvent<MemType::L1, MemType::BT>(EXTRA_PARAMS, dst, reinterpret_cast<uint64_t>(src), config);
}

// #48
SANITIZER_REPORT(copy_cbuf_to_bt, uint64_t dst, __cbuf__ void *src, uint16_t convControl, uint16_t nBurst,
                 uint16_t lenBurst, uint16_t sourceGap, uint16_t dstGap)
{
    RecordMovBtEvent<MemType::L1, MemType::BT>(EXTRA_PARAMS, dst, reinterpret_cast<uint64_t>(src), convControl,
                                               nBurst, lenBurst, sourceGap, dstGap);
}