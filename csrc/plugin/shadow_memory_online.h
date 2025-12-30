// Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
#ifndef SHADOW_MEMORY_ONLINE_H
#define SHADOW_MEMORY_ONLINE_H

#include "kernel_pub_func.h"
#include "parse_record.h"

namespace Sanitizer {

#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3101 && defined(SIMT_MODE)

class ShadowMemoryHeapAllocator {
public:
    __aicore__ inline ShadowMemoryHeapAllocator(): heapHeadPtr_(nullptr), isReady_(false) {}

    __aicore__ inline bool Init(uint64_t heapAddr, uint64_t size)
    {
        (void)size;
        if (heapAddr == 0U) {
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

    __aicore__ inline uint64_t AllocHeapForOneThread(uint64_t allocSize)
    {
        uint64_t newHeap = 0U;
        SyncThreads();
        uint64_t lock = AtomicCAS(&(heapHeadPtr_->lock), 0U, 1U);
        SyncThreads();
        if (lock == 0U) {
            // 获取到锁的线程
            newHeap = AllocHeap(allocSize);
            heapHeadPtr_->lock = 0U;
        }
        SyncThreads();

        return newHeap;
    }

    __aicore__ inline uint64_t GetHeapStartAddr()
    {
        if (!isReady_) {
            return 0U;
        }
        return heapHeadPtr_->startAddr;
    }

private:
    __aicore__ inline uint64_t AllocHeap(uint64_t allocSize)
    {
        // 需在外部处理多线程竞争
        if (!isReady_) {
            return 0U;
        }

        uint64_t rest = heapHeadPtr_->size - (heapHeadPtr_->current - heapHeadPtr_->startAddr);
        if (rest < allocSize) {
            // 不够分配，返回0地址
            return 0U;
        }

        uint64_t addr = heapHeadPtr_->current;
        heapHeadPtr_->current += allocSize;

        return addr;
    }

    __aicore__ inline uint64_t FreeHeap(uint64_t size)
    {
        // 暂不支持释放内存
        return 0U;
    }

    __gm__ ShadowMemoryHeapHead *heapHeadPtr_;
    bool isReady_;
    uint64_t maxAddr_;
};

class MemoryByteStatusParser {
public:
    __aicore__ static inline uint32_t Construct(uint16_t status, uint16_t threadId)
    {
        return static_cast<uint32_t>((status & 0x1U) << 11U) | static_cast<uint32_t>(threadId & 0x7FFU);
    }

    __aicore__ static inline uint16_t ExtractStatus(uint32_t val)
    {
        return static_cast<uint16_t>((val >> 11U) & 0x1U);
    }

    __aicore__ static inline uint16_t ExtractThreadId(uint32_t val)
    {
        return static_cast<uint16_t>(val & 0x7FFU);
    }

private:
    __aicore__ inline MemoryByteStatusParser() {}
};

class MultiLayerTable {
    // gm建模地址范围0 ~ 0xFFFF FFFF FFFF (48 bits)
    static constexpr uint64_t GLOBAL_MEM_MASK = 0xFFFFFFFFFFFFULL;
    // 片上内存建模地址范围0 ~ 0xF FFFF FFFF (36 bits)
    static constexpr uint64_t LOCAL_MEM_MASK = 0xFFFFFFFFFULL;
    // 用于标记GM上定义的数据来源于host
    static constexpr uint64_t ONE_SM_STAND_FOR_BYTE = 65536U; // 64KB

    enum class AddrStatus : uint64_t {
        LOCKED_BY_OTHER_THREADS = 1U,
        UNALLOCATABLE = UINT64_MAX, // 内存异常，无法再分配
    };

    struct TableLayout {
        uint64_t listPtr;
        uint64_t mask;
        uint64_t blockNum;
        uint64_t blockSize; // block 表达的内存范围大小

        __aicore__ inline TableLayout(): listPtr(0U), mask(0U), blockNum(0U), blockSize(0U) {}
    };

public:
    __aicore__ inline MultiLayerTable(): heapAllocator_(nullptr), isReady_(false)
    {
        l0Tbl_.listPtr = 0U;
        l0Tbl_.mask = GLOBAL_MEM_MASK;
        l0Tbl_.blockSize = LOCAL_MEM_MASK;
        l0Tbl_.blockNum = (GLOBAL_MEM_MASK + l0Tbl_.blockSize - 1U) / l0Tbl_.blockSize;

        l1Tbl_.listPtr = 0U;
        l1Tbl_.mask = LOCAL_MEM_MASK;
        l1Tbl_.blockSize = ONE_SM_STAND_FOR_BYTE;
        l1Tbl_.blockNum = (LOCAL_MEM_MASK + l1Tbl_.blockSize - 1U) / l1Tbl_.blockSize;

        l2Tbl_.listPtr = 0U;
        l2Tbl_.mask = ONE_SM_STAND_FOR_BYTE - 1U;
        l2Tbl_.blockSize = 1;
        l2Tbl_.blockNum = ONE_SM_STAND_FOR_BYTE;
    }

    __aicore__ inline void Init(ShadowMemoryHeapAllocator *allocator)
    {
        if (allocator == nullptr) {
            return;
        }
        // L0层数组无需手动初始化，内存已在host侧初始化为0
        heapAllocator_ = allocator;
        l0Tbl_.listPtr = allocator->GetHeapStartAddr();

        // 分配L0层
        heapAllocator_->AllocHeapForOneThread(l0Tbl_.blockNum * sizeof(uint64_t));

        isReady_ = true;
    }

    __aicore__ inline uint64_t LookUp(const uint64_t addr,
        uint64_t &l1StartAddr, uint64_t &l2StartAddr, uint64_t &l2MemStatusAddr)
    {
        if (!isReady_) {
            return 0U;
        }

        l1StartAddr = LookUpInL0(addr);
        if (l1StartAddr == 0U) {
            return 0U;
        }
        l2StartAddr = LookUpInL1(l1StartAddr, addr);
        if (l2StartAddr == 0U) {
            return 0U;
        }
        l2MemStatusAddr = LookUpInL2(l2StartAddr, addr);
        return l2MemStatusAddr;
    }

private:
    __aicore__ inline uint64_t LookUpInL0(uint64_t addr)
    {
        uint64_t l0Idx = (addr & l0Tbl_.mask) / l0Tbl_.blockSize;

        __gm__ uint64_t *l0Ptr = reinterpret_cast<__gm__ uint64_t *>(l0Tbl_.listPtr) + l0Idx;

        if (*l0Ptr == static_cast<uint64_t>(AddrStatus::UNALLOCATABLE)) {
            // 内存耗尽
            return 0U;
        }

        SyncThreads();

        uint64_t l1StartAddr = AtomicCAS(l0Ptr, 0U, static_cast<uint64_t>(AddrStatus::LOCKED_BY_OTHER_THREADS));
        if (l1StartAddr == 0) {
            // 获取到锁的线程
            uint64_t newHeap = heapAllocator_->AllocHeapForOneThread(l1Tbl_.blockNum * sizeof(uint64_t));
            // 赋值并解锁
            if (newHeap == 0U) {
                *l0Ptr = static_cast<uint64_t>(AddrStatus::UNALLOCATABLE);
            } else {
                *l0Ptr = newHeap;
            }
        }

        SyncThreads(); // warp间同步，等待l0Ptr赋值完成

        // 所有线程再读一次l0Ptr
        l1StartAddr = *l0Ptr;
        if (l1StartAddr == static_cast<uint64_t>(AddrStatus::UNALLOCATABLE)) {
            l1StartAddr = 0U;
        } else if (l1StartAddr == static_cast<uint64_t>(AddrStatus::LOCKED_BY_OTHER_THREADS)) {
            // 异常
            l1StartAddr = 0U;
        } else {
            // 正常地址
            l1StartAddr = *l0Ptr;
        }

        return l1StartAddr;
    }

    __aicore__ inline uint64_t LookUpInL1(uint64_t l1StartAddr, uint64_t addr)
    {
        l1Tbl_.listPtr = l1StartAddr;
        uint64_t l1Idx = (addr & l1Tbl_.mask) / l1Tbl_.blockSize;

        __gm__ uint64_t *l1Ptr = reinterpret_cast<__gm__ uint64_t *>(l1Tbl_.listPtr) + l1Idx;

        if (*l1Ptr == static_cast<uint64_t>(AddrStatus::UNALLOCATABLE)) {
            // 内存耗尽
            return 0U;
        }

        SyncThreads();

        uint64_t l2StartAddr = AtomicCAS(l1Ptr, 0U, static_cast<uint64_t>(AddrStatus::LOCKED_BY_OTHER_THREADS));
        if (l2StartAddr == 0) {
            // 获取到锁的线程
            uint64_t newHeap = heapAllocator_->AllocHeapForOneThread(l1Tbl_.blockSize * sizeof(uint32_t));
            // 赋值并解锁
            if (newHeap == 0U) {
                *l1Ptr = static_cast<uint64_t>(AddrStatus::UNALLOCATABLE);
            } else {
                *l1Ptr = newHeap;
            }
        }

        SyncThreads(); // warp间同步，等待l1Ptr赋值完成

        // 所有线程再读一次l0Ptr
        l2StartAddr = *l1Ptr;
        if (l2StartAddr == static_cast<uint64_t>(AddrStatus::UNALLOCATABLE)) {
            l2StartAddr = 0U;
        } else if (l2StartAddr == static_cast<uint64_t>(AddrStatus::LOCKED_BY_OTHER_THREADS)) {
            // 异常
            l2StartAddr = 0U;
        } else {
            // 正常地址
            l2StartAddr = *l1Ptr;
        }

        return l2StartAddr;
    }

    __aicore__ inline uint64_t LookUpInL2(uint64_t l2StartAddr, uint64_t addr)
    {
        l2Tbl_.listPtr = l2StartAddr;
        __gm__ uint32_t *l2StartPtr = reinterpret_cast<__gm__ uint32_t *>(l2StartAddr);
        uint64_t l2Idx = (addr & l2Tbl_.mask) / l2Tbl_.blockSize;
        return reinterpret_cast<uint64_t>(l2StartPtr + l2Idx);
    }

    ShadowMemoryHeapAllocator *heapAllocator_{nullptr};

    TableLayout l0Tbl_; // 仅一组，长度固定
    TableLayout l1Tbl_; // 每一个PM对应一组
    TableLayout l2Tbl_; // 单字节模型

    bool isReady_{false};
};

// simplified shadow memory running in SIMT VF only
class ShadowMemoryOnline {
    enum MemoryByteStatus {
        NOT_WRITTEN = 0U,
        WRITTEN = 1U,
    };
public:
    struct AuxInfo {
        /*** 线程间踩踏使用 ***/
        SimtThreadLocation conflictedThreadLoc; // 记录被当前线程所踩踏的线程坐标
        uint64_t l1StartAddr;
        uint64_t l2StartAddr;
        uint64_t l2MemStatusAddr; // 字节状态位地址

        __aicore__ inline AuxInfo(): l1StartAddr(0U), l2StartAddr(0U), l2MemStatusAddr(0U)
        {
            conflictedThreadLoc.idX = 0;
            conflictedThreadLoc.idY = 0;
            conflictedThreadLoc.idZ = 0;
        }
    };
    __aicore__ inline ShadowMemoryOnline() : heapAllocator_{}, tables_{},
        isReady_{false} {}

    __aicore__ inline bool Init(uint64_t heapAddr, uint64_t size)
    {
        // cache size中shadow memory ratio的内存由heap统一管理
        if (!heapAllocator_.Init(heapAddr, size)) {
            return false;
        }
        tables_.Init(&(this->heapAllocator_));
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

        if (!IsReady()) {
            return nBadBytesForOverlap;
        }

        for (uint64_t i = 0U; i < addrInfo.size; ++i) {
            uint64_t addr = addrInfo.addr + i;
            uint64_t memStatusAddr = tables_.LookUp(addr, auxInfo.l1StartAddr,
                auxInfo.l2StartAddr, auxInfo.l2MemStatusAddr);

            uint16_t threadId = GetThreadId();
            uint32_t oldValue = ExchangeMemStatus(memStatusAddr,
                static_cast<uint16_t>(MemoryByteStatus::WRITTEN), threadId);
            if ((MemoryByteStatusParser::ExtractStatus(oldValue) ==
                static_cast<uint16_t>(MemoryByteStatus::WRITTEN)) &&
                (MemoryByteStatusParser::ExtractThreadId(oldValue) != threadId)) {
                DecomposeThreadId(threadId, auxInfo.conflictedThreadLoc.idX,
                    auxInfo.conflictedThreadLoc.idY, auxInfo.conflictedThreadLoc.idZ);
                nBadBytesForOverlap++;
            }

            SyncThreads(); // warp间同步
        }

        return nBadBytesForOverlap;
    }

    __aicore__ inline uint32_t ExchangeMemStatus(uint64_t addr, uint16_t isWritten,
        uint16_t threadId)
    {
        uint32_t newValue = MemoryByteStatusParser::Construct(isWritten, threadId);
        return AtomicExch(reinterpret_cast<__gm__ uint32_t *>(addr), newValue);
    }

private:
    ShadowMemoryHeapAllocator heapAllocator_;
    MultiLayerTable tables_;
    bool isReady_;
};

#endif

}

#endif