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


#ifndef CORE_UTILITY_RESULT_H
#define CORE_UTILITY_RESULT_H

#include <string>

namespace Sanitizer {
class Result {
public:
    Result() = default;
    explicit Result(const std::string &str);
    ~Result() = default;
    void Clear();
    bool Fail() const;
    void SetError(const std::string &str);
    std::string GetDescription();
private:
    std::string description_;
    int32_t errorCode_ { 0 }; // 0 stands for success.
};
}

#endif