
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
// #1256
SANITIZER_REPORT(load_cbuf_to_cb_b2, __cb__ half *dst, __cbuf__ half *src, uint64_t config)
{
    RecordLoadB2Event<MemType::L1, MemType::L0B, DataType::DATA_B16>(EXTRA_PARAMS, dst, src, config);
}

// #1257
SANITIZER_REPORT(load_cbuf_to_cb_b2, __cb__ half *dst, __cbuf__ half *src, uint8_t repeat, uint8_t sid)
{
    RecordLoadB2Event<MemType::L1, MemType::L0B, DataType::DATA_B16>(EXTRA_PARAMS, dst, src, repeat);
}
// #1258
SANITIZER_REPORT(load_cbuf_to_cb_b2, __cb__ half *dst, __cbuf__ half *src, uint8_t repeat)
{
    RecordLoadB2Event<MemType::L1, MemType::L0B, DataType::DATA_B16>(EXTRA_PARAMS, dst, src, repeat);
}

// #1259
SANITIZER_REPORT(load_cbuf_to_cb_b2, __cb__ int8_t *dst, __cbuf__ int8_t *src, uint64_t config)
{
    RecordLoadB2Event<MemType::L1, MemType::L0B, DataType::DATA_B8>(EXTRA_PARAMS, dst, src, config);
}

// #1260
SANITIZER_REPORT(load_cbuf_to_cb_b2, __cb__ int8_t *dst, __cbuf__ int8_t *src, uint8_t repeat, uint8_t sid)
{
    RecordLoadB2Event<MemType::L1, MemType::L0B, DataType::DATA_B8>(EXTRA_PARAMS, dst, src, repeat);
}
// #1261
SANITIZER_REPORT(load_cbuf_to_cb_b2, __cb__ int8_t *dst, __cbuf__ int8_t *src, uint8_t repeat)
{
    RecordLoadB2Event<MemType::L1, MemType::L0B, DataType::DATA_B8>(EXTRA_PARAMS, dst, src, repeat);
}
// #1262
SANITIZER_REPORT(load_cbuf_to_cb_b2, __cb__ uint8_t *dst, __cbuf__ uint8_t *src, uint64_t config)
{
    RecordLoadB2Event<MemType::L1, MemType::L0B, DataType::DATA_B8>(EXTRA_PARAMS, dst, src, config);
}

// #1263
SANITIZER_REPORT(load_cbuf_to_cb_b2, __cb__ uint8_t *dst, __cbuf__ uint8_t *src, uint8_t repeat, uint8_t sid)
{
    RecordLoadB2Event<MemType::L1, MemType::L0B, DataType::DATA_B8>(EXTRA_PARAMS, dst, src, repeat);
}
// #1264
SANITIZER_REPORT(load_cbuf_to_cb_b2, __cb__ uint8_t *dst, __cbuf__ uint8_t *src, uint8_t repeat)
{
    RecordLoadB2Event<MemType::L1, MemType::L0B, DataType::DATA_B8>(EXTRA_PARAMS, dst, src, repeat);
}