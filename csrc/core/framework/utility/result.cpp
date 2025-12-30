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


#include "result.h"

namespace Sanitizer {
Result::Result(const std::string &str) : description_(str), errorCode_(-1) {}

void Result::Clear()
{
    errorCode_ = 0;
    description_ = "";
}

bool Result::Fail() const
{
    return (errorCode_ != 0);
}

void Result::SetError(const std::string &str)
{
    description_ = str;
    if (errorCode_ == 0) {
        errorCode_ = -1;
    }
}

std::string Result::GetDescription()
{
    return description_;
}
}
