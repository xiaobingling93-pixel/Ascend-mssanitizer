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


#ifndef RACE_SANITIZER_ALG_FRAMEWORK_RACE_ALG_BASE_H
#define RACE_SANITIZER_ALG_FRAMEWORK_RACE_ALG_BASE_H

#include <vector>
#include <memory>
#include "core/framework/event_def.h"
#include "event_container.h"
#include "cross_core_sync_info_container.h"
#include "pipe_line.h"
#include "vector_clock.h"
#include "mem_event_checker.h"

namespace Sanitizer {

// 该类是竞争算法的接口类，向上对接RaceSanitizer类，向下对接具体的检测算法
class RaceAlgBase {
public:
    RaceAlgBase(KernelType kernelType, DeviceType deviceType, uint32_t blockDim);
     // 处理算法预处理后的"事件"
    virtual void Do(const SanEvent& event) = 0;
    virtual bool IsFinished() const = 0;
    virtual std::shared_ptr<std::vector<RaceDispInfo>> GetResult() const = 0;
    virtual ~RaceAlgBase() = default;

    virtual ReturnType ProcessBlockSyncEvent(const SanEvent& event) final;
    virtual void CacheMstxCrossSet(const SanEvent& event) final;
    virtual ReturnType ProcessMstxCrossSyncEvent(const SanEvent& event) final;

protected:
    EventContainer eventContainer_;
    CrossCoreSyncInfoContainer crossCoreSyncInfoContainer_;
    /// 缓存需要处理的MSTX_WAIT_CROSS_SYNC对应的set数量，key:pair<addr, flagId>，value:被调用的次数；
    std::map<std::pair<uint64_t, uint64_t>, uint64_t> mstxSetCrossMap_;
    std::vector<VectorTime> vc_{};
    MemEventChecker memChecker_;
    KernelType kernelType_ = KernelType::AICPU;
    DeviceType deviceType_ = DeviceType::INVALID;
    uint32_t blockDim_{};
    bool isFinished_ = false;
};
}

#endif
