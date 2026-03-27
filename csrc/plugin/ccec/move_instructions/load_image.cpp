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

// #1516
SANITIZER_REPORT(load_image_to_cbuf, __cbuf__ half *dst, uint64_t xs, uint64_t xt)
{
    RecordLoadImageEvent<MemType::L1, DataType::DATA_B16>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst), xs, xt);
}

// #1517
SANITIZER_REPORT(load_image_to_cbuf, __cbuf__ half *dst, uint16_t horSize, uint16_t verSize, uint16_t horStartP,
                 uint16_t verStartP, uint16_t sHorRes, uint8_t topPadSize, uint8_t botPadSize, uint16_t lPadSize,
                 uint16_t rPadSize, uint8_t sid)
{
    RecordLoadImageEvent<MemType::L1, DataType::DATA_B16>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                         horSize, verSize, horStartP, verStartP, sHorRes, topPadSize,
                                                         botPadSize, lPadSize, rPadSize);
}

SANITIZER_REPORT(load_image_to_cbuf, __cbuf__ int8_t *dst, uint64_t xs, uint64_t xt)
{
    RecordLoadImageEvent<MemType::L1, DataType::DATA_B8>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                          xs, xt);
}

// #1521
SANITIZER_REPORT(load_image_to_cbuf, __cbuf__ int8_t *dst, uint16_t horSize, uint16_t verSize, uint16_t horStartP,
                 uint16_t verStartP, uint16_t sHorRes, uint8_t topPadSize, uint8_t botPadSize, uint16_t lPadSize,
                 uint16_t rPadSize, uint8_t sid)
{
    RecordLoadImageEvent<MemType::L1, DataType::DATA_B8>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
                                                          horSize, verSize, horStartP, verStartP, sHorRes, topPadSize,
                                                          botPadSize, lPadSize, rPadSize);
}
