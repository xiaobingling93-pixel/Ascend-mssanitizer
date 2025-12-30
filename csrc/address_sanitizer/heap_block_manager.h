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


#ifndef ADDRESS_SANITIZER_HEAP_BLOCK_MANAGER_H
#define ADDRESS_SANITIZER_HEAP_BLOCK_MANAGER_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "core/framework/record_defs.h"
#include "mem_error_def.h"

namespace Sanitizer {

struct HeapBlock {
    uint64_t serialNo{};
    uint64_t addr{};
    uint64_t len{};
    int32_t moduleId{};
    uint32_t lineNo{};
    std::string fileName;
    bool isInvalid{};          // 表示当前的heap是否非法，比如mstx中heap的范围超过了rt malloc的范围

    HeapBlock() = default;
    explicit HeapBlock(MemOpRecord record) : serialNo(record.serialNo), addr(record.dstAddr),
        len(record.memSize), moduleId(record.moduleId), lineNo(record.lineNo),
        fileName(std::string(record.fileName)), isInvalid(false) {}

    friend bool operator<(const HeapBlock &a, const HeapBlock &b)
    {
        return a.addr < b.addr;
    }
};

inline std::string MemInfoSrcToString(MemInfoSrc infoSrc)
{
    switch (infoSrc) {
        case MemInfoSrc::HAL: return "HAL";
        case MemInfoSrc::RT: return "RT";
        case MemInfoSrc::ACL: return "ACL";
        case MemInfoSrc::EXTRA: return "EXTRA";
        case MemInfoSrc::MSTX_HEAP: return "MSTX_HEAP";
        case MemInfoSrc::MSTX_REGION: return "MSTX_REGION";
        case MemInfoSrc::MANUAL: return "MANUAL";
        default: return "BYPASS";
    }
}

enum class HeapType: uint8_t {
    RT = 0,
    DFX,
    MSTX_HEAP,
    MSTX_RIGION,
    BYPASS,
    INVALID,
};

/** @desc HeapBlockManager 提供 heap block 的管理和查询功能
 * 针对 runtime 内存和 adump 等接口上报的 dfx 内存进行分别管理
 */
class HeapBlockManager {
public:
    HeapBlockManager();
    ~HeapBlockManager() = default;
    using HeapBlocks = std::map<uint64_t, HeapBlock>;

    /// 同一个heap下包含多个region，并且不同的region起始地址可能相同，故用vec保存region
    using RegionBlocks = std::map<uint64_t, std::vector<HeapBlock>>;
    using HeapBlockAddFunc = bool (HeapBlockManager::*)(const MemOpRecord &record);
    using HeapBlockFreeFunc = ErrorMsg (HeapBlockManager::*)(const MemOpRecord &record, uint64_t &size);

    bool AddHeapBlock(const MemOpRecord &record);
    ErrorMsg FreeHeapBlock(const MemOpRecord &record, uint64_t &size);
    HeapBlocks const &GetHeapBlocks(void) const { return heapBlocks_; }
    ErrorMsgList DoLeakCheck(void) const;
    uint64_t GetHeapBlockSize(const MemOpRecord &record) const;

private:
    HeapBlocks &GetHeapBlocks(MemInfoSrc infoSrc);
    HeapType GetHeapType(const MemOpRecord &record);
    bool AddRtDfxBlock(const MemOpRecord &record);
    bool AddByPassBlock(const MemOpRecord &record);
    bool AddMstxHeapBlock(const MemOpRecord &record);
    bool AddMstxRegionBlock(const MemOpRecord &record);
    ErrorMsg FreeRtDfxBlock(const MemOpRecord &record, uint64_t &size);
    ErrorMsg FreeByPassBlock(const MemOpRecord &record, uint64_t &size);
    ErrorMsg FreeMstxHeapBlock(const MemOpRecord &record, uint64_t &size);
    ErrorMsg FreeMstxRegionBlock(const MemOpRecord &record, uint64_t &size);
    void DeleteMstxInfo(const MemOpRecord &record, uint64_t freeSize);

private:
    HeapBlocks heapBlocks_;
    HeapBlocks heapBlocksForDfx_;
    HeapBlocks mstxHeapBlocks_;
    RegionBlocks mstxRegionBlocks_;
    std::unordered_map<HeapType, HeapBlockAddFunc> heapAddMap_;
    std::unordered_map<HeapType, HeapBlockFreeFunc> heapFreeMap_;
};

} // namespace Sanitizer

#endif // ADDRESS_SANITIZER_HEAP_BLOCK_MANAGER_H
