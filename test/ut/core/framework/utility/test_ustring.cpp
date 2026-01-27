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


#include "utility/types.h"
#include "utility/ustring.h"

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <iterator>

using namespace Sanitizer;
using namespace Utility;

TEST(UString, join_empty_list_expect_empty_string)
{
    std::vector<std::string> items;
    std::string joined = Join(items.cbegin(), items.cend());
    ASSERT_TRUE(joined.empty());
}

TEST(UString, join_non_empty_list_expect_correct_string)
{
    std::vector<std::string> items = {
        "aaa",
        "bbb",
        "ccc"
    };
    std::string joined = Join(items.cbegin(), items.cend(), "::");
    ASSERT_EQ(joined, "aaa::bbb::ccc");
}

TEST(UString, strip_empty_string_expect_equal_to_self)
{
    std::string str = "";
    ASSERT_EQ(Strip(str), str);
}

TEST(UString, strip_string_with_target_expect_correct_string)
{
    std::string str = "::abc::";
    ASSERT_EQ(Strip(str, ":"), "abc");
}

TEST(UString, strip_string_without_target_expect_equal_to_self)
{
    std::string str = "abc::def";
    ASSERT_EQ(Strip(str, ":"), str);
}

TEST(UString, strip_string_twice_expect_equal_to_strip_once)
{
    std::string str = "::abc::";
    ASSERT_EQ(Strip(str, ":"), Strip(Strip(str, ":"), ":"));
}

TEST(UString, split_empty_string_expect_list_of_one_empty_string)
{
    std::vector<std::string> items;
    Split("", std::back_inserter(items));
    ASSERT_EQ(items.size(), 1UL);
    ASSERT_EQ(items[0], "");
}

TEST(UString, split_string_without_delims_expect_list_of_one_string)
{
    std::vector<std::string> items;
    Split("aaa", std::back_inserter(items));
    ASSERT_EQ(items.size(), 1UL);
    ASSERT_EQ(items[0], "aaa");
}

TEST(UString, split_string_start_with_delims_expect_list_start_with_empty_string)
{
    std::vector<std::string> items;
    Split(":aaa", std::back_inserter(items), ":");
    ASSERT_EQ(items.size(), 2UL);
    ASSERT_EQ(items[0], "");
    ASSERT_EQ(items[1], "aaa");
}

TEST(UString, split_string_end_with_delims_expect_list_end_with_empty_string)
{
    std::vector<std::string> items;
    Split("aaa:", std::back_inserter(items), ":");
    ASSERT_EQ(items.size(), 2UL);
    ASSERT_EQ(items[0], "aaa");
    ASSERT_EQ(items[1], "");
}

TEST(UString, split_string_with_several_delims_expect_correct_list)
{
    std::vector<std::string> items;
    Split(":aaa:bbb:ccc:", std::back_inserter(items), ":");
    ASSERT_EQ(items.size(), 5UL);
    ASSERT_EQ(items[0], "");
    ASSERT_EQ(items[1], "aaa");
    ASSERT_EQ(items[2], "bbb");
    ASSERT_EQ(items[3], "ccc");
    ASSERT_EQ(items[4], "");
}

TEST(UString, format_empty_list_by_human_readable_list_format_expect_return_empty_string)
{
    std::vector<std::string> items;
    std::string ret = HumanReadableListFormat(items.cbegin(), items.cend(), Identity<std::string>());
    ASSERT_TRUE(ret.empty());
}

TEST(UString, format_list_of_one_elem_by_human_readable_list_format_expect_return_elem)
{
    std::vector<std::string> items = {
        "aaa"
    };
    std::string ret = HumanReadableListFormat(items.cbegin(), items.cend(), Identity<std::string>());
    ASSERT_EQ(ret, "aaa");
}

TEST(UString, format_list_of_two_elems_by_human_readable_list_format_expect_return_correct_string)
{
    std::vector<std::string> items = {
        "aaa",
        "bbb"
    };
    std::string ret = HumanReadableListFormat(items.cbegin(), items.cend(), Identity<std::string>());
    ASSERT_EQ(ret, "aaa and bbb");
}

TEST(UString, format_list_of_more_than_two_elems_by_human_readable_list_format_expect_return_correct_string)
{
    std::vector<std::string> items = {
        "aaa",
        "bbb",
        "ccc",
        "ddd"
    };
    std::string ret = HumanReadableListFormat(items.cbegin(), items.cend(), Identity<std::string>());
    ASSERT_EQ(ret, "aaa, bbb, ccc and ddd");
}

TEST(UString, simplify_simple_function_name_expect_return_false)
{
    std::string name = "illegal_read_and_write_kernel";
    std::string simplified;
    ASSERT_FALSE(SimplifyDemangledName(name, simplified));
}

TEST(UString, simplify_mangled_function_name_expect_return_false)
{
    std::string name = "_Z29illegal_read_and_write_kernelIiEvPhS0_";
    std::string simplified;
    ASSERT_FALSE(SimplifyDemangledName(name, simplified));
}

TEST(UString, simplify_function_name_with_incomplete_specialization_expect_return_false)
{
    std::string name = "void illegal_read_and_write_kernel>(unsigned char*, unsigned char*)";
    std::string simplified;
    ASSERT_FALSE(SimplifyDemangledName(name, simplified));
}

TEST(UString, simplify_function_name_without_return_type_expect_return_false)
{
    std::string name = "illegal_read_and_write_kernel(unsigned char*, unsigned char*)";
    std::string simplified;
    ASSERT_FALSE(SimplifyDemangledName(name, simplified));
}

TEST(UString, simplify_function_name_with_empty_name_expect_return_false)
{
    std::string name = "(unsigned char*, unsigned char*)";
    std::string simplified;
    ASSERT_FALSE(SimplifyDemangledName(name, simplified));
}

TEST(UString, simplify_valid_function_name_expect_get_correct_name)
{
    std::string name = "void illegal_read_and_write_kernel(unsigned char*, unsigned char*)";
    std::string simplified;
    ASSERT_TRUE(SimplifyDemangledName(name, simplified));
    ASSERT_EQ(simplified, "illegal_read_and_write_kernel");
}

TEST(UString, simplify_valid_function_name_with_specialization_expect_get_correct_name)
{
    std::string name = "void illegal_read_and_write_kernel<int>(unsigned char*, unsigned char*)";
    std::string simplified;
    ASSERT_TRUE(SimplifyDemangledName(name, simplified));
    ASSERT_EQ(simplified, "illegal_read_and_write_kernel");
}

TEST(UString, simplify_valid_function_name_with_namespace_expect_get_correct_name)
{
    std::string name = "void AscendC::Custom::illegal_read_and_write_kernel(unsigned char*, unsigned char*)";
    std::string simplified;
    ASSERT_TRUE(SimplifyDemangledName(name, simplified));
    ASSERT_EQ(simplified, "AscendC::Custom::illegal_read_and_write_kernel");
}

TEST(UString, simplify_valid_function_name_with_namespace_in_return_type_expect_get_correct_name)
{
    std::string name = "AscendC::Custom::T illegal_read_and_write_kernel(unsigned char*, unsigned char*)";
    std::string simplified;
    ASSERT_TRUE(SimplifyDemangledName(name, simplified));
    ASSERT_EQ(simplified, "illegal_read_and_write_kernel");
}

TEST(UString, simplify_valid_function_name_with_namespace_in_parameters_expect_get_correct_name)
{
    std::string name = "void illegal_read_and_write_kernel(unsigned char*, AscendC::Custom::Tiling)";
    std::string simplified;
    ASSERT_TRUE(SimplifyDemangledName(name, simplified));
    ASSERT_EQ(simplified, "illegal_read_and_write_kernel");
}