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

SANITIZER_REPORT(copy_gm_to_ubuf, __ubuf__ void *dst, __gm__ void *src, uint64_t config)
{
    RecordDmaMovEvent<MemType::GM, MemType::UB>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config, PadMode::PAD_NONE,
                                                ByteMode::BM_DISABLE);
}

SANITIZER_REPORT(copy_ubuf_to_gm, __gm__ void *dst, __ubuf__ void *src, uint64_t config)
{
    RecordDmaMovEvent<MemType::UB, MemType::GM>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config,
                                                PadMode::PAD_NONE, ByteMode::BM_DISABLE);
}

SANITIZER_REPORT(copy_ubuf_to_gm_byte, __gm__ void *dst, __ubuf__ void *src, uint64_t config, bm_t byteMode)
{
    RecordDmaMovEvent<MemType::UB, MemType::GM>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config, PadMode::PAD_NONE,
                                                static_cast<ByteMode>(byteMode));
}

SANITIZER_REPORT(copy_cbuf_to_gm, __gm__ void *dst, __cbuf__ void *src, uint64_t config)
{
    RecordDmaMovEvent<MemType::L1, MemType::GM>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config,
                                                PadMode::PAD_NONE, ByteMode::BM_DISABLE);
}

SANITIZER_REPORT(copy_gm_to_cbuf, __cbuf__ void *dst, __gm__ void *src, uint64_t config, pad_t padMode)
{
    RecordDmaMovEvent<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config,
                                                static_cast<PadMode>(padMode), ByteMode::BM_DISABLE);
}

// #113
SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_b16, __cbuf__ void *dst, __gm__ void *src, uint64_t xm, uint64_t xt)
{
    RecordDmaMovNd2nzEvent<MemType::GM, MemType::L1, DataType::DATA_B16>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                                         reinterpret_cast<uint64_t>(src), xm, xt);
}

// #121
SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_b32s, __cbuf__ void *dst, __gm__ void *src, uint64_t xm, uint64_t xt)
{
    RecordDmaMovNd2nzEvent<MemType::GM, MemType::L1, DataType::DATA_B32>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                     reinterpret_cast<uint64_t>(src), xm, xt);
}

// #127
SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_b8, __cbuf__ void *dst, __gm__ void *src, uint64_t xm, uint64_t xt)
{
    RecordDmaMovNd2nzEvent<MemType::GM, MemType::L1, DataType::DATA_B8>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                     reinterpret_cast<uint64_t>(src), xm, xt);
}


SANITIZER_REPORT(copy_gm_to_cbuf_v2, __cbuf__ void *dst, __gm__ void *src, uint64_t config0, uint64_t config1)
{
    RecordDmaMovV2Event<MemType::GM, MemType::L1, DataType::DATA_B32>(
        EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config0, config1);
}

SANITIZER_REPORT(nd_copy_gm_to_ubuf_b8, __ubuf__ void *dst, __gm__ void *src, uint64_t config0, uint64_t config1)
{
    RecordNdDMAOut2Ub<DataType::DATA_B8>(
        EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config0, config1);
}

SANITIZER_REPORT(nd_copy_gm_to_ubuf_b16, __ubuf__ void *dst, __gm__ void *src, uint64_t config0, uint64_t config1)
{
    RecordNdDMAOut2Ub<DataType::DATA_B16>(
        EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config0, config1);
}

SANITIZER_REPORT(nd_copy_gm_to_ubuf_b32, __ubuf__ void *dst, __gm__ void *src, uint64_t config0, uint64_t config1)
{
    RecordNdDMAOut2Ub<DataType::DATA_B32>(
        EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config0, config1);
}

SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_d_b8, __cbuf__ void *dst, __gm__ void *src, uint64_t xm, uint64_t xt)
{
    RecordDmaMovNd2nzEventDav<MemType::GM, MemType::L1, DataType::DATA_B8>(
        EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), xm, xt);
}

SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_d_b16, __cbuf__ void *dst, __gm__ void *src, uint64_t xm, uint64_t xt)
{
    RecordDmaMovNd2nzEventDav<MemType::GM, MemType::L1, DataType::DATA_B16>(
        EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), xm, xt);
}

SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_d_b32, __cbuf__ void *dst, __gm__ void *src, uint64_t xm, uint64_t xt)
{
    RecordDmaMovNd2nzEventDav<MemType::GM, MemType::L1, DataType::DATA_B32>(
        EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), xm, xt);
}

SANITIZER_REPORT(copy_gm_to_cbuf_multi_dn2nz_d_b8, __cbuf__ void *dst, __gm__ void *src, uint64_t xm, uint64_t xt)
{
    RecordDmaMovDn2nzEventDav<MemType::GM, MemType::L1, DataType::DATA_B8>(
        EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), xm, xt);
}

SANITIZER_REPORT(copy_gm_to_cbuf_multi_dn2nz_d_b16, __cbuf__ void *dst, __gm__ void *src, uint64_t xm, uint64_t xt)
{
    RecordDmaMovDn2nzEventDav<MemType::GM, MemType::L1, DataType::DATA_B16>(
        EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), xm, xt);
}

SANITIZER_REPORT(copy_gm_to_cbuf_multi_dn2nz_d_b32, __cbuf__ void *dst, __gm__ void *src, uint64_t xm, uint64_t xt)
{
    RecordDmaMovDn2nzEventDav<MemType::GM, MemType::L1, DataType::DATA_B32>(
        EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), xm, xt);
}

SANITIZER_REPORT(set_l1_2d_b16, __cbuf__ void *dst, uint64_t src)
{
    RecordSetL12DEvent<DetailedDataType::B16>(
        EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), src);
}

SANITIZER_REPORT(set_l1_2d_b32, __cbuf__ void *dst, uint64_t src)
{
    RecordSetL12DEvent<DetailedDataType::B32>(
        EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), src);
}

SANITIZER_REPORT(copy_cbuf_to_ubuf, __ubuf__ void *dst, __cbuf__ void *src, uint64_t config)
{
    RecordDmaMovL2UBEvent<MemType::L1, MemType::UB>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config);
}

SANITIZER_REPORT(copy_ubuf_to_cbuf, __cbuf__ void *dst, __ubuf__ void *src, uint64_t config)
{
    RecordDmaMovUB2L1Event<MemType::UB, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config);
}
 
SANITIZER_REPORT(copy_ubuf_to_ubuf, __ubuf__ void *dst, __ubuf__ void *src, uint64_t config)
{
    RecordDmaMovUB2UBEvent<MemType::UB, MemType::UB>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config);
}

SANITIZER_REPORT(copy_cbuf_to_bt_f32, uint64_t dst, __cbuf__ void *src, uint64_t config)
{
    RecordDmaMovL1BtEvent<DetailedDataType::FLOAT>(EXTRA_PARAMS, dst, reinterpret_cast<uint64_t>(src), config);
}

SANITIZER_REPORT(copy_cbuf_to_bt_s32, uint64_t dst, __cbuf__ void *src, uint64_t config)
{
    RecordDmaMovL1BtEvent<DetailedDataType::S32>(EXTRA_PARAMS, dst, reinterpret_cast<uint64_t>(src), config);
}

SANITIZER_REPORT(copy_cbuf_to_bt_f16, uint64_t dst, __cbuf__ void *src, uint64_t config)
{
    RecordDmaMovL1BtEvent<DetailedDataType::F16>(EXTRA_PARAMS, dst, reinterpret_cast<uint64_t>(src), config);
}

SANITIZER_REPORT(copy_cbuf_to_bt_b16, uint64_t dst, __cbuf__ void *src, uint64_t config)
{
    RecordDmaMovL1BtEvent<DetailedDataType::B16>(EXTRA_PARAMS, dst, reinterpret_cast<uint64_t>(src), config);
}

SANITIZER_REPORT(copy_cbuf_to_fbuf, __fbuf__ void *dst, __cbuf__ void *src, uint64_t config)
{
    RecordDmaMovL1FbEvent(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config);
}