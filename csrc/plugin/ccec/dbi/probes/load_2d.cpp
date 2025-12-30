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

SANITIZER_REPORT(load_gm_to_ca, __ca__ void *dst, __gm__ void *src, uint64_t config, addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                 reinterpret_cast<uint64_t>(src), config,
                                                 static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_gm_to_cb, __cb__ void *dst, __gm__ void *src, uint64_t config, addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                 reinterpret_cast<uint64_t>(src), config,
                                                 static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_gm_to_cbuf, __cbuf__ void *dst, __gm__ void *src, uint64_t config, addr_cal_mode_t addr_cal_mode)
{
    RecordLoad2DEvent<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config,
                                                static_cast<AddrCalMode>(addr_cal_mode));
}

SANITIZER_REPORT(load_cbuf_to_cb_2d_transpose_b4, __cb__ void *dst, __cbuf__ void *src, uint64_t config0, uint64_t config1)
{
    RecordLoadL12DTransposeEvent<DetailedDataType::B4>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config0, config1);
}

SANITIZER_REPORT(load_cbuf_to_cb_2d_transpose_b8, __cb__ void *dst, __cbuf__ void *src, uint64_t config0, uint64_t config1)
{
    RecordLoadL12DTransposeEvent<DetailedDataType::B8>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config0, config1);
}

SANITIZER_REPORT(load_cbuf_to_cb_2d_transpose_b16, __cb__ void *dst, __cbuf__ void *src, uint64_t config0, uint64_t config1)
{
    RecordLoadL12DTransposeEvent<DetailedDataType::B16>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config0, config1);
}

SANITIZER_REPORT(load_cbuf_to_cb_2d_transpose_b32, __cb__ void *dst, __cbuf__ void *src, uint64_t config0, uint64_t config1)
{
    RecordLoadL12DTransposeEvent<DetailedDataType::B32>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config0, config1);
}

SANITIZER_REPORT(load_gm_to_cbuf_2dv2, __cbuf__ void *dst, __gm__ void *src, uint64_t config0, uint64_t config1)
{
    RecordLoad2DV2Event<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                  reinterpret_cast<uint64_t>(src), config0, config1);
}

SANITIZER_REPORT(load_cbuf_to_ca_2dv2_b4, __ca__ void *dst, __cbuf__ void *src, uint64_t config0, uint64_t config1, uint64_t transpose)
{
    RecordLoadL12DV2Event<MemType::L0A, DetailedDataType::B4>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                  reinterpret_cast<uint64_t>(src), config0, config1,
                                                  static_cast<TransposeMode>(transpose));
}

SANITIZER_REPORT(load_cbuf_to_ca_2dv2_b8, __ca__ void *dst, __cbuf__ void *src, uint64_t config0, uint64_t config1, uint64_t transpose)
{
    RecordLoadL12DV2Event<MemType::L0A, DetailedDataType::B8>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                  reinterpret_cast<uint64_t>(src), config0, config1,
                                                  static_cast<TransposeMode>(transpose));
}

SANITIZER_REPORT(load_cbuf_to_ca_2dv2_b16, __ca__ void *dst, __cbuf__ void *src, uint64_t config0, uint64_t config1, uint64_t transpose)
{
    RecordLoadL12DV2Event<MemType::L0A, DetailedDataType::B16>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                  reinterpret_cast<uint64_t>(src), config0, config1,
                                                  static_cast<TransposeMode>(transpose));
}

SANITIZER_REPORT(load_cbuf_to_ca_2dv2_b32, __ca__ void *dst, __cbuf__ void *src, uint64_t config0, uint64_t config1, uint64_t transpose)
{
    RecordLoadL12DV2Event<MemType::L0A, DetailedDataType::B32>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                  reinterpret_cast<uint64_t>(src), config0, config1,
                                                  static_cast<TransposeMode>(transpose));
}

SANITIZER_REPORT(load_cbuf_to_cb_2dv2_b4, __cb__ void *dst, __cbuf__ void *src, uint64_t config0, uint64_t config1, uint64_t transpose)
{
    RecordLoadL12DV2Event<MemType::L0B, DetailedDataType::B4>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                  reinterpret_cast<uint64_t>(src), config0, config1,
                                                  static_cast<TransposeMode>(transpose));
}

SANITIZER_REPORT(load_cbuf_to_cb_2dv2_b8, __cb__ void *dst, __cbuf__ void *src, uint64_t config0, uint64_t config1, uint64_t transpose)
{
    RecordLoadL12DV2Event<MemType::L0B, DetailedDataType::B8>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                  reinterpret_cast<uint64_t>(src), config0, config1,
                                                  static_cast<TransposeMode>(transpose));
}

SANITIZER_REPORT(load_cbuf_to_cb_2dv2_b16, __cb__ void *dst, __cbuf__ void *src, uint64_t config0, uint64_t config1, uint64_t transpose)
{
    RecordLoadL12DV2Event<MemType::L0B, DetailedDataType::B16>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                  reinterpret_cast<uint64_t>(src), config0, config1,
                                                  static_cast<TransposeMode>(transpose));
}

SANITIZER_REPORT(load_cbuf_to_cb_2dv2_b32, __cb__ void *dst, __cbuf__ void *src, uint64_t config0, uint64_t config1, uint64_t transpose)
{
    RecordLoadL12DV2Event<MemType::L0B, DetailedDataType::B32>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                  reinterpret_cast<uint64_t>(src), config0, config1,
                                                  static_cast<TransposeMode>(transpose));
}

SANITIZER_REPORT(load_cbuf_to_ca_mx_2dv2, uint64_t dst, __cbuf__ void *src, uint64_t config0, uint64_t config1)
{
    RecordLoadL1Mx2DV2Event<MemType::L0A>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                  reinterpret_cast<uint64_t>(src), config0, config1);
}

SANITIZER_REPORT(load_cbuf_to_cb_mx_2dv2, uint64_t dst, __cbuf__ void *src, uint64_t config0, uint64_t config1)
{
    RecordLoadL1Mx2DV2Event<MemType::L0B>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                  reinterpret_cast<uint64_t>(src), config0, config1);
}