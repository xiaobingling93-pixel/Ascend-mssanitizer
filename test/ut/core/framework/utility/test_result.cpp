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


#include <string>
#include <gtest/gtest.h>
#include "utility/result.h"

using namespace Sanitizer;

TEST(ResultTest, default_construct_result_expect_result_not_failure)
{
    Result result;
    EXPECT_TRUE(!result.Fail());
}

TEST(ResultTest, default_construct_then_set_error_expect_result_an_failure)
{
    Result result;
    std::string msg("error");
    result.SetError(msg);
    EXPECT_TRUE(result.Fail());
    EXPECT_TRUE(result.GetDescription() == msg);
}

TEST(ResultTest, clear_error_msg_expect_result_not_failure)
{
    Result result;
    result.SetError("some error");
    result.Clear();
    EXPECT_TRUE(!result.Fail());
}

TEST(ResultTest, construct_with_msg_expect_result_an_failure)
{
    std::string msg("error");
    Result result(msg);
    EXPECT_TRUE(result.Fail());
    EXPECT_TRUE(result.GetDescription() == msg);
    result.Clear();
    EXPECT_TRUE(!result.Fail());
}
