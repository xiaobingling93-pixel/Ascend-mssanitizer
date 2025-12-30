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

#include "mem_series.h"
#include "core/framework/constant.h"

namespace Sanitizer {

Interval BlkSeries::GetCurItv()
{
    return curInterval_;
}

bool BlkSeries::Move2NextItv()
{
    if (curBlockIdx_ + 1 >= blockNum_) {
        return false;
    }
    ++curBlockIdx_;
    UpdateCurItv();
    return true;
}

void BlkSeries::UpdateCurItv()
{
    curInterval_.start = addr_ + curBlockIdx_ * blockSize_ * blockStride_;
    curInterval_.end = curInterval_.start + (curBlockIdx_ == blockNum_ - 1 ? lastBlockSize_ : blockSize_);
}

MemSeries::MemSeries(const MemOpInfo memOpInfo) : nextLeftMostIdx_(0)
{
    uint64_t addr = memOpInfo.addr;
    uint64_t blockNum = memOpInfo.blockNum;
    uint64_t repeatTimes = 0;
    uint64_t lastBlockNum = 0;
    uint64_t lastBlockSize = 0;
    repeatTimes = memOpInfo.repeatTimes;
    lastBlockNum = blockNum;
    lastBlockSize = memOpInfo.blockSize;
    for (uint64_t repeatIdx = 0; repeatIdx + 1 < repeatTimes; ++repeatIdx) {
        repeatSeries_.emplace_back(addr, memOpInfo.blockSize, memOpInfo.blockSize, memOpInfo.blockStride, blockNum);
        addr += memOpInfo.repeatStride * memOpInfo.blockSize;
    }
    repeatSeries_.emplace_back(addr, memOpInfo.blockSize, lastBlockSize, memOpInfo.blockStride, lastBlockNum);
    Interval interval;
    for (uint64_t idx = 0; idx < repeatTimes; ++idx) {
        interval = repeatSeries_[idx].GetCurItv();
        leftMostIntervals_.emplace(idx, interval);
    }
}

// 重复从优先级队列获取队首元素，以延长(合并)当前连续区间，直到无法延长。通过出参返回当前合并后的区间
bool MemSeries::GetNextItv(Interval & outInterval)
{
    if (leftMostIntervals_.empty()) {
        return false;
    }
    // 获取地址最小的区间, 并获取右边界
    auto idxInterval = leftMostIntervals_.top();
    uint64_t leftMostAddr = idxInterval.interval.start;
    uint64_t extEnd = leftMostAddr;

    // 循环遍历repeatSeries以延长区间
    while (!leftMostIntervals_.empty()) {
        auto curIdxedInterval = leftMostIntervals_.top();
        // 判断是否满足延长条件(首次一定满足)
        if (curIdxedInterval.interval.start <= extEnd) {
            extEnd = curIdxedInterval.interval.end;
            leftMostIntervals_.pop();
            // 如果迭代未结束则补充下一个区间
            if (repeatSeries_[curIdxedInterval.idx].Move2NextItv()) {
                auto interval = repeatSeries_[curIdxedInterval.idx].GetCurItv();
                leftMostIntervals_.emplace(curIdxedInterval.idx, interval);
            } else {
                continue;
            }
        } else {
            break;
        }
    };

    outInterval.start = leftMostAddr;
    outInterval.end = extEnd;
    return true;
}

}