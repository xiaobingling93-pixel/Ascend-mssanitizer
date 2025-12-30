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


// 本文件声明了竞争检测内存重叠算法使用的类
// 一次内存操作（读/写）由若干repeat组成，每个repeat内又包含若干block。本文件中，BlkSeries定义为一个repeat内block的迭代器。
// MemSeries类同样是一个迭代器类。其对象对内存操作中所有block在完成合并后，所有连续的区间进行遍历。
#ifndef MEM_SERIES_H
#define MEM_SERIES_H
#include <vector>
#include <queue>

#include "core/framework/event_def.h"

namespace Sanitizer {
// 区间左闭右开，如 [0, 1)表示仅占地址为0的一个字节；[0, 0)表示区间为空
class Interval {
public:
    uint64_t start;
    uint64_t end;
};

// 索引区间，其中索引表示该区间所来自的BlkSeries在MemSeries中的索引（即repeatIdx）。
// 定义该结构体用于支持MemSeries利用优先级队列取出队顶元素后，更新该元素来自的BlkSeries
struct IndexedInterval {
    IndexedInterval(uint64_t idx, Interval interval):idx(idx), interval(interval) {}
    uint64_t idx;
    Interval interval;
};

// 索引区间的比较方法，用于优先级队列将最靠前的区间排在队首
struct Greater {
    bool operator()(const IndexedInterval &lhs, const IndexedInterval &rhs)
    {
        return lhs.interval.start > rhs.interval.start ||
               (rhs.interval.start == lhs.interval.start && lhs.interval.end > rhs.interval.end);
    }
};

// BlockSeries为一次repeat中block的迭代器类
class BlkSeries {
public:
    BlkSeries(uint64_t addr, uint64_t blockSize, uint64_t lastBlockSize, uint64_t blockStride, uint64_t blockNum)
        : addr_(addr), blockSize_(blockSize), lastBlockSize_(lastBlockSize), blockStride_(blockStride),
          blockNum_(blockNum), curBlockIdx_(0)
    {
        UpdateCurItv();
    }
    Interval GetCurItv();
    bool Move2NextItv();
    void UpdateCurItv();
private:
    uint64_t addr_;
    uint64_t blockSize_;
    uint64_t lastBlockSize_;
    uint64_t blockStride_;
    uint64_t blockNum_;
    uint64_t curBlockIdx_;
    Interval curInterval_;
};


// 一次内存操作中，所有block合并后，连续区间的迭代器。
// 从小到大地依次获取连续的内存区间。使用优先级队列避免非必要地对全部区间进行排序
class MemSeries {
public:
    explicit MemSeries(const MemOpInfo memOpInfo);

    // 获取下一个合并后的区间
    // @return: true 获取成功， false 获取失败（迭代结束）
    bool GetNextItv(Interval &outInterval);

private:
    uint64_t nextLeftMostIdx_;
    std::vector<BlkSeries> repeatSeries_;
    // 优先级队列，用于缓存每个BlkSeries最靠右的区间
    std::priority_queue<IndexedInterval, std::vector<IndexedInterval>, Greater > leftMostIntervals_;
};
}

#endif // MEM_SERIES_H