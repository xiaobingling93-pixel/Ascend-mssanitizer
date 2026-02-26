/* -------------------------------------------------------------------------
 * This file is part of the MindStudio project.
 * Copyright (c) 2026 Huawei Technologies Co.,Ltd.
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

#ifndef REGISTER_SANITIZER_REGISTER_INFO_DISPLAY_H
#define REGISTER_SANITIZER_REGISTER_INFO_DISPLAY_H

#include <vector>
#include <memory>
#include <iostream>
#include "core/framework/event_def.h"
#include "core/framework/call_stack.h"
#include "core/framework/record_format.h"
#include "core/framework/file_mapping.h"
#include "core/framework/runtime_context.h"
#include "register_sanitizer/register_sanitizer.h"

namespace Sanitizer {

inline std::ostream &operator << (std::ostream &os, RegisterDispInfo const &info)
{
    if (info.regType < RegisterType::VECTOR_MASK_0 || info.regType >= RegisterType::MAX) {
        os << "Invalid regType " << static_cast<uint32_t>(info.regType) << std::endl;
        return os;
    }

    os << "[mssanitizer] Warning:Register " << g_regInfoTbl[static_cast<uint32_t>(info.regType)].regNameStr
        << " was not reset to default in block " << info.baseEvent.blockType << "(" << info.baseEvent.coreId << ") "
        << "on kernel " << info.kernelName << ". Expected default value is (" << info.regExpVal.regVal
        << "), but current value is (" << info.regActVal.regVal << ")" << std::endl;
    return os;
}
}

#endif  // REGISTER_SANITIZER_REGISTER_INFO_DISPLAY_H
