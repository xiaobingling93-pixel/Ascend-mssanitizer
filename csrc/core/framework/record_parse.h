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


#ifndef CORE_FRAMEWORK_RECORD_PARSE_H
#define CORE_FRAMEWORK_RECORD_PARSE_H

#include <map>
#include <unordered_map>
#include <unordered_set>

#include "record_defs.h"
#include "event_def.h"

namespace Sanitizer {

std::map<uint16_t, bool>& GetMapAtomicMode();

template <typename Record>
inline void SetLocationInfo(SanEvent &event, Record const &record, const BlockType &blockType,
                            uint64_t serialNo)
{
    event.serialNo = serialNo;
    event.loc.blockType = blockType;
    event.loc.coreId = record.location.blockId;
    event.isAtomicMode = GetMapAtomicMode()[event.loc.coreId];
    event.loc.fileNo = record.location.fileNo;
    event.loc.lineNo = record.location.lineNo;
    event.loc.pc = record.location.pc;
}

class RecordParse {
public:
    static void Parse(const SanitizerRecord &record, std::vector<SanEvent> &events);
    static void ResetSyncInPipeInfo();
    thread_local static std::array<bool, static_cast<uint8_t>(PipeType::SIZE)> setWaitStat_;
    // 用图存储流水间同步信息，key:dst val:[src1, src2, ...]
    using DstSrcGraph = std::unordered_map<PipeType, std::unordered_set<PipeType>>;
    thread_local static DstSrcGraph dstSrcGraph_;
private:
    static void UpdateSyncInPipe(KernelRecord const& record, std::vector<SanEvent> &events);
    /// 递归函数，查找和targetPipe直接或间接相连的src pipe
    static void DfsSrcGraph(PipeType targetPipe, std::unordered_set<PipeType> &visited);
};
}
#endif
