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


#ifndef SYNC_SANITIZER_SYNC_INFO_DISPLAY_H
#define SYNC_SANITIZER_SYNC_INFO_DISPLAY_H

#include <vector>
#include <memory>
#include <iostream>
#include "core/framework/event_def.h"
#include "core/framework/call_stack.h"
#include "core/framework/record_format.h"
#include "core/framework/file_mapping.h"
#include "core/framework/runtime_context.h"

namespace Sanitizer {

inline std::ostream &PrintClassicLocation(std::ostream &os, BaseEvent const &info)
{
    return os << "======    code in " << FileMapping::Instance().Query(info.fileNo).fileName
              << ":" << info.lineNo << " (serialNo:" << info.serialNo << ")" << std::endl;
}

inline std::ostream &PrintLocationInfo(std::ostream &os, BaseEvent const &info)
{
    if (info.pc == 0UL) {
        return PrintClassicLocation(os, info);
    }

    CallStack::Stack stack = CallStack::Instance().Query(info.pc);
    if (stack.empty()) {
        return PrintClassicLocation(os, info);
    }

    os << "======    code in pc current 0x" << std::hex << info.pc << std::dec
       << " (serialNo:" << info.serialNo << ")" << std::endl;
    for (std::size_t idx = 0UL; idx < stack.size(); ++idx) {
        os << "======    #" << idx << " " << stack[idx].fileName
           << ":" << stack[idx].line << ":" << stack[idx].column << std::endl;
    }
    return os;
}

struct FormatKernelName {};

inline std::ostream &operator<<(std::ostream &os, FormatKernelName const &formatKernelName)
{
    // 同步异常一定是 kernel 上发生的，因此打印 kernel name
    return os << " in " << RuntimeContext::Instance().kernelNameDisplay;
}

inline std::ostream &operator << (std::ostream &os, SyncDispInfo const &info)
{
    os << "====== WARNING: Unpaired set_flag instructions detected" << std::endl
       << "======    from " << static_cast<PipeType>(info.srcPipe) << " to "
       << static_cast<PipeType>(info.dstPipe) << FormatKernelName{} << std::endl
       << "======    in block " << info.baseEvent.blockType << "(" << info.baseEvent.coreId << ")"
       << " on device "<< RuntimeContext::Instance().GetDeviceId() << std::endl;
    PrintLocationInfo(os, info.baseEvent);
    return os;
}
}

#endif  // SYNC_SANITIZER_SYNC_INFO_DISPLAY_H
