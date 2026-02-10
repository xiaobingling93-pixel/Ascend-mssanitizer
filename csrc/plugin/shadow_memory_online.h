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

#ifndef SHADOW_MEMORY_ONLINE_H
#define SHADOW_MEMORY_ONLINE_H

#include "kernel_pub_func.h"
#include "parse_record.h"

namespace Sanitizer {

#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3101 && defined(SIMT_MODE)

enum class ErrorCode : uint64_t {
    // for MultiLayerTable
    UNALLOCATABLE_FOR_L0 = 0x100U,
    INVALID_L0_IDX_ADDR,
    INVALID_L1_START_ADDR,
    UNALLOCATABLE_FOR_L1,
    INVALID_L1_IDX_ADDR,
    WAIT_FOR_L0_UNLOCK_TIMEOUT,
    WAIT_FOR_L1_UNLOCK_TIMEOUT,

    // for ShadowMemoryHeapAllocator
    ALLOCATOR_NOT_READY = 0x200,
    ALLOCATOR_REMAINING_MEM_NOT_ENOUGH,
    ALLOCATED_ADDR_ILLEGAL,

};

class ShadowMemoryHeapAllocator {
public:
    __aicore__ __attribute__((always_inline)) ShadowMemoryHeapAllocator(): heapHeadPtr_(nullptr), isReady_(false) {}

    __aicore__ inline bool Init(uint64_t heapAddr, uint64_t size)
    {
        if (heapAddr == 0U || (size == 0U)) {
            return false;
        }
        // ShadowMemoryHeapHead 已在host完成赋值，此处只需创建head指针
        heapHeadPtr_ = reinterpret_cast<__gm__ ShadowMemoryHeapHead *>(heapAddr);

        if (heapHeadPtr_->size == 0U) {
            return false;
        }

        maxAddr_ = heapHeadPtr_->startAddr + heapHeadPtr_->size;
        isReady_ = true;
        return true;
    }

    __aicore__ inline uint64_t AllocHeap(uint64_t allocSize, uint64_t &addr)
    {
        if (!isReady_) {
            return static_cast<uint64_t>(ErrorCode::ALLOCATOR_NOT_READY);
        }

        if (AtomicAdd(&(heapHeadPtr_->current), 0U) > maxAddr_) {
            // 已无内存可分配
            return static_cast<uint64_t>(ErrorCode::ALLOCATOR_REMAINING_MEM_NOT_ENOUGH);
        }

        addr = AtomicAdd(&(heapHeadPtr_->current), allocSize);

        // 检查是否超过范围
        if (AtomicAdd(&(heapHeadPtr_->current), 0U) > maxAddr_) {
            // 本次不够分配
            addr = static_cast<uint64_t>(OnlineSmAddrStatus::UNALLOCATABLE);
            return static_cast<uint64_t>(ErrorCode::ALLOCATED_ADDR_ILLEGAL);
        }

        return 0U;
    }

    __aicore__ inline uint64_t FreeHeap(uint64_t size)
    {
        // 暂不支持释放内存
        return 0U;
    }

    __aicore__ inline uint64_t GetHeapStartAddr()
    {
        if (!isReady_) {
            return 0U;
        }
        return heapHeadPtr_->startAddr;
    }

    __aicore__ inline uint64_t GetHeapEndAddr()
    {
        if (!isReady_) {
            return 0U;
        }
        return heapHeadPtr_->startAddr + heapHeadPtr_->size;
    }

    __aicore__ inline bool IsHeapFreshNew()
    {
        return (AtomicAdd(&(heapHeadPtr_->startAddr), 0U)) == (AtomicAdd(&(heapHeadPtr_->current), 0U));
    }

private:
    bool isReady_;
    uint64_t maxAddr_;
    __gm__ ShadowMemoryHeapHead *heapHeadPtr_;
};

template <typename ByteStatus_t>
class MemoryByteStatusParser {
public:
    __aicore__ static inline ByteStatus_t Construct(uint16_t status, uint16_t threadId, uint64_t pc)
    {
        /*
            [63:16]: pc
            [11]: status
            [10:0]: threadId
        */
        static_assert(sizeof(ByteStatus_t) >= sizeof(uint64_t),
            "memory byte status model requires not less than 8 bytes length for every single byte");
        return static_cast<ByteStatus_t>(pc << 16U) | static_cast<ByteStatus_t>((status & 0x1U) << 11U) |
            static_cast<ByteStatus_t>(threadId & 0x7FFU);
    }

    __aicore__ static inline uint16_t ExtractStatus(ByteStatus_t val)
    {
        return static_cast<uint16_t>((val >> 11U) & 0x1U);
    }

    __aicore__ static inline uint16_t ExtractThreadId(ByteStatus_t val)
    {
        return static_cast<uint16_t>(val & 0x7FFU);
    }

private:
    __aicore__ inline MemoryByteStatusParser() {}
};

struct TableLayout {
    uint64_t listPtr;
    uint64_t mask;
    uint64_t blockNum;
    uint64_t blockSize; // block 表达的内存范围大小

    __aicore__ inline TableLayout(): listPtr(0U), mask(0U), blockNum(0U), blockSize(0U) {}
};

template <typename ByteStatus_t>
class MultiLayerTable {
public:
    __aicore__ inline MultiLayerTable(): heapAllocator_()
    {
        l0Tbl_.listPtr = 0U;
        l0Tbl_.mask = ONLINE_GLOBAL_MEM_MASK;
        l0Tbl_.blockSize = ONLINE_LOCAL_MEM_MASK;
        l0Tbl_.blockNum = (ONLINE_GLOBAL_MEM_MASK + l0Tbl_.blockSize - 1U) / l0Tbl_.blockSize;

        l1Tbl_.listPtr = 0U;
        l1Tbl_.mask = ONLINE_LOCAL_MEM_MASK;
        l1Tbl_.blockSize = ONLINE_ONE_SM_STAND_FOR_BYTE;
        l1Tbl_.blockNum = (ONLINE_LOCAL_MEM_MASK + l1Tbl_.blockSize - 1U) / l1Tbl_.blockSize;

        l2Tbl_.listPtr = 0U;
        l2Tbl_.mask = ONLINE_ONE_SM_STAND_FOR_BYTE - 1U;
        l2Tbl_.blockSize = 1;
        l2Tbl_.blockNum = ONLINE_ONE_SM_STAND_FOR_BYTE;
    }

    __aicore__ inline bool Init(uint64_t heapAddr, uint64_t size)
    {
        if (!heapAllocator_.Init(heapAddr, size)) {
            return false;
        }

        // Host已预分配了L0级表空间，每个节点占8字节，节点数：
        // 节点数 = (ONLINE_GLOBAL_MEM_MASK + ONLINE_LOCAL_MEM_MASK - 1U) / ONLINE_LOCAL_MEM_MASK) * sizeof(uint64_t)
        // L0层起始地址，始终与heap起始地址一致
        l0Tbl_.listPtr = heapAllocator_.GetHeapStartAddr();

        return true;
    }

    __aicore__ inline uint64_t LookUp(const uint64_t addr,
        uint64_t &l1StartAddr, uint64_t &l2StartAddr, uint64_t &l2MemStatusAddr)
    {
        uint64_t ret = 0U;
        ret = LookUpInL0(addr, l1StartAddr);
        if (ret != 0U) {
            l2StartAddr = ret; // 承载错误码
            return ret;
        }

        ret = LookUpInL1(addr, l1StartAddr, l2StartAddr);
        if (ret != 0U) {
            l2StartAddr = ret; // 承载错误码
            return ret;
        }

        l2MemStatusAddr = LookUpInL2(addr, l2StartAddr);
        return ret;
    }

private:
    __aicore__ inline uint64_t LookUpInL0(uint64_t addr, uint64_t &l1StartAddr)
    {
        uint64_t ret = 0U;
        uint64_t l0Idx = (addr & l0Tbl_.mask) / l0Tbl_.blockSize;

        __gm__ uint64_t *l0Ptr = reinterpret_cast<__gm__ uint64_t *>(l0Tbl_.listPtr) + l0Idx;

        if (reinterpret_cast<uint64_t>(l0Ptr) < heapAllocator_.GetHeapStartAddr() ||
            reinterpret_cast<uint64_t>(l0Ptr) > heapAllocator_.GetHeapEndAddr()) {
            return static_cast<uint64_t>(ErrorCode::INVALID_L0_IDX_ADDR);
        }
        if (*l0Ptr == static_cast<uint64_t>(OnlineSmAddrStatus::UNALLOCATABLE)) {
            // 内存耗尽
            return static_cast<uint64_t>(ErrorCode::UNALLOCATABLE_FOR_L0);
        }

        uint64_t newHeap = 0U;
        uint64_t lock = AtomicCAS(l0Ptr, 0U, static_cast<uint64_t>(OnlineSmAddrStatus::LOCKED_BY_OTHER_THREADS));
        if (lock == 0) {
            // 获取锁的线程
            ret = heapAllocator_.AllocHeap(l1Tbl_.blockNum * sizeof(uint64_t), newHeap);
            if (ret == 0U) {
                AtomicExch(l0Ptr, newHeap);
            } else {
                AtomicExch(l0Ptr, static_cast<uint64_t>(OnlineSmAddrStatus::UNALLOCATABLE));
            }
        } else {
            // 未获取锁的线程，等待锁释放
            uint64_t count = 0;
            constexpr uint64_t max_count = 2000;
            while ((lock == static_cast<uint64_t>(OnlineSmAddrStatus::LOCKED_BY_OTHER_THREADS)) &&
                (count < max_count)) {
                lock = AtomicAdd(l0Ptr, 0);
                count++;
            }
        }
        l1StartAddr = AtomicAdd(l0Ptr, 0);
        if (l1StartAddr == static_cast<uint64_t>(OnlineSmAddrStatus::UNALLOCATABLE)) {
            // 无内存可分配
            ret = static_cast<uint64_t>(OnlineSmAddrStatus::UNALLOCATABLE);
        } else if (l1StartAddr == static_cast<uint64_t>(OnlineSmAddrStatus::LOCKED_BY_OTHER_THREADS)) {
            // 状态异常
            ret = static_cast<uint64_t>(OnlineSmAddrStatus::LOCKED_BY_OTHER_THREADS);
        } else if (l1StartAddr == 0) {
            ret = static_cast<uint64_t>(ErrorCode::WAIT_FOR_L0_UNLOCK_TIMEOUT);
        } else {
            // 正常地址
            ret = 0U;
        }

        return ret;
    }

    __aicore__ inline uint64_t LookUpInL1(uint64_t addr, uint64_t l1StartAddr, uint64_t &l2StartAddr)
    {
        uint64_t ret = 0U;
        if (l1StartAddr == 0U) {
            return static_cast<uint64_t>(ErrorCode::INVALID_L1_START_ADDR);
        }
        l1Tbl_.listPtr = l1StartAddr;
        uint64_t l1Idx = (addr & l1Tbl_.mask) / l1Tbl_.blockSize;

        __gm__ uint64_t *l1Ptr = reinterpret_cast<__gm__ uint64_t *>(l1Tbl_.listPtr) + l1Idx;

        if (reinterpret_cast<uint64_t>(l1Ptr) < heapAllocator_.GetHeapStartAddr() ||
            reinterpret_cast<uint64_t>(l1Ptr) > heapAllocator_.GetHeapEndAddr()) {
            return static_cast<uint64_t>(ErrorCode::INVALID_L1_IDX_ADDR);
        }
        if (*l1Ptr == static_cast<uint64_t>(OnlineSmAddrStatus::UNALLOCATABLE)) {
            // 内存耗尽
            return static_cast<uint64_t>(ErrorCode::UNALLOCATABLE_FOR_L1);
        }

        uint64_t newHeap = 0U;
        uint64_t lock = AtomicCAS(l1Ptr, 0U, static_cast<uint64_t>(OnlineSmAddrStatus::LOCKED_BY_OTHER_THREADS));
        if (lock == 0) {
            // 获取锁的线程
            ret = heapAllocator_.AllocHeap(l2Tbl_.blockNum * sizeof(ByteStatus_t), newHeap);
            if (ret == 0U) {
                AtomicExch(l1Ptr, newHeap);
            } else {
                AtomicExch(l1Ptr, static_cast<uint64_t>(OnlineSmAddrStatus::UNALLOCATABLE));
            }
        } else {
            // 未获取锁的线程，等待锁释放
            uint64_t count = 0;
            constexpr uint64_t max_count = 2000;
            while (lock == static_cast<uint64_t>(OnlineSmAddrStatus::LOCKED_BY_OTHER_THREADS) &&
                (count < max_count)) {
                lock = AtomicAdd(l1Ptr, 0);
                count++;
            }
        }

        l2StartAddr = AtomicAdd(l1Ptr, 0);
        if (l2StartAddr == static_cast<uint64_t>(OnlineSmAddrStatus::UNALLOCATABLE)) {
            ret = static_cast<uint64_t>(OnlineSmAddrStatus::UNALLOCATABLE);
        } else if (l2StartAddr == static_cast<uint64_t>(OnlineSmAddrStatus::LOCKED_BY_OTHER_THREADS)) {
            // 异常
            ret = static_cast<uint64_t>(OnlineSmAddrStatus::LOCKED_BY_OTHER_THREADS);
        } else if (l2StartAddr == 0) {
            ret = static_cast<uint64_t>(ErrorCode::WAIT_FOR_L1_UNLOCK_TIMEOUT);
        } else {
            // 正常地址
            ret = 0U;
        }

        return ret;
    }

    __aicore__ inline uint64_t LookUpInL2(uint64_t addr, uint64_t l2StartAddr)
    {
        l2Tbl_.listPtr = l2StartAddr;
        __gm__ ByteStatus_t *l2StartPtr = reinterpret_cast<__gm__ ByteStatus_t *>(l2StartAddr);
        uint64_t l2Idx = (addr & l2Tbl_.mask) / l2Tbl_.blockSize;
        return reinterpret_cast<uint64_t>(l2StartPtr + l2Idx);
    }

    ShadowMemoryHeapAllocator heapAllocator_;

    TableLayout l0Tbl_; // 仅一组，长度固定
    TableLayout l1Tbl_; // 每一个PM对应一组
    TableLayout l2Tbl_; // 单字节模型
};

// simplified shadow memory running in SIMT VF only
class ShadowMemoryOnline {
    enum MemoryByteStatus {
        NOT_WRITTEN = 0U,
        WRITTEN = 1U,
    };
    using ByteStatus_t = uint64_t;
public:
    struct AuxInfo {
        /*** 线程间踩踏使用 ***/
        SimtThreadLocation conflictedThreadLoc; // 记录被当前线程所踩踏的线程坐标
        uint64_t l1StartAddr; // L1StartAddr
        uint64_t l2StartAddr; // L2StartAddr
        uint64_t l2MemStatusAddr; // 字节状态位地址

        __aicore__ inline AuxInfo(): l1StartAddr(0U), l2StartAddr(0U), l2MemStatusAddr(0U)
        {
            conflictedThreadLoc.idX = 0;
            conflictedThreadLoc.idY = 0;
            conflictedThreadLoc.idZ = 0;
        }
    };
    __aicore__ inline ShadowMemoryOnline() : tables_{}, isReady_{false} {}

    __aicore__ inline bool Init(uint64_t heapAddr, uint64_t size)
    {
        if (!tables_.Init(heapAddr, size)) {
            return false;
        }
        isReady_ = true;
        return true;
    }

    __aicore__ inline bool IsReady() const
    {
        return isReady_;
    }

    __aicore__ inline uint64_t LoadNBytes(AddrInfo const &addrInfo, AuxInfo &auxInfo)
    {
        // 踩踏不关心读操作
        (void)addrInfo;
        (void)auxInfo;
        return 0U;
    }

    __aicore__ inline uint64_t StoreNBytes(AddrInfo const &addrInfo, AuxInfo &auxInfo)
    {
        uint64_t nBadBytesForOverlap = 0U;
        uint64_t ret = 0U;

        if (!IsReady()) {
            return 0U;
        }

        for (uint64_t i = 0U; i < addrInfo.size; ++i) {
            uint64_t addr = addrInfo.addr + i;
            ret = tables_.LookUp(addr, auxInfo.l1StartAddr,
                auxInfo.l2StartAddr, auxInfo.l2MemStatusAddr);

            if (ret != 0U) {
                // shadow memory内存不够，地址查询失败，中断后续的地址访问
                return 0U;
            }

            uint16_t threadId = GetThreadId();
            ByteStatus_t oldValue = ExchangeMemStatus(auxInfo.l2MemStatusAddr,
                static_cast<uint16_t>(MemoryByteStatus::WRITTEN), threadId, addrInfo.location.pc);
            if ((MemoryByteStatusParser<ByteStatus_t>::ExtractStatus(oldValue) ==
                static_cast<uint16_t>(MemoryByteStatus::WRITTEN)) &&
                (MemoryByteStatusParser<ByteStatus_t>::ExtractThreadId(oldValue) != threadId)) {
                DecomposeThreadId(threadId, auxInfo.conflictedThreadLoc.idX,
                    auxInfo.conflictedThreadLoc.idY, auxInfo.conflictedThreadLoc.idZ);
                nBadBytesForOverlap++;
            }
        }

        return nBadBytesForOverlap;
    }

    __aicore__ inline ByteStatus_t ExchangeMemStatus(uint64_t addr, uint16_t isWritten,
        uint16_t threadId, uint64_t pc)
    {
        ByteStatus_t newValue = MemoryByteStatusParser<ByteStatus_t>::Construct(isWritten, threadId, pc);
        return AtomicExch(reinterpret_cast<__gm__ uint64_t *>(addr), newValue);
    }

private:
    MultiLayerTable<ByteStatus_t> tables_;
    bool isReady_;
};

#endif

}

#endif