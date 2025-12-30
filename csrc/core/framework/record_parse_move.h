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


#ifndef CORE_FRAMEWORK_RECORD_PARSE_MOVE_H
#define CORE_FRAMEWORK_RECORD_PARSE_MOVE_H

#include <vector>

#include "record_defs.h"
#include "event_def.h"
#include "utility/log.h"

namespace Sanitizer {
template<uint32_t alignSize>
inline uint32_t CeilByAlignSize(uint32_t v)
{
    static_assert(alignSize != 0, "align size cannot be zero");
    return ((v + alignSize - 1) / alignSize) * alignSize;
}

inline uint32_t CeilByAlignSize(uint32_t v, uint32_t alignSize)
{
    if (alignSize == 0) {
        SAN_ERROR_LOG("align size cannot be zero");
        return 0;
    }
    return ((v + alignSize - 1) / alignSize) * alignSize;
}

void ParseDmaMovWithByteMode(const DmaMovRecord &dmaMovRecord, std::vector<SanEvent> &events, SanEvent &event);
void ParseDmaMovWithPadMode(const DmaMovRecord& dmaMovRecord, std::vector<SanEvent>& events, SanEvent& event);

} // namespace Sanitizer

#endif
