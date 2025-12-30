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


#include "event_container.h"
#include "core/framework/utility/log.h"

namespace Sanitizer {

void EventContainer::Init(uint32_t blockNum)
{
    maxBlockNum_ = blockNum;
    ques_.resize(blockNum * (static_cast<uint32_t>(PipeType::SIZE)));
}
// 将事件保存到对应block的PIPE队列中
void EventContainer::Push(const SanEvent &e, PipeType pipe, uint32_t blockIdx)
{
    if (blockIdx >= maxBlockNum_) {
        SAN_WARN_LOG("Invalid blockIdx %u , serialNo %lu", blockIdx, e.serialNo);
        return;
    }
    uint32_t pipeIndex = static_cast<uint32_t>(pipe) + blockIdx * static_cast<uint32_t>(PipeType::SIZE);
    if (e.type == EventType::MEM_EVENT && e.eventInfo.memInfo.opType == AccessType::MEMCPY_BLOCKS) {
        SanEvent event = e;
        event.eventInfo.memInfo.opType = AccessType::READ;
        ques_[pipeIndex].push(event);
        event.eventInfo.memInfo.opType = AccessType::WRITE;
        ques_[pipeIndex].push(event);
        return;
    }
    ques_[pipeIndex].push(e);
}

// 获取当前PIPE的头部事件
SanEvent EventContainer::Front()
{
    // Front前先判空, 判空由调用方做
    return ques_[(blockIndex_ * (static_cast<uint32_t>(PipeType::SIZE))) + pipeIndex_].front();
}

void EventContainer::Pop()
{
    isEventCntsChanged_ = true;
    popCount_ += 1;
    ques_[(blockIndex_ * (static_cast<uint32_t>(PipeType::SIZE))) + pipeIndex_].pop();
}

bool EventContainer::IsCurQueEmpty()
{
    return ques_[(blockIndex_ * (static_cast<uint32_t>(PipeType::SIZE))) + pipeIndex_].empty();
}

bool EventContainer::IsCurBlockEmpty()
{
    for (auto i = blockIndex_ * static_cast<uint32_t>(PipeType::SIZE);
        i < ((blockIndex_ + 1) * static_cast<uint32_t>(PipeType::SIZE)); ++i) {
        if (!ques_[i].empty()) {
            return false;
        }
    }

    return true;
}

bool EventContainer::IsNeedSwitchNextBlock()
{
    if (popCount_ == 0) {
        return true;
    }
    return IsCurBlockEmpty();
}


bool EventContainer::IsEmpty()
{
    for (const auto &it : ques_) {
        if (!it.empty()) {
            return false;
        }
    }

    return true;
}

bool EventContainer::IsTraveAllAndEventsNoChanged()
{
    return cntNoChangedBlocks_ == maxBlockNum_;
}

void EventContainer::CheckEventCntsChangedTag()
{
    if (isEventCntsChanged_) {
        cntNoChangedBlocks_ = 0;
        isEventCntsChanged_ = false;
    } else {
        cntNoChangedBlocks_++;
    }
}

void EventContainer::PrintStuckSerialNo()
{
    for (size_t i = 0; i < ques_.size(); i++) {
        if (ques_[i].empty()) {
            continue;
        }
        SAN_INFO_LOG("Stuck pipe index %lu , serialNo %lu, remaining event count %lu",
            i, ques_[i].front().serialNo, ques_[i].size());
    }
}

void EventContainer::SwitchToNextBlock()
{
    blockIndex_ = (blockIndex_ + 1U) % maxBlockNum_;
    popCount_ = 1U;
    pipeIndex_ = 0U;
}

void EventContainer::SwitchToNextPipe()
{
    pipeIndex_ = (pipeIndex_ + 1U) % static_cast<uint32_t>(PipeType::SIZE);
    // 每次遍历一轮pipe，刷新本轮此popCount
    if (pipeIndex_ == static_cast<uint32_t>(PipeType::PIPE_S)) {
        popCount_ = popCount_ > 1U ? 1U : 0U;
    }
}

PipeType EventContainer::GetPipeIndex() const
{
    return static_cast<PipeType>(pipeIndex_);
}

uint32_t EventContainer::GetQueIndex() const
{
    return pipeIndex_ + blockIndex_ * static_cast<uint32_t>(PipeType::SIZE);
}

uint32_t EventContainer::GetBlockIndex() const
{
    return blockIndex_;
}

uint32_t EventContainer::GetCurQueSize()
{
    return ques_[(blockIndex_ * (static_cast<uint32_t>(PipeType::SIZE))) + pipeIndex_].size();
}

uint32_t EventContainer::GetAllQueSize()
{
    uint32_t sum = 0U;
    for (const auto &it: ques_) {
        sum += it.size();
    }

    return sum;
}

void EventContainer::SetQueIndex(PipeType pipe)
{
    pipeIndex_ = static_cast<uint32_t>(pipe);
}

}