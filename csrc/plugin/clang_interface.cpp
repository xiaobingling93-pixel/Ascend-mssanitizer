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


#include "clang_interface.h"

uint32_t NeedReport(const char *decoratedName)
{
    if (decoratedName != nullptr) {
        return Sanitizer::SanitizerAPIRegister::GetInstance()->Get(decoratedName);
    } else {
        return 0;
    }
}

namespace Sanitizer {

void SanitizerAPIRegister::Register(std::string decoratedName, uint32_t place)
{
    decoratedNames[decoratedName] = place;
}

uint32_t SanitizerAPIRegister::Get(std::string decoratedName)
{
    typename decltype(decoratedNames)::const_iterator itr = decoratedNames.find(decoratedName);
    if (itr != decoratedNames.end()) {
        return itr->second;
    }
    return 0;
}

SanitizerAPIRegister *SanitizerAPIRegister::GetInstance()
{
    static SanitizerAPIRegister instance;
    return &instance;
}

SanitizerAPIBuiler::SanitizerAPIBuiler(std::string decoratedName, uint32_t place)
{
    SanitizerAPIRegister::GetInstance()->Register(decoratedName, place);
}

}  // namespace Sanitizer
