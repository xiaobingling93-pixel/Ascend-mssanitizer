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

// #2137
SANITIZER_REPORT(set_l0a_2d, __ca__ void *dst, int64_t config)
{
    RecordM200Set2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), config);
}

// #2138
SANITIZER_REPORT(set_l0a_2d, __ca__ __bf16 *dst, int64_t config)
{
    RecordSet2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), config);
}

// #2139
SANITIZER_REPORT(set_l0a_2d, __ca__ half *dst, int64_t config)
{
    RecordSet2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), config);
}

// #2140
SANITIZER_REPORT(set_l0a_2d, __ca__ float *dst, int64_t config)
{
    RecordSet2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), config);
}

// #2141
SANITIZER_REPORT(set_l0a_2d, __ca__ int16_t *dst, int64_t config)
{
    RecordSet2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), config);
}

// #2142
SANITIZER_REPORT(set_l0a_2d, __ca__ int32_t *dst, int64_t config)
{
    RecordSet2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), config);
}

// #2143
SANITIZER_REPORT(set_l0a_2d, __ca__ uint16_t *dst, int64_t config)
{
    RecordSet2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), config);
}

// #2145
SANITIZER_REPORT(set_l0a_2d, __ca__ uint32_t *dst, int64_t config)
{
    RecordSet2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), config);
}

// #2148
SANITIZER_REPORT(set_l0b_2d, __cb__ __bf16 *dst, int64_t config)
{
    RecordSet2DEvent<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), config);
}

// #2149
SANITIZER_REPORT(set_l0b_2d, __cb__ half *dst, int64_t config)
{
    RecordSet2DEvent<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), config);
}

// #2150
SANITIZER_REPORT(set_l0b_2d, __cb__ float *dst, int64_t config)
{
    RecordSet2DEvent<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), config);
}

// #2151
SANITIZER_REPORT(set_l0b_2d, __cb__ int16_t *dst, int64_t config)
{
    RecordSet2DEvent<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), config);
}

// #2152
SANITIZER_REPORT(set_l0b_2d, __cb__ int32_t *dst, int64_t config)
{
    RecordSet2DEvent<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), config);
}

// #2153
SANITIZER_REPORT(set_l0b_2d, __cb__ uint16_t *dst, int64_t config)
{
    RecordSet2DEvent<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), config);
}

// #2155
SANITIZER_REPORT(set_l0b_2d, __cb__ uint32_t *dst, int64_t config)
{
    RecordSet2DEvent<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), config);
}

// #2157
SANITIZER_REPORT(set_l1_2d, __cbuf__ void *dst, int64_t config)
{
    RecordM200Set2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), config);
}

// #2158
SANITIZER_REPORT(set_l1_2d, __cbuf__ __bf16 *dst, int64_t config)
{
    RecordSet2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), config);
}

// #2159
SANITIZER_REPORT(set_l1_2d, __cbuf__ half *dst, int64_t config)
{
    RecordSet2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), config);
}

// #2160
SANITIZER_REPORT(set_l1_2d, __cbuf__ float *dst, int64_t config)
{
    RecordSet2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), config);
}

// #2161
SANITIZER_REPORT(set_l1_2d, __cbuf__ int16_t *dst, int64_t config)
{
    RecordSet2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), config);
}

// #2162
SANITIZER_REPORT(set_l1_2d, __cbuf__ int32_t *dst, int64_t config)
{
    RecordSet2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), config);
}

// #2163
SANITIZER_REPORT(set_l1_2d, __cbuf__ uint16_t *dst, int64_t config)
{
    RecordSet2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), config);
}

// #2165
SANITIZER_REPORT(set_l1_2d, __cbuf__ uint32_t *dst, int64_t config)
{
    RecordSet2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), config);
}

// #573
SANITIZER_REPORT(create_ca_matrix, __ca__ void *dst, int64_t repeat, half value)
{
    RecordM200Set2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #574
SANITIZER_REPORT(create_ca_matrix, __ca__ __bf16 *dst, int64_t repeat, half value)
{
    RecordSet2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #575
SANITIZER_REPORT(create_ca_matrix, __ca__ __bf16 *dst, int64_t repeat, __bf16 value)
{
    RecordSet2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #576
SANITIZER_REPORT(create_ca_matrix, __ca__ __bf16 *dst, int64_t repeat, uint32_t value)
{
    RecordSet2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #577
SANITIZER_REPORT(create_ca_matrix, __ca__ half *dst, int64_t repeat, half value)
{
    RecordSet2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #578
SANITIZER_REPORT(create_ca_matrix, __ca__ int16_t *dst, int64_t repeat, half value)
{
    RecordSet2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #579
SANITIZER_REPORT(create_ca_matrix, __ca__ uint16_t *dst, int64_t repeat, half value)
{
    RecordSet2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #580
SANITIZER_REPORT(create_ca_matrix, __ca__ half *dst, int64_t repeat, uint32_t value)
{
    RecordSet2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #581
SANITIZER_REPORT(create_ca_matrix, __ca__ float *dst, int64_t repeat, uint32_t value)
{
    RecordSet2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #582
SANITIZER_REPORT(create_ca_matrix, __ca__ float *dst, int64_t repeat, half value)
{
    RecordSet2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #583
SANITIZER_REPORT(create_ca_matrix, __ca__ int16_t *dst, int64_t repeat, uint32_t value)
{
    RecordSet2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #584
SANITIZER_REPORT(create_ca_matrix, __ca__ int32_t *dst, int64_t repeat, uint32_t value)
{
    RecordSet2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #585
SANITIZER_REPORT(create_ca_matrix, __ca__ int32_t *dst, int64_t repeat, half value)
{
    RecordSet2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #586
SANITIZER_REPORT(create_ca_matrix, __ca__ uint16_t *dst, int64_t repeat, uint32_t value)
{
    RecordSet2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #587
SANITIZER_REPORT(create_ca_matrix, __ca__ uint32_t *dst, int64_t repeat, uint32_t value)
{
    RecordSet2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #588
SANITIZER_REPORT(create_ca_matrix, __ca__ uint32_t *dst, int64_t repeat, half value)
{
    RecordSet2DEvent<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #593
SANITIZER_REPORT(create_cb_matrix, __cb__ void *dst, int64_t repeat, half value)
{
    RecordM200Set2DEvent<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #594
SANITIZER_REPORT(create_cb_matrix, __cb__ __bf16 *dst, int64_t repeat, half value)
{
    RecordSet2DEvent<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #595
SANITIZER_REPORT(create_cb_matrix, __cb__ __bf16 *dst, int64_t repeat, __bf16 value)
{
    RecordSet2DEvent<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #596
SANITIZER_REPORT(create_cb_matrix, __cb__ __bf16 *dst, int64_t repeat, uint32_t value)
{
    RecordSet2DEvent<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #597
SANITIZER_REPORT(create_cb_matrix, __cb__ half *dst, int64_t repeat, half value)
{
    RecordSet2DEvent<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #598
SANITIZER_REPORT(create_cb_matrix, __cb__ int16_t *dst, int64_t repeat, half value)
{
    RecordSet2DEvent<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #599
SANITIZER_REPORT(create_cb_matrix, __cb__ uint16_t *dst, int64_t repeat, half value)
{
    RecordSet2DEvent<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #600
SANITIZER_REPORT(create_cb_matrix, __cb__ half *dst, int64_t repeat, uint32_t value)
{
    RecordSet2DEvent<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #601
SANITIZER_REPORT(create_cb_matrix, __cb__ float *dst, int64_t repeat, uint32_t value)
{
    RecordSet2DEvent<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #602
SANITIZER_REPORT(create_cb_matrix, __cb__ float *dst, int64_t repeat, half value)
{
    RecordSet2DEvent<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #603
SANITIZER_REPORT(create_cb_matrix, __cb__ int16_t *dst, int64_t repeat, uint32_t value)
{
    RecordSet2DEvent<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #604
SANITIZER_REPORT(create_cb_matrix, __cb__ int32_t *dst, int64_t repeat, uint32_t value)
{
    RecordSet2DEvent<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #605
SANITIZER_REPORT(create_cb_matrix, __cb__ int32_t *dst, int64_t repeat, half value)
{
    RecordSet2DEvent<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #606
SANITIZER_REPORT(create_cb_matrix, __cb__ uint16_t *dst, int64_t repeat, uint32_t value)
{
    RecordSet2DEvent<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #607
SANITIZER_REPORT(create_cb_matrix, __cb__ uint32_t *dst, int64_t repeat, uint32_t value)
{
    RecordSet2DEvent<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #608
SANITIZER_REPORT(create_cb_matrix, __cb__ uint32_t *dst, int64_t repeat, half value)
{
    RecordSet2DEvent<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #613
SANITIZER_REPORT(create_cbuf_matrix, __cbuf__ void *dst, int64_t repeat, half value)
{
    RecordM200Set2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #614
SANITIZER_REPORT(create_cbuf_matrix, __cbuf__ __bf16 *dst, int64_t repeat, half value)
{
    RecordSet2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #615
SANITIZER_REPORT(create_cbuf_matrix, __cbuf__ __bf16 *dst, int64_t repeat, __bf16 value)
{
    RecordSet2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #616
SANITIZER_REPORT(create_cbuf_matrix, __cbuf__ __bf16 *dst, int64_t repeat, uint32_t value)
{
    RecordSet2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #617
SANITIZER_REPORT(create_cbuf_matrix, __cbuf__ half *dst, int64_t repeat, half value)
{
    RecordSet2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #618
SANITIZER_REPORT(create_cbuf_matrix, __cbuf__ int16_t *dst, int64_t repeat, half value)
{
    RecordSet2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #619
SANITIZER_REPORT(create_cbuf_matrix, __cbuf__ uint16_t *dst, int64_t repeat, half value)
{
    RecordSet2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #620
SANITIZER_REPORT(create_cbuf_matrix, __cbuf__ half *dst, int64_t repeat, uint32_t value)
{
    RecordSet2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #621
SANITIZER_REPORT(create_cbuf_matrix, __cbuf__ float *dst, int64_t repeat, uint32_t value)
{
    RecordSet2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #622
SANITIZER_REPORT(create_cbuf_matrix, __cbuf__ float *dst, int64_t repeat, half value)
{
    RecordSet2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #623
SANITIZER_REPORT(create_cbuf_matrix, __cbuf__ int16_t *dst, int64_t repeat, uint32_t value)
{
    RecordSet2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #624
SANITIZER_REPORT(create_cbuf_matrix, __cbuf__ int32_t *dst, int64_t repeat, uint32_t value)
{
    RecordSet2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #625
SANITIZER_REPORT(create_cbuf_matrix, __cbuf__ int32_t *dst, int64_t repeat, half value)
{
    RecordSet2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #626
SANITIZER_REPORT(create_cbuf_matrix, __cbuf__ uint16_t *dst, int64_t repeat, uint32_t value)
{
    RecordSet2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #627
SANITIZER_REPORT(create_cbuf_matrix, __cbuf__ uint32_t *dst, int64_t repeat, uint32_t value)
{
    RecordSet2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}

// #628
SANITIZER_REPORT(create_cbuf_matrix, __cbuf__ uint32_t *dst, int64_t repeat, half value)
{
    RecordSet2DEvent<MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), repeat);
}