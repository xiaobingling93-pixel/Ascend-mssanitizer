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

SANITIZER_REPORT(load_gm_to_cbuf, __cbuf__ __bf16 *dst, __gm__ __bf16 *src, uint64_t config)
{
    RecordLoad2DEvent<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config);
}

SANITIZER_REPORT(load_gm_to_cbuf, __cbuf__ __bf16 *dst, __gm__ __bf16 *src, uint64_t config,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1489
SANITIZER_REPORT(load_gm_to_cbuf, __cbuf__ half *dst, __gm__ half *src, uint64_t config)
{
    RecordLoad2DEvent<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config);
}

// #1490
SANITIZER_REPORT(load_gm_to_cbuf, __cbuf__ half *dst, __gm__ half *src, uint64_t config,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

// # 1491
SANITIZER_REPORT(load_gm_to_cbuf, __cbuf__ half *dst, __gm__ half *src, uint16_t baseIdx, uint8_t repeat,
                uint16_t srcStride, uint16_t dstStride, uint8_t sid, addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, dstStride,
                                                sid, static_cast<AddrCalMode>(addr_cal_mode));
}

// # 1492
SANITIZER_REPORT(load_gm_to_cbuf, __cbuf__ half *dst, __gm__ half *src, uint16_t baseIdx, uint8_t repeat,
                uint16_t srcStride, uint8_t sid)
{
    RecordLoad2DEvent<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, 0, sid);
}

SANITIZER_REPORT(load_gm_to_cbuf, __cbuf__ float *dst, __gm__ float *src, uint64_t config,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_gm_to_cbuf, __cbuf__ int32_t *dst, __gm__ int32_t *src, uint64_t config,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_gm_to_cbuf, __cbuf__ int8_t *dst, __gm__ int8_t *src, uint64_t config)
{
    RecordLoad2DEvent<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config);
}

SANITIZER_REPORT(load_gm_to_cbuf, __cbuf__ int8_t *dst, __gm__ int8_t *src, uint64_t config,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1497
SANITIZER_REPORT(load_gm_to_cbuf, __cbuf__ int8_t *dst, __gm__ int8_t *src, uint16_t baseIdx, uint8_t repeat,
                uint16_t srcStride, uint16_t dstStride, uint8_t sid, addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, dstStride,
                                                sid, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1498
SANITIZER_REPORT(load_gm_to_cbuf, __cbuf__ int8_t *dst, __gm__ int8_t *src, uint16_t baseIdx, uint8_t repeat,
                uint16_t srcStride, uint8_t sid)
{
    RecordLoad2DEvent<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, 0, sid);
}

SANITIZER_REPORT(load_gm_to_cbuf, __cbuf__ uint32_t *dst, __gm__ uint32_t *src, uint64_t config,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_gm_to_cbuf, __cbuf__ uint8_t *dst, __gm__ uint8_t *src, uint64_t config)
{
    RecordLoad2DEvent<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config);
}

SANITIZER_REPORT(load_gm_to_cbuf, __cbuf__ uint8_t *dst, __gm__ uint8_t *src, uint64_t config,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1503
SANITIZER_REPORT(load_gm_to_cbuf, __cbuf__ uint8_t *dst, __gm__ uint8_t *src, uint16_t baseIdx, uint8_t repeat,
                uint16_t srcStride, uint16_t dstStride, uint8_t sid, addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, dstStride,
                                                sid, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1504
SANITIZER_REPORT(load_gm_to_cbuf, __cbuf__ uint8_t *dst, __gm__ uint8_t *src, uint16_t baseIdx, uint8_t repeat,
                uint16_t srcStride, uint8_t sid)
{
    RecordLoad2DEvent<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, 0, sid);
}

// #1512
SANITIZER_REPORT(load_gm_to_cbuf_s4, __cbuf__ void *dst, __gm__ void *src, uint64_t config,
                 addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1513
SANITIZER_REPORT(load_gm_to_cbuf_s4, __cbuf__ void *dst, __gm__ void *src, uint16_t baseIdx, uint8_t repeat,
                uint16_t srcStride, uint8_t sid, addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, 0, sid, static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_gm_to_ca, __ca__ __bf16 *dst, __gm__ __bf16 *src, uint64_t config)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config);
}

SANITIZER_REPORT(load_gm_to_ca, __ca__ __bf16 *dst, __gm__ __bf16 *src, uint64_t config,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}


SANITIZER_REPORT(load_gm_to_ca, __ca__ half *dst, __gm__ half *src, uint64_t config)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config);
}


SANITIZER_REPORT(load_gm_to_ca, __ca__ half *dst, __gm__ half *src, uint64_t config,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1347
SANITIZER_REPORT(load_gm_to_ca, __ca__ half *dst, __gm__ half *src, uint16_t baseIdx, uint8_t repeat,
                 uint16_t srcStride, uint16_t dstStride, uint8_t sid,
                 addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                 reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, dstStride,
                                                 sid, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1348
SANITIZER_REPORT(load_gm_to_ca, __ca__ half *dst, __gm__ half *src, uint16_t baseIdx,
                 uint8_t repeat, uint16_t srcStride, uint8_t sid)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, 0, sid);
}

SANITIZER_REPORT(load_gm_to_ca, __ca__ float *dst, __gm__ float *src, uint64_t config,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_gm_to_ca, __ca__ int32_t *dst, __gm__ int32_t *src, uint64_t config,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}


SANITIZER_REPORT(load_gm_to_ca, __ca__ int8_t *dst, __gm__ int8_t *src, uint64_t config)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config);
}


SANITIZER_REPORT(load_gm_to_ca, __ca__ int8_t *dst, __gm__ int8_t *src, uint64_t config,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1353
SANITIZER_REPORT(load_gm_to_ca, __ca__ int8_t *dst, __gm__ int8_t *src, uint16_t baseIdx, uint8_t repeat,
                 uint16_t srcStride, uint16_t dstStride, uint8_t sid,
                 addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                 reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, dstStride,
                                                 sid, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1354
SANITIZER_REPORT(load_gm_to_ca, __ca__ int8_t *dst, __gm__ int8_t *src, uint16_t baseIdx, uint8_t repeat,
                 uint16_t srcStride, uint8_t sid)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, 0, sid);
}

SANITIZER_REPORT(load_gm_to_ca, __ca__ uint32_t *dst, __gm__ uint32_t *src, uint64_t config,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}


SANITIZER_REPORT(load_gm_to_ca, __ca__ uint8_t *dst, __gm__ uint8_t *src, uint64_t config)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config);
}


SANITIZER_REPORT(load_gm_to_ca, __ca__ uint8_t *dst, __gm__ uint8_t *src, uint64_t config,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1359
SANITIZER_REPORT(load_gm_to_ca, __ca__ uint8_t *dst, __gm__ uint8_t *src, uint16_t baseIdx, uint8_t repeat,
                 uint16_t srcStride, uint16_t dstStride, uint8_t sid,
                 addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                 reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, dstStride,
                                                 sid, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1360
SANITIZER_REPORT(load_gm_to_ca, __ca__ uint8_t *dst, __gm__ uint8_t *src, uint16_t baseIdx, uint8_t repeat,
                 uint16_t srcStride, uint8_t sid)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, 0, sid);
}

// #1368
SANITIZER_REPORT(load_gm_to_ca_s4, __ca__ void *dst, __gm__ void *src, uint64_t config, addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                 reinterpret_cast<uint64_t>(src), config,
                                                 static_cast<AddrCalMode>(addr_cal_mode));
}

// #1369
SANITIZER_REPORT(load_gm_to_ca_s4, __ca__ void *dst, __gm__ void *src, uint16_t baseIdx,
                 uint8_t repeat, uint16_t srcStride, uint16_t dstStride,
                 uint8_t sid, addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                 reinterpret_cast<uint64_t>(src), baseIdx, repeat,
                                                 srcStride, dstStride, sid, static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_gm_to_cb, __cb__ __bf16 *dst, __gm__ __bf16 *src, uint64_t config)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config);
}

SANITIZER_REPORT(load_gm_to_cb, __cb__ __bf16 *dst, __gm__ __bf16 *src, uint64_t config,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_gm_to_cb, __cb__ half *dst, __gm__ half *src, uint64_t config)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config);
}

SANITIZER_REPORT(load_gm_to_cb, __cb__ half *dst, __gm__ half *src, uint64_t config,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_gm_to_cb, __cb__ half *dst, __gm__ half *src, uint16_t baseIdx, uint8_t repeat,
                 uint16_t srcStride, uint16_t dstStride, uint8_t sid, addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                 reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride,
                                                 dstStride, sid, static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_gm_to_cb, __cb__ half *dst, __gm__ half *src, uint16_t baseIdx,
                 uint8_t repeat, uint16_t srcStride, uint8_t sid)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, 0, sid);
}

SANITIZER_REPORT(load_gm_to_cb, __cb__ float *dst, __gm__ float *src, uint64_t config,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_gm_to_cb, __cb__ int32_t *dst, __gm__ int32_t *src, uint64_t config,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_gm_to_cb, __cb__ int8_t *dst, __gm__ int8_t *src, uint64_t config)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config);
}

SANITIZER_REPORT(load_gm_to_cb, __cb__ int8_t *dst, __gm__ int8_t *src, uint64_t config,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1425
SANITIZER_REPORT(load_gm_to_cb, __cb__ int8_t *dst, __gm__ int8_t *src, uint16_t baseIdx, uint8_t repeat,
                 uint16_t srcStride, uint16_t dstStride, uint8_t sid, addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                 reinterpret_cast<uint64_t>(src), baseIdx, repeat,
                                                 srcStride, dstStride, sid, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1426
SANITIZER_REPORT(load_gm_to_cb, __cb__ int8_t *dst, __gm__ int8_t *src, uint16_t baseIdx, uint8_t repeat,
                 uint16_t srcStride, uint8_t sid)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                 reinterpret_cast<uint64_t>(src),
                                                 baseIdx, repeat, srcStride, 0, sid);
}

SANITIZER_REPORT(load_gm_to_cb, __cb__ uint32_t *dst, __gm__ uint32_t *src, uint64_t config,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_gm_to_cb, __cb__ uint8_t *dst, __gm__ uint8_t *src, uint64_t config)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config);
}

SANITIZER_REPORT(load_gm_to_cb, __cb__ uint8_t *dst, __gm__ uint8_t *src, uint64_t config,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1431
SANITIZER_REPORT(load_gm_to_cb, __cb__ uint8_t *dst, __gm__ uint8_t *src, uint16_t baseIdx, uint8_t repeat,
                 uint16_t srcStride, uint16_t dstStride, uint8_t sid, addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                 reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride,
                                                 dstStride, sid, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1432
SANITIZER_REPORT(load_gm_to_cb, __cb__ uint8_t *dst, __gm__ uint8_t *src, uint16_t baseIdx, uint8_t repeat,
                 uint16_t srcStride, uint8_t sid)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                 reinterpret_cast<uint64_t>(src),
                                                 baseIdx, repeat, srcStride, 0, sid);
}

// $1440
SANITIZER_REPORT(load_gm_to_cb_s4, __cb__ void *dst, __gm__ void *src, uint64_t config, addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                 reinterpret_cast<uint64_t>(src),
                                                 config, static_cast<AddrCalMode>(addr_cal_mode));
}

// $1441
SANITIZER_REPORT(load_gm_to_cb_s4, __cb__ void *dst, __gm__ void *src, uint16_t baseIdx, uint8_t repeat,
                uint16_t srcStride, uint16_t dstStride, uint8_t sid, addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), baseIdx, repeat,
                                                srcStride, dstStride, sid, static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_cbuf_to_ca, __ca__ __bf16 *dst, __cbuf__ __bf16 *src, uint64_t config, bool transpose)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config);
}

SANITIZER_REPORT(load_cbuf_to_ca, __ca__ __bf16 *dst, __cbuf__ __bf16 *src, uint64_t config, bool transpose,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_cbuf_to_ca, __ca__ half *dst, __cbuf__ half *src, uint64_t config, bool transpose)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config);
}

SANITIZER_REPORT(load_cbuf_to_ca, __ca__ half *dst, __cbuf__ half *src, uint64_t config, bool transpose,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}
// #1137
SANITIZER_REPORT(load_cbuf_to_ca, __ca__ half *dst, __cbuf__ half *src, uint16_t baseIdx, uint8_t repeat,
                uint16_t srcStride, uint16_t dstStride, uint8_t sid, bool transpose, addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, dstStride,
        sid, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1138
SANITIZER_REPORT(load_cbuf_to_ca, __ca__ half *dst, __cbuf__ half *src, uint16_t baseIdx, uint8_t repeat,
                uint16_t srcStride, uint8_t sid, bool transpose)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, 0, sid);
}

SANITIZER_REPORT(load_cbuf_to_ca, __ca__ float *dst, __cbuf__ float *src, uint64_t config, bool transpose,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_cbuf_to_ca, __ca__ int32_t *dst, __cbuf__ int32_t *src, uint64_t config, bool transpose,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_cbuf_to_ca, __ca__ int8_t *dst, __cbuf__ int8_t *src, uint64_t config, bool transpose)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config);
}

SANITIZER_REPORT(load_cbuf_to_ca, __ca__ int8_t *dst, __cbuf__ int8_t *src, uint64_t config, bool transpose,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1143
SANITIZER_REPORT(load_cbuf_to_ca, __ca__ int8_t *dst, __cbuf__ int8_t *src, uint16_t baseIdx, uint8_t repeat,
                uint16_t srcStride, uint16_t dstStride, uint8_t sid, bool transpose, addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, dstStride,
        sid, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1144
SANITIZER_REPORT(load_cbuf_to_ca, __ca__ int8_t *dst, __cbuf__ int8_t *src, uint16_t baseIdx, uint8_t repeat,
                uint16_t srcStride, uint8_t sid, bool transpose)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, 0, sid);
}

SANITIZER_REPORT(load_cbuf_to_ca, __ca__ uint32_t *dst, __cbuf__ uint32_t *src, uint64_t config, bool transpose,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_cbuf_to_ca, __ca__ uint8_t *dst, __cbuf__ uint8_t *src, uint64_t config, bool transpose)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config);
}

SANITIZER_REPORT(load_cbuf_to_ca, __ca__ uint8_t *dst, __cbuf__ uint8_t *src, uint64_t config, bool transpose,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}
// #1149
SANITIZER_REPORT(load_cbuf_to_ca, __ca__ uint8_t *dst, __cbuf__ uint8_t *src, uint16_t baseIdx, uint8_t repeat,
                uint16_t srcStride, uint16_t dstStride, uint8_t sid, bool transpose, addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, dstStride,
        sid, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1150
SANITIZER_REPORT(load_cbuf_to_ca, __ca__ uint8_t *dst, __cbuf__ uint8_t *src, uint16_t baseIdx, uint8_t repeat,
                uint16_t srcStride, uint8_t sid, bool transpose)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, 0, sid);
}

// #1158
SANITIZER_REPORT(load_cbuf_to_ca_s4, __ca__ void *dst, __cbuf__ void *src, uint64_t config, bool transpose,
                addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1159
SANITIZER_REPORT(load_cbuf_to_ca_s4, __ca__ void *dst, __cbuf__ void *src, uint16_t baseIdx, uint8_t repeat,
                uint16_t srcStride, uint16_t dstStride, uint8_t sid, bool transpose, addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, dstStride,
        sid, static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_cbuf_to_cb, __cb__ __bf16 *dst, __cbuf__ __bf16 *src, uint64_t config, bool transpose)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config);
}

SANITIZER_REPORT(load_cbuf_to_cb, __cb__ __bf16 *dst, __cbuf__ __bf16 *src, uint64_t config, bool transpose,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_cbuf_to_cb, __cb__ half *dst, __cbuf__ half *src, uint64_t config, bool transpose)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config);
}

SANITIZER_REPORT(load_cbuf_to_cb, __cb__ half *dst, __cbuf__ half *src, uint64_t config, bool transpose,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_cbuf_to_cb, __cb__ half *dst, __cbuf__ half *src, uint16_t baseIdx, uint8_t repeat,
                uint16_t srcStride, uint16_t dstStride, uint8_t sid, bool transpose, addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, dstStride,
        sid, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1240
SANITIZER_REPORT(load_cbuf_to_cb, __cb__ half *dst, __cbuf__ half *src, uint16_t baseIdx, uint8_t repeat,
                uint16_t srcStride, uint8_t sid, bool transpose)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, 0, sid);
}

// #1239

SANITIZER_REPORT(load_cbuf_to_cb, __cb__ float *dst, __cbuf__ float *src, uint64_t config, bool transpose,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_cbuf_to_cb, __cb__ int32_t *dst, __cbuf__ int32_t *src, uint64_t config, bool transpose,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_cbuf_to_cb, __cb__ int8_t *dst, __cbuf__ int8_t *src, uint64_t config, bool transpose)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config);
}

SANITIZER_REPORT(load_cbuf_to_cb, __cb__ int8_t *dst, __cbuf__ int8_t *src, uint64_t config, bool transpose,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1245
SANITIZER_REPORT(load_cbuf_to_cb, __cb__ int8_t *dst, __cbuf__ int8_t *src, uint16_t baseIdx, uint8_t repeat,
                uint16_t srcStride, uint16_t dstStride, uint8_t sid, bool transpose, addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, dstStride,
        sid, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1246
SANITIZER_REPORT(load_cbuf_to_cb, __cb__ int8_t *dst, __cbuf__ int8_t *src, uint16_t baseIdx, uint8_t repeat,
                uint16_t srcStride, uint8_t sid, bool transpose)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, 0, sid);
}

SANITIZER_REPORT(load_cbuf_to_cb, __cb__ uint32_t *dst, __cbuf__ uint32_t *src, uint64_t config, bool transpose,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_cbuf_to_cb, __cb__ uint8_t *dst, __cbuf__ uint8_t *src, uint64_t config, bool transpose)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config);
}

SANITIZER_REPORT(load_cbuf_to_cb, __cb__ uint8_t *dst, __cbuf__ uint8_t *src, uint64_t config, bool transpose,
    addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1251
SANITIZER_REPORT(load_cbuf_to_cb, __cb__ uint8_t *dst, __cbuf__ uint8_t *src, uint16_t baseIdx, uint8_t repeat,
                uint16_t srcStride, uint16_t dstStride, uint8_t sid, bool transpose, addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, dstStride,
        sid, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1252
SANITIZER_REPORT(load_cbuf_to_cb, __cb__ uint8_t *dst, __cbuf__ uint8_t *src, uint16_t baseIdx, uint8_t repeat,
                uint16_t srcStride, uint8_t sid, bool transpose)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, 0, sid);
}

// #1270
SANITIZER_REPORT(load_cbuf_to_cb_s4, __cb__ void *dst, __cbuf__ void *src, uint64_t config, bool transpose,
                 addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1271
SANITIZER_REPORT(load_cbuf_to_cb_s4, __cb__ void *dst, __cbuf__ void *src, uint16_t baseIdx, uint8_t repeat,
                 uint16_t srcStride, uint16_t dstStride, uint8_t sid, bool transpose, addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::L1, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), baseIdx, repeat, srcStride, 0, sid, static_cast<AddrCalMode>(addr_cal_mode));
}

// #1272
SANITIZER_REPORT(load_cbuf_to_cb_sp, __cb__ int8_t *dst, __cbuf__ int8_t *src, uint64_t config)
{
    // internal buffer暂不考虑
    RecordLoad2DSparseEvent<MemType::L1, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config);
}

SANITIZER_REPORT(load_cbuf_to_cb_sp, __cb__ int8_t *dst, __cbuf__ int8_t *src, uint16_t startID, uint8_t repeat)
{
    RecordLoad2DSparseEvent<MemType::L1, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), startID, repeat);
}

SANITIZER_REPORT(load_cbuf_to_cb_transpose_s4, __cb__ void *dst, __cbuf__ void *src,
                uint64_t config, uint64_t fracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0B, DataType::DATA_B4>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config, fracStride);
}

SANITIZER_REPORT(load_cbuf_to_cb_transpose_s4, __cb__ void *dst, __cbuf__ void *src, uint16_t indexID,
                uint8_t repeat, uint16_t srcStride, uint16_t dstStride, bool addrmode, uint16_t dstFracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0B, DataType::DATA_B4>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), indexID, repeat, srcStride,
        dstStride, addrmode, dstFracStride);
}

// #1274
SANITIZER_REPORT(load_cbuf_to_cb_transpose, __cb__ __bf16 *dst, __cbuf__ __bf16 *src, uint64_t config,
                uint64_t fracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0B, DataType::DATA_B16>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config, fracStride);
}

SANITIZER_REPORT(load_cbuf_to_cb_transpose, __cb__ __bf16 *dst, __cbuf__ __bf16 *src, uint16_t indexID,
                uint8_t repeat, uint16_t srcStride, uint16_t dstStride, bool addrmode, uint16_t dstFracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0B, DataType::DATA_B16>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), indexID, repeat, srcStride,
        dstStride, addrmode, dstFracStride);
}

SANITIZER_REPORT(load_cbuf_to_cb_transpose, __cb__ half *dst, __cbuf__ half *src, uint64_t config, uint64_t fracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0B, DataType::DATA_B16>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config, fracStride);
}

SANITIZER_REPORT(load_cbuf_to_cb_transpose, __cb__ half *dst, __cbuf__ half *src, uint16_t indexID, uint8_t repeat,
                uint16_t srcStride, uint16_t dstStride, bool addrmode, uint16_t dstFracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0B, DataType::DATA_B16>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), indexID, repeat, srcStride,
        dstStride, addrmode, dstFracStride);
}

SANITIZER_REPORT(load_cbuf_to_cb_transpose, __cb__ float *dst, __cbuf__ float *src,
                uint64_t config, uint64_t fracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0B, DataType::DATA_B32>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config, fracStride);
}

SANITIZER_REPORT(load_cbuf_to_cb_transpose, __cb__ float *dst, __cbuf__ float *src, uint16_t indexID,
                uint8_t repeat, uint16_t srcStride, uint16_t dstStride, bool addrmode, uint16_t dstFracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0B, DataType::DATA_B32>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), indexID, repeat, srcStride,
        dstStride, addrmode, dstFracStride);
}

SANITIZER_REPORT(load_cbuf_to_cb_transpose, __cb__ int32_t *dst, __cbuf__ int32_t *src,
                uint64_t config, uint64_t fracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0B, DataType::DATA_B32>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config, fracStride);
}

SANITIZER_REPORT(load_cbuf_to_cb_transpose, __cb__ int32_t *dst, __cbuf__ int32_t *src, uint16_t indexID,
                uint8_t repeat, uint16_t srcStride, uint16_t dstStride, bool addrmode, uint16_t dstFracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0B, DataType::DATA_B32>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), indexID, repeat, srcStride,
        dstStride, addrmode, dstFracStride);
}

SANITIZER_REPORT(load_cbuf_to_cb_transpose, __cb__ int8_t *dst, __cbuf__ int8_t *src,
                uint64_t config, uint64_t fracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0B, DataType::DATA_B8>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config, fracStride);
}

SANITIZER_REPORT(load_cbuf_to_cb_transpose, __cb__ int8_t *dst, __cbuf__ int8_t *src, uint16_t indexID,
                uint8_t repeat, uint16_t srcStride, uint16_t dstStride, bool addrmode, uint16_t dstFracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0B, DataType::DATA_B8>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), indexID, repeat, srcStride,
        dstStride, addrmode, dstFracStride);
}

SANITIZER_REPORT(load_cbuf_to_cb_transpose, __cb__ uint32_t *dst, __cbuf__ uint32_t *src,
                uint64_t config, uint64_t fracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0B, DataType::DATA_B32>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config, fracStride);
}

SANITIZER_REPORT(load_cbuf_to_cb_transpose, __cb__ uint32_t *dst, __cbuf__ uint32_t *src, uint16_t indexID,
                uint8_t repeat, uint16_t srcStride, uint16_t dstStride, bool addrmode, uint16_t dstFracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0B, DataType::DATA_B32>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), indexID, repeat, srcStride,
        dstStride, addrmode, dstFracStride);
}

SANITIZER_REPORT(load_cbuf_to_cb_transpose, __cb__ uint8_t *dst, __cbuf__ uint8_t *src,
                uint64_t config, uint64_t fracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0B, DataType::DATA_B8>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config, fracStride);
}

SANITIZER_REPORT(load_cbuf_to_cb_transpose, __cb__ uint8_t *dst, __cbuf__ uint8_t *src, uint16_t indexID,
                uint8_t repeat, uint16_t srcStride, uint16_t dstStride, bool addrmode, uint16_t dstFracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0B, DataType::DATA_B8>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), indexID, repeat, srcStride,
        dstStride, addrmode, dstFracStride);
}

// #1160
SANITIZER_REPORT(load_cbuf_to_ca_transpose, __ca__ __bf16 *dst, __cbuf__ __bf16 *src, uint64_t config,
                uint64_t fracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0A, DataType::DATA_B16>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config, fracStride);
}

// #1161
SANITIZER_REPORT(load_cbuf_to_ca_transpose, __ca__ __bf16 *dst, __cbuf__ __bf16 *src, uint16_t indexID,
                uint8_t repeat, uint16_t srcStride, uint16_t dstStride, bool addrmode, uint16_t dstFracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0A, DataType::DATA_B16>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), indexID, repeat, srcStride,
        dstStride, addrmode, dstFracStride);
}

SANITIZER_REPORT(load_cbuf_to_ca_transpose, __ca__ half *dst, __cbuf__ half *src, uint64_t config, uint64_t fracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0A, DataType::DATA_B16>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config, fracStride);
}

SANITIZER_REPORT(load_cbuf_to_ca_transpose, __ca__ half *dst, __cbuf__ half *src, uint16_t indexID, uint8_t repeat,
                uint16_t srcStride, uint16_t dstStride, bool addrmode, uint16_t dstFracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0A, DataType::DATA_B16>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), indexID, repeat, srcStride,
        dstStride, addrmode, dstFracStride);
}

SANITIZER_REPORT(load_cbuf_to_ca_transpose, __ca__ float *dst, __cbuf__ float *src, uint64_t config,
                uint64_t fracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0A, DataType::DATA_B32>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config, fracStride);
}

SANITIZER_REPORT(load_cbuf_to_ca_transpose, __ca__ float *dst, __cbuf__ float *src, uint16_t indexID, uint8_t repeat,
                uint16_t srcStride, uint16_t dstStride, bool addrmode, uint16_t dstFracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0A, DataType::DATA_B32>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), indexID, repeat, srcStride,
        dstStride, addrmode, dstFracStride);
}

SANITIZER_REPORT(load_cbuf_to_ca_transpose, __ca__ int32_t *dst, __cbuf__ int32_t *src, uint64_t config,
                uint64_t fracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0A, DataType::DATA_B32>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config, fracStride);
}

SANITIZER_REPORT(load_cbuf_to_ca_transpose, __ca__ int32_t *dst, __cbuf__ int32_t *src, uint16_t indexID,
                uint8_t repeat, uint16_t srcStride, uint16_t dstStride, bool addrmode, uint16_t dstFracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0A, DataType::DATA_B32>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), indexID, repeat, srcStride,
        dstStride, addrmode, dstFracStride);
}

SANITIZER_REPORT(load_cbuf_to_ca_transpose, __ca__ int8_t *dst, __cbuf__ int8_t *src, uint64_t config,
                uint64_t fracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0A, DataType::DATA_B8>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config, fracStride);
}

SANITIZER_REPORT(load_cbuf_to_ca_transpose, __ca__ int8_t *dst, __cbuf__ int8_t *src, uint16_t indexID, uint8_t repeat,
                uint16_t srcStride, uint16_t dstStride, bool addrmode, uint16_t dstFracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0A, DataType::DATA_B8>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), indexID, repeat, srcStride,
        dstStride, addrmode, dstFracStride);
}

SANITIZER_REPORT(load_cbuf_to_ca_transpose, __ca__ uint32_t *dst, __cbuf__ uint32_t *src, uint64_t config,
                uint64_t fracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0A, DataType::DATA_B32>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config, fracStride);
}

SANITIZER_REPORT(load_cbuf_to_ca_transpose, __ca__ uint32_t *dst, __cbuf__ uint32_t *src, uint16_t indexID,
                uint8_t repeat, uint16_t srcStride, uint16_t dstStride, bool addrmode, uint16_t dstFracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0A, DataType::DATA_B32>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), indexID, repeat, srcStride,
        dstStride, addrmode, dstFracStride);
}

SANITIZER_REPORT(load_cbuf_to_ca_transpose, __ca__ uint8_t *dst, __cbuf__ uint8_t *src, uint64_t config,
                uint64_t fracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0A, DataType::DATA_B8>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config, fracStride);
}

SANITIZER_REPORT(load_cbuf_to_ca_transpose, __ca__ uint8_t *dst, __cbuf__ uint8_t *src, uint16_t indexID,
                uint8_t repeat, uint16_t srcStride, uint16_t dstStride, bool addrmode, uint16_t dstFracStride)
{
    RecordLoad2DTransposeEvent<MemType::L1, MemType::L0A, DataType::DATA_B8>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), indexID, repeat, srcStride,
        dstStride, addrmode, dstFracStride);
}