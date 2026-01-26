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


#include <cstdlib>
#include <cxxabi.h>
#include <memory>

#include "ustring.h"

namespace Utility {
std::string ReplaceInvalidChar(const std::string &input)
{
    std::string initStr = input;
    size_t invalidPos;
    static const std::unordered_map<std::string, std::string> INVALID_CHAR = {
        {"\n", "\\n"}, {"\f", "\\f"}, {"\r", "\\r"}, {"\b", "\\b"},
        {"\t", "\\t"}, {"\v", "\\v"}, {"\u007F", "\\u007F"}
    };
    for (auto &item: INVALID_CHAR) {
        invalidPos = initStr.find(item.first);
        while (invalidPos != std::string::npos) {
            initStr.replace(invalidPos, item.first.length(), item.second);
            invalidPos = initStr.find(item.first, invalidPos + item.second.length());
        }
    }
    return initStr;
}

bool Demangle(std::string const &name, std::string &demangled)
{
    int status{-1};
    std::unique_ptr<char, decltype(&std::free)> res {
        abi::__cxa_demangle(name.c_str(), nullptr, nullptr, &status), std::free
    };
    if (status == 0) {
        demangled = res.get();
    }
    return status == 0;
}

}
