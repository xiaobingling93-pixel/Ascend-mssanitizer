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


#include "record_parse.h"
#include "address_sanitizer/align_checker.h"

#include "record_parse_ctrl.h"

namespace Sanitizer {

void ParseRecordCmpMask(const KernelRecord &record, std::vector<SanEvent> &events)
{
    SanEvent event;
    auto& memInfo = event.eventInfo.memInfo;
    auto& cmpMaskRecord = record.payload.cmpMaskRecord;
    SetLocationInfo(event, cmpMaskRecord, record.blockType, record.serialNo);
    event.type = EventType::MEM_EVENT;
    event.pipe = PipeType::PIPE_V;
    memInfo.memType = MemType::UB;
    memInfo.opType = cmpMaskRecord.accessType;
    memInfo.vectorMask = { static_cast<uint64_t>(-1), static_cast<uint64_t>(-1) };
    memInfo.maskMode = MaskMode::MASK_NORM;
    memInfo.dataBits = 8U;
    memInfo.addr = cmpMaskRecord.addr;
    memInfo.blockNum = 1U;
    memInfo.blockSize = cmpMaskRecord.size;
    memInfo.blockStride = 1U;
    memInfo.repeatTimes = 1U;
    memInfo.repeatStride = 1U;

    AlignChecker::Instance().CheckAlign(event, 16UL);
    events.emplace_back(event);
}

} // namespace Sanitizer
