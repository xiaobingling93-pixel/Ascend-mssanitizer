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
#include "plugin/addr_process.h"

using namespace Sanitizer;

SANITIZER_REPORT(dc_preload, __gm__ uint64_t *address, int64_t offset)
{
    uint64_t addr = reinterpret_cast<uint64_t>(address);
    AddressSpace space = RemapAddress(addr);
    RecordDcPreloadEvent(EXTRA_PARAMS, space, addr, offset);
}

SANITIZER_REPORT(dc_preloadi, __gm__ uint64_t *address, int16_t offset)
{
    uint64_t addr = reinterpret_cast<uint64_t>(address);
    AddressSpace space = RemapAddress(addr);
    RecordDcPreloadEvent(EXTRA_PARAMS, space, addr, static_cast<int64_t>(offset));
}