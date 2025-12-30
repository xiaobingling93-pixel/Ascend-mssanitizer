
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


#include "plugin/kernel_pub_func.h"
#include "plugin/utils.h"
#include "plugin/record_move_instructions.h"

using namespace Sanitizer;

// #1174
SANITIZER_REPORT(load_cbuf_to_ca_winograd, __ca__ half *dst, __cbuf__ half *src, uint64_t config0, uint64_t config1)
{
    RecordLoadAWinograd<MemType::L1, MemType::L0A, DataType::DATA_B16>(EXTRA_PARAMS, dst, src, config0, config1);
}

// #1175
SANITIZER_REPORT(load_cbuf_to_ca_winograd, __ca__ half *dst, __cbuf__ half *src, uint16_t FMWidth, uint16_t FMHeight,
                 uint16_t FMChannel, uint8_t dstGap, uint8_t colIndicator, uint8_t padModeHc, uint8_t padModeV,
                 uint16_t stepK, uint16_t posK, uint16_t stepM, uint16_t posM)
{
    RecordLoadAWinograd<MemType::L1, MemType::L0A, DataType::DATA_B16>(EXTRA_PARAMS, dst, src, FMWidth, FMHeight,
                                                                       FMChannel, dstGap, colIndicator, padModeHc,
                                                                       padModeV, stepK, posK, stepM, posM);
}

// #1176
SANITIZER_REPORT(load_cbuf_to_ca_winograd, __ca__ int8_t *dst, __cbuf__ int8_t *src, uint64_t config0, uint64_t config1)
{
    RecordLoadAWinograd<MemType::L1, MemType::L0A, DataType::DATA_B8>(EXTRA_PARAMS, dst, src, config0, config1);
}

// #1177
SANITIZER_REPORT(load_cbuf_to_ca_winograd, __ca__ int8_t *dst, __cbuf__ int8_t *src, uint16_t FMWidth,
                 uint16_t FMHeight, uint16_t FMChannel, uint8_t dstGap, uint8_t colIndicator, uint8_t padModeHc,
                 uint8_t padModeV, uint16_t stepK, uint16_t posK, uint16_t stepM, uint16_t posM)
{
    RecordLoadAWinograd<MemType::L1, MemType::L0A, DataType::DATA_B8>(EXTRA_PARAMS, dst, src, FMWidth, FMHeight,
                                                                      FMChannel, dstGap, colIndicator, padModeHc,
                                                                      padModeV, stepK, posK, stepM, posM);
}

// #1178
SANITIZER_REPORT(load_cbuf_to_ca_winograd, __ca__ uint8_t *dst, __cbuf__ uint8_t *src, uint64_t config0,
                 uint64_t config1)
{
    RecordLoadAWinograd<MemType::L1, MemType::L0A, DataType::DATA_B8>(EXTRA_PARAMS, dst, src, config0, config1);
}

// #1179
SANITIZER_REPORT(load_cbuf_to_ca_winograd, __ca__ uint8_t *dst, __cbuf__ uint8_t *src, uint16_t FMWidth,
                 uint16_t FMHeight, uint16_t FMChannel, uint8_t dstGap, uint8_t colIndicator, uint8_t padModeHc,
                 uint8_t padModeV, uint16_t stepK, uint16_t posK, uint16_t stepM, uint16_t posM)
{
    RecordLoadAWinograd<MemType::L1, MemType::L0A, DataType::DATA_B8>(EXTRA_PARAMS, dst, src, FMWidth, FMHeight,
                                                                      FMChannel, dstGap, colIndicator, padModeHc,
                                                                      padModeV, stepK, posK, stepM, posM);
}

// #1290
SANITIZER_REPORT(load_cbuf_to_cb_winograd, __cb__ half *dst, __cbuf__ half *src, uint64_t config)
{
    RecordLoadBWinograd<MemType::L1, MemType::L0B>(EXTRA_PARAMS, dst, src, config);
}

// #1291
SANITIZER_REPORT(load_cbuf_to_cb_winograd, __cb__ half *dst, __cbuf__ half *src, uint8_t innerDstStride,
                 uint16_t srcRepeatStride, uint8_t dstRepeatStride, uint8_t addr_SMASK, uint8_t weightIndicator,
                 bool repeatIndicator, bool weightMatrixOffset, uint8_t repeatStride)
{
    RecordLoadBWinograd<MemType::L1, MemType::L0B>(EXTRA_PARAMS, dst, src, innerDstStride, srcRepeatStride,
                                                   dstRepeatStride, addr_SMASK, weightIndicator, repeatIndicator,
                                                   weightMatrixOffset, repeatStride);
}

// #1292
SANITIZER_REPORT(load_cbuf_to_cb_winograd, __cb__ int8_t *dst, __cbuf__ int8_t *src, uint64_t config)
{
    RecordLoadBWinograd<MemType::L1, MemType::L0B>(EXTRA_PARAMS, dst, src, config);
}

// 1293
SANITIZER_REPORT(load_cbuf_to_cb_winograd, __cb__ int8_t *dst, __cbuf__ int8_t *src, uint8_t innerDstStride,
                 uint16_t srcRepeatStride, uint8_t dstRepeatStride, uint8_t addr_SMASK, uint8_t weightIndicator,
                 bool repeatIndicator, bool weightMatrixOffset, uint8_t repeatStride)
{
    RecordLoadBWinograd<MemType::L1, MemType::L0B>(EXTRA_PARAMS, dst, src, innerDstStride, srcRepeatStride,
                                                   dstRepeatStride, addr_SMASK, weightIndicator, repeatIndicator,
                                                   weightMatrixOffset, repeatStride);
}