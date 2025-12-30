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


#include "pipe_line.h"
#include "core/framework/utility/log.h"

namespace Sanitizer {
void PipeLine::RegisterEventFunc(const EventFunc &f)
{
    func_ = f;
}

void PipeLine::Run()
{
    while ((pipeState_ != PipeState::EXIT) && (pipeState_ != PipeState::EXCEPTION)) {
        switch (pipeState_) {
            case PipeState::CHECK_ALL_BLOCK:
                pipeState_ = CheckAllBlock();
                break;
            case PipeState::CHECK_CUR_BLOCK:
                pipeState_ = CheckCurBlock();
                break;
            case PipeState::CHECK_CUR_PIPE:
                pipeState_ = CheckCurPipe();
                break;
            case PipeState::PROCESS_EVENT:
                pipeState_ = Process();
                break;
            case PipeState::EXIT:
            case PipeState::EXCEPTION:
            default:
                pipeState_ = PipeState::EXCEPTION;
                break;
        }
    }
}

PipeState PipeLine::CheckAllBlock()
{
    return (events_.IsEmpty()) ? PipeState::EXIT : PipeState::CHECK_CUR_BLOCK;
}

PipeState PipeLine::CheckCurBlock()
{
    if (!events_.IsNeedSwitchNextBlock()) {
        return PipeState::CHECK_CUR_PIPE;
    }
    events_.SwitchToNextBlock();
    events_.CheckEventCntsChangedTag();
    if (events_.IsTraveAllAndEventsNoChanged()) {
        SAN_ERROR_LOG("The pipe line is in the infinite loop!");
        events_.PrintStuckSerialNo();
        return PipeState::EXCEPTION;
    }
    return PipeState::CHECK_ALL_BLOCK;
}

PipeState PipeLine::CheckCurPipe()
{
    if (events_.IsCurQueEmpty()) {
        events_.SwitchToNextPipe();
        return PipeState::CHECK_CUR_BLOCK;
    }
    return PipeState::PROCESS_EVENT;
}

PipeState PipeLine::Process()
{
    SanEvent event = events_.Front();
    if (!func_) {
        return PipeState::CHECK_ALL_BLOCK;
    }
    if (func_(event) == ReturnType::PROCESS_OK) {
        events_.Pop();
    } else {
        events_.SwitchToNextPipe();
    }

    return PipeState::CHECK_CUR_BLOCK;
}
}