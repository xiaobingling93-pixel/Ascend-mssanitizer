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


#include "plugin/utils.h"
#include "plugin/record_sync_instructions.h"

using namespace Sanitizer;

SANITIZER_REPORT(__ib_set_stub, int32_t blockIdx, int32_t eventID, bool isAIVOnly)
{
    RecordSoftSyncEvent<RecordType::IB_SET_STUB>(EXTRA_PARAMS, blockIdx, eventID, 0, isAIVOnly);
}

SANITIZER_REPORT(__ib_wait_stub, int32_t blockIdx, int32_t eventID, bool isAIVOnly)
{
    RecordSoftSyncEvent<RecordType::IB_WAIT_STUB>(EXTRA_PARAMS, blockIdx, eventID, 0, isAIVOnly);
}

SANITIZER_REPORT(__sync_all_stub, int32_t usedCores, bool isAIVOnly)
{
    RecordSoftSyncEvent<RecordType::SYNC_ALL_STUB>(EXTRA_PARAMS, 0, 0, usedCores, isAIVOnly);
}

// #855
SANITIZER_REPORT(hset_flag, pipe_t pipe, pipe_t tpipe, event_t eventID, mem_t memory, bool v)
{
    RecordHardSyncEvent<RecordType::HSET_FLAG>(EXTRA_PARAMS, pipe, tpipe, static_cast<uint64_t>(eventID), memory, v);
}

// #856
SANITIZER_REPORT(hset_flag, pipe_t pipe, pipe_t tpipe, uint64_t eventID, mem_t memory, bool v)
{
    RecordHardSyncEvent<RecordType::HSET_FLAG>(EXTRA_PARAMS, pipe, tpipe, eventID, memory, v);
}

// #857
SANITIZER_REPORT(hwait_flag, pipe_t pipe, pipe_t tpipe, event_t eventID, mem_t memory, bool v)
{
    RecordHardSyncEvent<RecordType::HWAIT_FLAG>(EXTRA_PARAMS, pipe, tpipe, static_cast<uint64_t>(eventID), memory, v);
}

// #858
SANITIZER_REPORT(hwait_flag, pipe_t pipe, pipe_t tpipe, uint64_t eventID, mem_t memory, bool v)
{
    RecordHardSyncEvent<RecordType::HWAIT_FLAG>(EXTRA_PARAMS, pipe, tpipe, eventID, memory, v);
}

// #2120
SANITIZER_REPORT(set_flag, pipe_t pipe, pipe_t tpipe, event_t eventID)
{
    RecordSyncEvent<RecordType::SET_FLAG>(EXTRA_PARAMS, pipe, tpipe, static_cast<uint64_t>(eventID));
}

// #2122
SANITIZER_REPORT(set_flag, pipe_t tpipe, event_t eventID)
{
    pipe_t pipe = static_cast<pipe_t>(PipeType::PIPE_V);
    RecordSyncEvent<RecordType::SET_FLAG>(EXTRA_PARAMS, pipe, tpipe, static_cast<uint64_t>(eventID));
}

// #2123
SANITIZER_REPORT(set_flag, pipe_t pipe, pipe_t tpipe, uint64_t eventID)
{
    RecordSyncEvent<RecordType::SET_FLAG>(EXTRA_PARAMS, pipe, tpipe, eventID);
}

// #2125
SANITIZER_REPORT(set_flag, pipe_t tpipe, uint64_t eventID)
{
    pipe_t pipe = static_cast<pipe_t>(PipeType::PIPE_V);
    RecordSyncEvent<RecordType::SET_FLAG>(EXTRA_PARAMS, pipe, tpipe, eventID);
}

// #3291
SANITIZER_REPORT(wait_flag, pipe_t pipe, pipe_t tpipe, event_t eventID)
{
    RecordSyncEvent<RecordType::WAIT_FLAG>(EXTRA_PARAMS, pipe, tpipe, static_cast<uint64_t>(eventID));
}

// #3293
SANITIZER_REPORT(wait_flag, pipe_t tpipe, event_t eventID)
{
    pipe_t pipe = static_cast<pipe_t>(PipeType::PIPE_V);
    RecordSyncEvent<RecordType::WAIT_FLAG>(EXTRA_PARAMS, pipe, tpipe, static_cast<uint64_t>(eventID));
}

// #3294
SANITIZER_REPORT(wait_flag, pipe_t pipe, pipe_t tpipe, uint64_t eventID)
{
    RecordSyncEvent<RecordType::WAIT_FLAG>(EXTRA_PARAMS, pipe, tpipe, eventID);
}

// #3296
SANITIZER_REPORT(wait_flag, pipe_t tpipe, uint64_t eventID)
{
    pipe_t pipe = static_cast<pipe_t>(PipeType::PIPE_V);
    RecordSyncEvent<RecordType::WAIT_FLAG>(EXTRA_PARAMS, pipe, tpipe, eventID);
}

// #1654
SANITIZER_REPORT(pipe_barrier, pipe_t pipe)
{
    RecordPipeBarrierEvent(EXTRA_PARAMS, pipe);
}

SANITIZER_REPORT(ffts_cross_core_sync, pipe_t pipe, uint64_t config)
{
    RecordFftsSyncEvent<RecordType::FFTS_SYNC>(EXTRA_PARAMS, pipe, config);
}

SANITIZER_REPORT(wait_flag_dev, int64_t flagID)
{
    RecordWaitFlagDevEvent(EXTRA_PARAMS, flagID);
}

SANITIZER_REPORT(set_atomic_none)
{
    RecordSetAtomicEvent(EXTRA_PARAMS, AtomicMode::NONE);
}

SANITIZER_REPORT(set_atomic_f32)
{
    RecordSetAtomicEvent(EXTRA_PARAMS, AtomicMode::F32);
}

SANITIZER_REPORT(set_atomic_f16)
{
    RecordSetAtomicEvent(EXTRA_PARAMS, AtomicMode::F16);
}

SANITIZER_REPORT(set_atomic_s16)
{
    RecordSetAtomicEvent(EXTRA_PARAMS, AtomicMode::S16);
}

SANITIZER_REPORT(set_atomic_s32)
{
    RecordSetAtomicEvent(EXTRA_PARAMS, AtomicMode::S32);
}

SANITIZER_REPORT(set_atomic_s8)
{
    RecordSetAtomicEvent(EXTRA_PARAMS, AtomicMode::S8);
}

SANITIZER_REPORT(set_atomic_bf16)
{
    RecordSetAtomicEvent(EXTRA_PARAMS, AtomicMode::BF16);
}

SANITIZER_REPORT(set_atomic_add)
{
    RecordSetAtomicEvent(EXTRA_PARAMS, AtomicMode::SUM);
}

SANITIZER_REPORT(set_atomic_max)
{
    RecordSetAtomicEvent(EXTRA_PARAMS, AtomicMode::MAX);
}

SANITIZER_REPORT(set_atomic_min)
{
    RecordSetAtomicEvent(EXTRA_PARAMS, AtomicMode::MIN);
}
