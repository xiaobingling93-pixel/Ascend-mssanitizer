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


#ifndef __CORE_FRAMEWORK_CALL_STACK_H__
#define __CORE_FRAMEWORK_CALL_STACK_H__

#include <set>
#include <mutex>
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include "core/framework/utility/singleton.h"

#include "nlohmann/json.hpp"

namespace Sanitizer {

class CallStack : public ThreadSingleton<CallStack> {
public:
    struct Location {
        std::string funcName;
        std::string fileName;
        uint64_t line;
        uint64_t column;
    };

    using Stack = std::vector<Location>;
    using StackCacheMap = std::unordered_map<uint64_t, Stack>;

public:
    void Load(std::vector<char> const &binary);
    void CachePcOffsets(std::set<uint64_t> pcOffsets);
    Stack Query(uint64_t pcOffset);
    friend class std::pair<const std::thread::id, CallStack>;
    void SetIsPrintFullStack(bool isPrintFullStack){ isPrintFullStack_ = isPrintFullStack; }
    bool IsPrintFullStack() const { return isPrintFullStack_; }

private:
    CallStack(void) : isBinaryEmpty_(true) { }
    ~CallStack(void);
    StackCacheMap ParseStacks(std::string const &msg) const;
    void ParseEachStack(nlohmann::json const &stackJson, StackCacheMap &stackCacheMap) const;

    bool isBinaryEmpty_;
    std::string kernelPath_;
    mutable StackCacheMap pcStackMap_;
    std::mutex mtx_;
    bool isPrintFullStack_{false};
};

} // namespace Sanitizer

#endif  // __CORE_FRAMEWORK_CALL_STACK_H__
