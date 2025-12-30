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


// #4
SANITIZER_REPORT(broadcast_ub_to_cc, __cc__ half *dst, __ubuf__ half *src, uint64_t config)
{
    RecordBroadcastEvent<MemType::UB, MemType::L0C, DataType::DATA_B16, DataType::DATA_B16>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config);
}

// #5
SANITIZER_REPORT(broadcast_ub_to_cc, __cc__ half *dst, __ubuf__ half *src, uint8_t nBurst, uint8_t lenBurst,
                uint8_t srcGap, uint8_t dstGap)
{
    RecordBroadcastEvent<MemType::UB, MemType::L0C, DataType::DATA_B16, DataType::DATA_B16>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), nBurst, lenBurst, srcGap, dstGap);
}

// #6
SANITIZER_REPORT(broadcast_ub_to_cc, __cc__ half *dst, __ubuf__ half *src, uint8_t nBurst, uint8_t lenBurst,
                uint8_t srcGap, uint8_t dstGap, bool repeat)
{
    RecordBroadcastEvent<MemType::UB, MemType::L0C, DataType::DATA_B16, DataType::DATA_B16>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), nBurst, lenBurst, srcGap, dstGap, repeat);
}

// #7
SANITIZER_REPORT(broadcast_ub_to_cc, __cc__ float *dst, __ubuf__ float *src, uint64_t config)
{
    RecordBroadcastEvent<MemType::UB, MemType::L0C, DataType::DATA_B32, DataType::DATA_B32>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config);
}

// #8
SANITIZER_REPORT(broadcast_ub_to_cc, __cc__ float *dst, __ubuf__ float *src, uint8_t nBurst, uint8_t lenBurst,
                uint8_t srcGap, uint8_t dstGap)
{
    RecordBroadcastEvent<MemType::UB, MemType::L0C, DataType::DATA_B32, DataType::DATA_B32>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), nBurst, lenBurst, srcGap, dstGap);
}

// #9
SANITIZER_REPORT(broadcast_ub_to_cc, __cc__ float *dst, __ubuf__ float *src, uint8_t nBurst, uint8_t lenBurst,
                uint8_t srcGap, uint8_t dstGap, bool repeat)
{
    RecordBroadcastEvent<MemType::UB, MemType::L0C, DataType::DATA_B32, DataType::DATA_B32>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), nBurst, lenBurst, srcGap, dstGap, repeat);
}

// #10
SANITIZER_REPORT(broadcast_ub_to_cc, __cc__ float *dst, __ubuf__ half *src, uint64_t config)
{
    RecordBroadcastEvent<MemType::UB, MemType::L0C, DataType::DATA_B16, DataType::DATA_B32>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config);
}

// #11
SANITIZER_REPORT(broadcast_ub_to_cc, __cc__ float *dst, __ubuf__ half *src, uint8_t nBurst, uint8_t lenBurst,
                uint8_t srcGap, uint8_t dstGap)
{
    RecordBroadcastEvent<MemType::UB, MemType::L0C, DataType::DATA_B16, DataType::DATA_B32>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), nBurst, lenBurst, srcGap, dstGap);
}

// #12
SANITIZER_REPORT(broadcast_ub_to_cc, __cc__ float *dst, __ubuf__ half *src, uint8_t nBurst, uint8_t lenBurst,
                uint8_t srcGap, uint8_t dstGap, bool repeat)
{
    RecordBroadcastEvent<MemType::UB, MemType::L0C, DataType::DATA_B16, DataType::DATA_B32>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), nBurst, lenBurst, srcGap, dstGap, repeat);
}

// #13
SANITIZER_REPORT(broadcast_ub_to_cc, __cc__ int32_t *dst, __ubuf__ int32_t *src, uint64_t config)
{
    RecordBroadcastEvent<MemType::UB, MemType::L0C, DataType::DATA_B32, DataType::DATA_B32>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), config);
}

// #14
SANITIZER_REPORT(broadcast_ub_to_cc, __cc__ int32_t *dst, __ubuf__ int32_t *src, uint8_t nBurst, uint8_t lenBurst,
                uint8_t srcGap, uint8_t dstGap)
{
    RecordBroadcastEvent<MemType::UB, MemType::L0C, DataType::DATA_B32, DataType::DATA_B32>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), nBurst, lenBurst, srcGap, dstGap);
}

// #15
SANITIZER_REPORT(broadcast_ub_to_cc, __cc__ int32_t *dst, __ubuf__ int32_t *src, uint8_t nBurst, uint8_t lenBurst,
                uint8_t srcGap, uint8_t dstGap, bool repeat)
{
    RecordBroadcastEvent<MemType::UB, MemType::L0C, DataType::DATA_B32, DataType::DATA_B32>(EXTRA_PARAMS,
        reinterpret_cast<uint64_t>(dst), reinterpret_cast<uint64_t>(src), nBurst, lenBurst, srcGap, dstGap, repeat);
}