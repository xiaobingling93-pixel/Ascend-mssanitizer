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


#ifndef RACE_SANITIZER_RACE_INFO_DISPLAY_H
#define RACE_SANITIZER_RACE_INFO_DISPLAY_H

#include <vector>
#include <memory>
#include <iostream>
#include "core/framework/event_def.h"
#include "core/framework/call_stack.h"
#include "core/framework/record_format.h"
#include "core/framework/file_mapping.h"
#include "core/framework/runtime_context.h"

namespace Sanitizer {

inline std::ostream &PrintClassicLocation(std::ostream &os, uint64_t fileNo, uint64_t lineNo,
    uint64_t serialNo)
{
    return os << "at " << FileMapping::Instance().Query(fileNo).fileName << ":" << lineNo <<
                 " (serialNo:" << serialNo << ")" << std::endl;
}

inline std::ostream &PrintLocationInfo(std::ostream &os, BaseEvent const & event,
    uint64_t serialNo)
{
    if (event.pc == 0UL) {
        return PrintClassicLocation(os, event.fileNo, event.lineNo, serialNo);
    }

    CallStack::Stack stack = CallStack::Instance().Query(event.pc);
    if (stack.empty()) {
        return PrintClassicLocation(os, event.fileNo, event.lineNo, serialNo);
    }

    os << "at pc current 0x" << std::hex << event.pc << std::dec <<
          " (serialNo:" << serialNo << ")" << std::endl;

    return CallStack::Instance().FormatCallStack(os, stack);
}

struct FormatKernelName {};

inline std::ostream &operator<<(std::ostream &os, FormatKernelName const &formatKernelName)
{
    // 竞争异常一定是 kernel 上发生的，因此打印 kernel name
    return os << " in " << RuntimeContext::Instance().kernelNameDisplay;
}

inline std::ostream &operator << (std::ostream &os, RaceDispInfo const &raceInfo)
{
    // 按照指令序号serialNo对事件p1和p2排序，让p1先于p2
    auto const &raceEvent1 = raceInfo.p1.serialNo > raceInfo.p2.serialNo ? raceInfo.p2 : raceInfo.p1;
    auto const &raceEvent2 = raceInfo.p1.serialNo > raceInfo.p2.serialNo ? raceInfo.p1 : raceInfo.p2;

    std::string const &accessType1 =
        raceEvent1.accessType == static_cast<uint8_t>(AccessType::WRITE) ? " Write" : " Read";
    std::string const &accessType2 =
        raceEvent2.accessType == static_cast<uint8_t>(AccessType::WRITE) ? " Write" : " Read";
    std::string errType = accessType2.substr(1, 1) + "A" + accessType1.substr(1, 1);  // 有空格，取"W"和"R"
    os << "====== ERROR: Potential " << errType << " hazard detected at " <<static_cast<MemType>(raceEvent2.memType) <<
        FormatKernelName{} << " on device " << RuntimeContext::Instance().GetDeviceId() << ":" << std::endl;
    os << "======    " << static_cast<PipeType>(raceEvent1.pipeType) << accessType1 << " at " << errType <<
        "()+0x" << std::hex << raceEvent1.addr << std::dec << " in block " << raceEvent1.coreId <<
        " (" << raceEvent1.blockType << ") " <<"on device "<<RuntimeContext::Instance().GetDeviceId()<<" ";
    PrintLocationInfo(os, raceEvent1, raceEvent1.serialNo);
    os << "======    " << static_cast<PipeType>(raceEvent2.pipeType) << accessType2 << " at " << errType <<
        "()+0x" << std::hex << raceEvent2.addr << std::dec << " in block " << raceEvent2.coreId <<
        " (" << raceEvent2.blockType << ") " <<"on device "<<RuntimeContext::Instance().GetDeviceId()<<" ";
    PrintLocationInfo(os, raceEvent2, raceEvent2.serialNo);
    return os;
}

}

#endif
