#include "core/framework/record_defs.h"
#include "core/framework/arch_def.h"
#include "plugin/utils.h"
#include "plugin/record_sync_instructions.h"
using namespace Sanitizer;

SANITIZER_REPORT(wait_flag_dev_pipe, pipe_t pipe, int64_t flagID)
{
    RecordWaitFlagDevEventWithPipe<RecordType::WAIT_FLAG_DEV_PIPE>(EXTRA_PARAMS, pipe, flagID);
}

SANITIZER_REPORT(wait_flag_devi_pipe, pipe_t pipe, uint8_t flagID)
{
    RecordWaitFlagDevEventWithPipe<RecordType::WAIT_FLAG_DEVI_PIPE>(EXTRA_PARAMS, pipe, flagID);
}

SANITIZER_REPORT(set_cross_core, pipe_t pipe, uint64_t config)
{
    RecordFftsSyncEvent(EXTRA_PARAMS, pipe, config);
}

SANITIZER_REPORT(wait_intra_block, pipe_t pipe, uint64_t syncID)
{
    RecordIntraBlockSyncEvent<RecordType::WAIT_INTRA_BLOCK>(EXTRA_PARAMS, pipe, syncID);
}

SANITIZER_REPORT(set_intra_block, pipe_t pipe, uint64_t syncID)
{
    RecordIntraBlockSyncEvent<RecordType::SET_INTRA_BLOCK>(EXTRA_PARAMS, pipe, syncID);
}

SANITIZER_REPORT(wait_intra_blocki, pipe_t pipe, uint8_t syncID)
{
    RecordIntraBlockSyncEvent<RecordType::WAIT_INTRA_BLOCKI>(EXTRA_PARAMS, pipe, syncID);
}

SANITIZER_REPORT(set_intra_blocki, pipe_t pipe, uint8_t syncID)
{
    RecordIntraBlockSyncEvent<RecordType::SET_INTRA_BLOCKI>(EXTRA_PARAMS, pipe, syncID);
}