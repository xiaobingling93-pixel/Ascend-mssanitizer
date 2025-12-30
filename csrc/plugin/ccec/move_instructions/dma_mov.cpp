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

// #466
SANITIZER_REPORT(copy_ubuf_to_cbuf, __cbuf__ void *dst, __ubuf__ void *src, uint64_t config)
{
    RecordDmaMovEvent<MemType::UB, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config,
                                                PadMode::PAD_NONE, ByteMode::BM_DISABLE);
}
// #469
SANITIZER_REPORT(copy_ubuf_to_cbuf, __cbuf__ void *dst, __ubuf__ void *src, uint8_t sid, uint16_t nBurst,
                 uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride)
{
    RecordDmaMovEvent<MemType::UB, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), sid, nBurst, lenBurst,
                                                srcStride, dstStride, PadMode::PAD_NONE, ByteMode::BM_DISABLE);
}

// #75
SANITIZER_REPORT(copy_cbuf_to_ubuf, __ubuf__ void *dst, __cbuf__ void *src, uint64_t config)
{
    RecordDmaMovEvent<MemType::L1, MemType::UB>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config,
                                                PadMode::PAD_NONE, ByteMode::BM_DISABLE);
}

// #78
SANITIZER_REPORT(copy_cbuf_to_ubuf, __ubuf__ void *dst, __cbuf__ void *src, uint8_t sid, uint16_t nBurst,
                 uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride)
{
    RecordDmaMovEvent<MemType::L1, MemType::UB>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), sid, nBurst, lenBurst,
                                                srcStride, dstStride, PadMode::PAD_NONE, ByteMode::BM_DISABLE);
}


SANITIZER_REPORT(copy_cbuf_to_gm, __gm__ void *dst, __cbuf__ void *src, uint64_t config)
{
    RecordDmaMovEvent<MemType::L1, MemType::GM>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config,
                                                PadMode::PAD_NONE, ByteMode::BM_DISABLE);
}

SANITIZER_REPORT(copy_cbuf_to_gm, __gm__ void *dst, __cbuf__ void *src, uint8_t sid, uint16_t nBurst,
                 uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride)
{
    RecordDmaMovEvent<MemType::L1, MemType::GM>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), sid, nBurst, lenBurst,
                                                srcStride, dstStride, PadMode::PAD_NONE, ByteMode::BM_DISABLE);
}

SANITIZER_REPORT(copy_gm_to_cbuf, __cbuf__ void *dst, __gm__ void *src, uint64_t config, pad_t padMode)
{
    RecordDmaMovEvent<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config,
                                                static_cast<PadMode>(padMode), ByteMode::BM_DISABLE);
}

SANITIZER_REPORT(copy_gm_to_cbuf, __cbuf__ void *dst, __gm__ void *src, uint8_t sid, uint16_t nBurst,
                 uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride, pad_t padMode)
{
    RecordDmaMovEvent<MemType::GM, MemType::L1>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), sid, nBurst, lenBurst,
                                                srcStride, dstStride, static_cast<PadMode>(padMode),
                                                ByteMode::BM_DISABLE);
}

// #113
SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_b16, __cbuf__ __bf16 *dst, __gm__ __bf16 *src, uint64_t xm, uint64_t xt)
{
    RecordDmaMovNd2nzEvent<MemType::GM, MemType::L1, DataType::DATA_B16>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                                         reinterpret_cast<uint64_t>(src), xm, xt);
}

// #114
SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_b16, __cbuf__ __bf16 *dst, __gm__ __bf16 *src, uint8_t sid,
                 uint16_t ndNum, uint16_t nValue, uint16_t dValue, uint16_t srcNdMatrixStride, uint16_t srcDValue,
                 uint16_t dstNzC0Stride, uint16_t dstNzNStride, uint16_t dstNzMatrixStride)
{
    RecordDmaMovNd2nzEvent<MemType::GM, MemType::L1, DataType::DATA_B16>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                                         reinterpret_cast<uint64_t>(src), ndNum,
                                                                         nValue, dValue, srcNdMatrixStride, srcDValue,
                                                                         dstNzC0Stride, dstNzNStride,
                                                                         dstNzMatrixStride);
}

// #115
SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_b16, __cbuf__ half *dst, __gm__ half *src, uint64_t xm, uint64_t xt)
{
    RecordDmaMovNd2nzEvent<MemType::GM, MemType::L1, DataType::DATA_B16>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                                         reinterpret_cast<uint64_t>(src), xm, xt);
}

// #116
SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_b16, __cbuf__ half *dst, __gm__ half *src, uint8_t sid, uint16_t ndNum,
                 uint16_t nValue, uint16_t dValue, uint16_t srcNdMatrixStride, uint16_t srcDValue,
                 uint16_t dstNzC0Stride, uint16_t dstNzNStride, uint16_t dstNzMatrixStride)
{
    RecordDmaMovNd2nzEvent<MemType::GM, MemType::L1, DataType::DATA_B16>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                                         reinterpret_cast<uint64_t>(src), ndNum,
                                                                         nValue, dValue, srcNdMatrixStride,
                                                                         srcDValue, dstNzC0Stride, dstNzNStride,
                                                                         dstNzMatrixStride);
}

// #117
SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_b16, __cbuf__ int16_t *dst, __gm__ int16_t *src, uint64_t xm, uint64_t xt)
{
    RecordDmaMovNd2nzEvent<MemType::GM, MemType::L1, DataType::DATA_B16>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                                         reinterpret_cast<uint64_t>(src), xm, xt);
}

// #118
SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_b16, __cbuf__ int16_t *dst, __gm__ int16_t *src, uint8_t sid,
                 uint16_t ndNum, uint16_t nValue, uint16_t dValue, uint16_t srcNdMatrixStride, uint16_t srcDValue,
                 uint16_t dstNzC0Stride, uint16_t dstNzNStride, uint16_t dstNzMatrixStride)
{
    RecordDmaMovNd2nzEvent<MemType::GM, MemType::L1, DataType::DATA_B16>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                                         reinterpret_cast<uint64_t>(src), ndNum,
                                                                         nValue, dValue, srcNdMatrixStride, srcDValue,
                                                                         dstNzC0Stride, dstNzNStride,
                                                                         dstNzMatrixStride);
}

// #119
SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_b16, __cbuf__ uint16_t *dst, __gm__ uint16_t *src,
                 uint64_t xm, uint64_t xt)
{
    RecordDmaMovNd2nzEvent<MemType::GM, MemType::L1, DataType::DATA_B16>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                                         reinterpret_cast<uint64_t>(src), xm, xt);
}

// #120
SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_b16, __cbuf__ uint16_t *dst, __gm__ uint16_t *src, uint8_t sid,
                 uint16_t ndNum, uint16_t nValue, uint16_t dValue, uint16_t srcNdMatrixStride, uint16_t srcDValue,
                 uint16_t dstNzC0Stride, uint16_t dstNzNStride, uint16_t dstNzMatrixStride)
{
    RecordDmaMovNd2nzEvent<MemType::GM, MemType::L1, DataType::DATA_B16>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                                         reinterpret_cast<uint64_t>(src), ndNum,
                                                                         nValue, dValue, srcNdMatrixStride, srcDValue,
                                                                         dstNzC0Stride, dstNzNStride,
                                                                         dstNzMatrixStride);
}

// #121
SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_b32s, __cbuf__ float *dst, __gm__ float *src, uint64_t xm, uint64_t xt)
{
    RecordDmaMovNd2nzEvent<MemType::GM, MemType::L1, DataType::DATA_B32>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                     reinterpret_cast<uint64_t>(src), xm, xt);
}

// #122
SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_b32s, __cbuf__ float *dst, __gm__ float *src, uint8_t sid, uint16_t ndNum,
                 uint16_t nValue, uint16_t dValue, uint16_t srcNdMatrixStride, uint16_t srcDValue,
                 uint16_t dstNzC0Stride, uint16_t dstNzNStride, uint16_t dstNzMatrixStride)
{
    RecordDmaMovNd2nzEvent<MemType::GM, MemType::L1, DataType::DATA_B32>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                                         reinterpret_cast<uint64_t>(src), ndNum,
                                                                         nValue, dValue, srcNdMatrixStride, srcDValue,
                                                                         dstNzC0Stride, dstNzNStride,
                                                                         dstNzMatrixStride);
}

// #123
SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_b32s, __cbuf__ int32_t *dst, __gm__ int32_t *src,
                 uint64_t xm, uint64_t xt)
{
    RecordDmaMovNd2nzEvent<MemType::GM, MemType::L1, DataType::DATA_B32>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                     reinterpret_cast<uint64_t>(src), xm, xt);
}

// #124
SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_b32s, __cbuf__ int32_t *dst, __gm__ int32_t *src, uint8_t sid,
                 uint16_t ndNum, uint16_t nValue, uint16_t dValue, uint16_t srcNdMatrixStride, uint16_t srcDValue,
                 uint16_t dstNzC0Stride, uint16_t dstNzNStride, uint16_t dstNzMatrixStride)
{
    RecordDmaMovNd2nzEvent<MemType::GM, MemType::L1, DataType::DATA_B32>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                                         reinterpret_cast<uint64_t>(src), ndNum,
                                                                         nValue, dValue, srcNdMatrixStride, srcDValue,
                                                                         dstNzC0Stride, dstNzNStride,
                                                                         dstNzMatrixStride);
}

// #125
SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_b32s, __cbuf__ uint32_t *dst, __gm__ uint32_t *src,
                 uint64_t xm, uint64_t xt)
{
    RecordDmaMovNd2nzEvent<MemType::GM, MemType::L1, DataType::DATA_B32>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                     reinterpret_cast<uint64_t>(src), xm, xt);
}

// #126
SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_b32s, __cbuf__ uint32_t *dst, __gm__ uint32_t *src, uint8_t sid,
                 uint16_t ndNum, uint16_t nValue, uint16_t dValue, uint16_t srcNdMatrixStride, uint16_t srcDValue,
                 uint16_t dstNzC0Stride, uint16_t dstNzNStride, uint16_t dstNzMatrixStride)
{
    RecordDmaMovNd2nzEvent<MemType::GM, MemType::L1, DataType::DATA_B32>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                                         reinterpret_cast<uint64_t>(src), ndNum,
                                                                         nValue, dValue, srcNdMatrixStride, srcDValue,
                                                                         dstNzC0Stride, dstNzNStride,
                                                                         dstNzMatrixStride);
}

// #127
SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_b8, __cbuf__ int8_t *dst, __gm__ int8_t *src, uint64_t xm, uint64_t xt)
{
    RecordDmaMovNd2nzEvent<MemType::GM, MemType::L1, DataType::DATA_B8>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                     reinterpret_cast<uint64_t>(src), xm, xt);
}

// #128
SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_b8, __cbuf__ int8_t *dst, __gm__ int8_t *src, uint8_t sid, uint16_t ndNum,
                 uint16_t nValue, uint16_t dValue, uint16_t srcNdMatrixStride, uint16_t srcDValue,
                 uint16_t dstNzC0Stride, uint16_t dstNzNStride, uint16_t dstNzMatrixStride)
{
    RecordDmaMovNd2nzEvent<MemType::GM, MemType::L1, DataType::DATA_B8>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                                        reinterpret_cast<uint64_t>(src), ndNum, nValue,
                                                                        dValue, srcNdMatrixStride, srcDValue,
                                                                        dstNzC0Stride, dstNzNStride,
                                                                        dstNzMatrixStride);
}

// #129
SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_b8, __cbuf__ uint8_t *dst, __gm__ uint8_t *src, uint64_t xm, uint64_t xt)
{
    RecordDmaMovNd2nzEvent<MemType::GM, MemType::L1, DataType::DATA_B8>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                     reinterpret_cast<uint64_t>(src), xm, xt);
}

// #130
SANITIZER_REPORT(copy_gm_to_cbuf_multi_nd2nz_b8, __cbuf__ uint8_t *dst, __gm__ uint8_t *src, uint8_t sid,
                 uint16_t ndNum, uint16_t nValue, uint16_t dValue, uint16_t srcNdMatrixStride, uint16_t srcDValue,
                 uint16_t dstNzC0Stride, uint16_t dstNzNStride, uint16_t dstNzMatrixStride)
{
    RecordDmaMovNd2nzEvent<MemType::GM, MemType::L1, DataType::DATA_B8>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                                        reinterpret_cast<uint64_t>(src), ndNum, nValue,
                                                                        dValue, srcNdMatrixStride, srcDValue,
                                                                        dstNzC0Stride, dstNzNStride,
                                                                        dstNzMatrixStride);
}

SANITIZER_REPORT(copy_gm_to_ubuf, __ubuf__ void *dst, __gm__ void *src, uint64_t config)
{
    RecordDmaMovEvent<MemType::GM, MemType::UB>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config, PadMode::PAD_NONE,
                                                ByteMode::BM_DISABLE);
}

SANITIZER_REPORT(copy_gm_to_ubuf, __ubuf__ void *dst, __gm__ void *src, uint8_t sid, uint16_t nBurst,
                 uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride)
{
    RecordDmaMovEvent<MemType::GM, MemType::UB>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), sid, nBurst, lenBurst,
                                                srcStride, dstStride, PadMode::PAD_NONE, ByteMode::BM_DISABLE);
}

SANITIZER_REPORT(copy_ubuf_to_gm, __gm__ void *dst, __ubuf__ void *src, uint64_t config, bm_t byteMode)
{
    RecordDmaMovEvent<MemType::UB, MemType::GM>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config, PadMode::PAD_NONE,
                                                static_cast<ByteMode>(byteMode));
}

SANITIZER_REPORT(copy_ubuf_to_gm, __gm__ void *dst, __ubuf__ void *src, uint64_t config)
{
    RecordDmaMovEvent<MemType::UB, MemType::GM>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config,
                                                PadMode::PAD_NONE, ByteMode::BM_DISABLE);
}

SANITIZER_REPORT(copy_ubuf_to_gm, __gm__ void *dst, __ubuf__ void *src, uint8_t sid, uint16_t nBurst,
                 uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride)
{
    RecordDmaMovEvent<MemType::UB, MemType::GM>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), sid, nBurst, lenBurst,
                                                srcStride, dstStride, PadMode::PAD_NONE, ByteMode::BM_DISABLE);
}

SANITIZER_REPORT(copy_ubuf_to_gm, __gm__ void *dst, __ubuf__ void *src, uint8_t sid, uint16_t nBurst,
                 uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride, bm_t byteMode)
{
    RecordDmaMovEvent<MemType::UB, MemType::GM>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), sid, nBurst, lenBurst,
                                                srcStride, dstStride, PadMode::PAD_NONE,
                                                static_cast<ByteMode>(byteMode));
}

SANITIZER_REPORT(copy_ubuf_to_ubuf, __ubuf__ void *dst, __ubuf__ void *src, uint64_t config)
{
    RecordDmaMovEvent<MemType::UB, MemType::UB>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), config,
                                                PadMode::PAD_NONE, ByteMode::BM_DISABLE);
}

SANITIZER_REPORT(copy_ubuf_to_ubuf, __ubuf__ void *dst, __ubuf__ void *src, uint8_t sid, uint16_t nBurst,
                 uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride)
{
    RecordDmaMovEvent<MemType::UB, MemType::UB>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                reinterpret_cast<uint64_t>(src), sid, nBurst, lenBurst,
                                                srcStride, dstStride, PadMode::PAD_NONE, ByteMode::BM_DISABLE);
}

SANITIZER_REPORT(copy_vector_cc_to_ubuf, __ubuf__ half *dst, __cc__ half *src, uint64_t config, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B16, DataType::DATA_B16,
                                RecordType::DMA_MOV_CONV_RELU>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), config, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_vector_cc_to_ubuf, __ubuf__ half *dst, __cc__ half *src, uint8_t sid, uint16_t nBurst,
                uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B16, DataType::DATA_B16,
                                RecordType::DMA_MOV_CONV_RELU>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), sid, nBurst, lenBurst, srcStride,
                                dstStride, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_vector_cc_to_ubuf, __ubuf__ half *dst, __cc__ float *src, uint64_t config, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B16,
                                RecordType::DMA_MOV_CONV_RELU>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), config, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_vector_cc_to_ubuf, __ubuf__ half *dst, __cc__ float *src, uint8_t sid, uint16_t nBurst,
                uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B16,
                                RecordType::DMA_MOV_CONV_RELU>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), sid, nBurst, lenBurst, srcStride,
                                dstStride, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_vector_cc_to_ubuf, __ubuf__ float *dst, __cc__ float *src, uint64_t config, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B32,
                                RecordType::DMA_MOV_CONV_RELU>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), config, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_vector_cc_to_ubuf, __ubuf__ float *dst, __cc__ float *src, uint8_t sid, uint16_t nBurst,
                uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B32,
                                RecordType::DMA_MOV_CONV_RELU>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), sid, nBurst, lenBurst, srcStride,
                                dstStride, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_vector_cc_to_ubuf, __ubuf__ half *dst, __cc__ int32_t *src, uint64_t config, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B16,
                                RecordType::DMA_MOV_CONV_RELU>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), config, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_vector_cc_to_ubuf, __ubuf__ half *dst, __cc__ int32_t *src, uint8_t sid, uint16_t nBurst,
                uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B16,
                                RecordType::DMA_MOV_CONV_RELU>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), sid, nBurst, lenBurst, srcStride,
                                dstStride, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_vector_cc_to_ubuf, __ubuf__ int16_t *dst, __cc__ int32_t *src, uint64_t config, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B16,
                                RecordType::DMA_MOV_CONV_RELU>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), config, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_vector_cc_to_ubuf, __ubuf__ int16_t *dst, __cc__ int32_t *src, uint8_t sid, uint16_t nBurst,
                uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B16,
                                RecordType::DMA_MOV_CONV_RELU>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), sid, nBurst, lenBurst, srcStride,
                                dstStride, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_vector_cc_to_ubuf, __ubuf__ int32_t *dst, __cc__ int32_t *src, uint64_t config, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B32,
                                RecordType::DMA_MOV_CONV_RELU>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), config, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_vector_cc_to_ubuf, __ubuf__ int32_t *dst, __cc__ int32_t *src, uint8_t sid, uint16_t nBurst,
                uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B32,
                                RecordType::DMA_MOV_CONV_RELU>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), sid, nBurst, lenBurst, srcStride,
                                dstStride, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_vector_cc_to_ubuf, __ubuf__ int8_t *dst, __cc__ int32_t *src, uint64_t config, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B8,
                                RecordType::DMA_MOV_CONV_RELU>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), config, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_vector_cc_to_ubuf, __ubuf__ int8_t *dst, __cc__ int32_t *src, uint8_t sid, uint16_t nBurst,
                uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B8,
                                RecordType::DMA_MOV_CONV_RELU>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), sid, nBurst, lenBurst, srcStride,
                                dstStride, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_vector_cc_to_ubuf, __ubuf__ uint32_t *dst, __cc__ uint32_t *src, uint64_t config,
                ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B32,
                                RecordType::DMA_MOV_CONV_RELU>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), config, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_vector_cc_to_ubuf, __ubuf__ uint32_t *dst, __cc__ uint32_t *src, uint8_t sid, uint16_t nBurst,
                uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B32,
                                RecordType::DMA_MOV_CONV_RELU>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), sid, nBurst, lenBurst, srcStride,
                                dstStride, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_vector_cc_to_ubuf, __ubuf__ uint8_t *dst, __cc__ int32_t *src, uint64_t config, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B8,
                                RecordType::DMA_MOV_CONV_RELU>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), config, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_vector_cc_to_ubuf, __ubuf__ uint8_t *dst, __cc__ int32_t *src, uint8_t sid, uint16_t nBurst,
                uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B8,
                                RecordType::DMA_MOV_CONV_RELU>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), sid, nBurst, lenBurst, srcStride,
                                dstStride, static_cast<ConvRelu>(crMode));
}

// #81
SANITIZER_REPORT(copy_depthwise_cc_to_ubuf, __ubuf__ half *dst, __cc__ half *src, uint64_t config, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B16, DataType::DATA_B16,
                                RecordType::DMA_MOV_DEPTH_WISE>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), config, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_depthwise_cc_to_ubuf, __ubuf__ half *dst, __cc__ half *src, uint8_t sid, uint16_t nBurst,
                int16_t lenBurst, uint16_t srcStride, uint16_t dstStride, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B16, DataType::DATA_B16,
                                RecordType::DMA_MOV_DEPTH_WISE>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), sid, nBurst, lenBurst, srcStride,
                                dstStride, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_depthwise_cc_to_ubuf, __ubuf__ float *dst, __cc__ half *src, uint64_t config, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B16, DataType::DATA_B32,
                                RecordType::DMA_MOV_DEPTH_WISE>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), config, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_depthwise_cc_to_ubuf, __ubuf__ float *dst, __cc__ half *src, uint8_t sid, uint16_t nBurst,
                uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B16, DataType::DATA_B32,
                                RecordType::DMA_MOV_DEPTH_WISE>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), sid, nBurst, lenBurst, srcStride,
                                dstStride, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_depthwise_cc_to_ubuf, __ubuf__ half *dst, __cc__ float *src, uint64_t config, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B16,
                                RecordType::DMA_MOV_DEPTH_WISE>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), config, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_depthwise_cc_to_ubuf, __ubuf__ half *dst, __cc__ float *src, uint8_t sid, uint16_t nBurst,
                uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B16,
                                RecordType::DMA_MOV_DEPTH_WISE>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), sid, nBurst, lenBurst, srcStride,
                                dstStride, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_depthwise_cc_to_ubuf, __ubuf__ float *dst, __cc__ float *src, uint64_t config, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B32,
                                RecordType::DMA_MOV_DEPTH_WISE>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), config, static_cast<ConvRelu>(crMode));
}

SANITIZER_REPORT(copy_depthwise_cc_to_ubuf, __ubuf__ float *dst, __cc__ float *src, uint8_t sid, uint16_t nBurst,
                uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride, ConvRelu_t crMode)
{
    RecordDmaMovConvReluEvent<MemType::L0C, MemType::UB, DataType::DATA_B32, DataType::DATA_B32,
                                RecordType::DMA_MOV_DEPTH_WISE>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                reinterpret_cast<uint64_t>(src), sid, nBurst, lenBurst, srcStride,
                                dstStride, static_cast<ConvRelu>(crMode));
}