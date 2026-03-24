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


#ifndef RACE_SANITIZER_ALG_FRAMEWORK_EVENT_CONTAINER_H
#define RACE_SANITIZER_ALG_FRAMEWORK_EVENT_CONTAINER_H

#include <queue>
#include <memory>
#include "core/framework/event_def.h"

namespace Sanitizer {

// 该类使用若干个队列存储事件，记录各PIPE里待执行的事件。一个类对象记录了一个AICore上的所有事件。
class EventContainer {
public:
    // 初始化存储队列
    void Init(uint32_t blockNum);
    // 将事件保存到对应的队列中
    void Push(const SanEvent &e, PipeType pipe, uint32_t blockIdx);
    // 获取当前队列头部事件
    SanEvent Front();
    // 将当前队列的头部事件出队，表示事件处理完成
    void Pop();
    // 判断当前队列为空
    bool IsCurQueEmpty() const;
    // 判断是否满足条件切换到下一个block
    bool IsNeedSwitchNextBlock();
    // 所有队列为空
    bool IsEmpty() const;
    // 是否遍历完所有核的管道（核内仅遍历当前核），且事件没有变化
    bool IsTraveAllAndEventsNoChanged() const;
    // 检查事件是否变更的标志位
    void CheckEventCntsChangedTag();
    // 打印卡住的事件serialNo
    void PrintStuckSerialNo() const;
    // 切换到下一个block的PIPES队列
    void SwitchToNextBlock();
    // 切换到block内下一个PIPE进行事件出队
    void SwitchToNextPipe();
    // 获取当前block内pipe编号
    PipeType GetPipeIndex() const;
    // 获取当前队列编号
    uint32_t GetQueIndex() const;
    // 获取当前block编号
    uint32_t GetBlockIndex() const;

    // 测试接口
    uint32_t GetCurQueSize() const;
    uint32_t GetAllQueSize() const;
    void SetQueIndex(PipeType pipe);
private:
    // 当前block内事件队列是否为空
    bool IsCurBlockEmpty() const;
private:
    std::vector<std::queue<SanEvent>> ques_;
    bool isEventCntsChanged_ = false;
    // 事件未发生变化的block连续次数
    uint32_t cntNoChangedBlocks_ = 0;
    uint32_t maxBlockNum_ = 0;

    // 用于记录本轮block出队事件数，用于控制block切换
    uint32_t popCount_ = 1U;
    uint32_t pipeIndex_ = 0U;
    uint32_t blockIndex_ = 0U;
};
}

#endif
