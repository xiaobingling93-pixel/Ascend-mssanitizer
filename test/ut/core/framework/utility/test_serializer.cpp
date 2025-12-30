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


#include <gtest/gtest.h>

#include "utility/serializer.h"

using namespace Sanitizer;

struct Foo {
    int a;
    float b;
    double c;
    bool d;
};

TEST(Serializer, serialize_then_deserialize_expect_equal_to_original_struct)
{
    Foo foo{0, 1.0, 2.0, true};
    std::string serialized = Serialize(foo);
    Foo bar;
    ASSERT_TRUE(Deserialize<Foo>(serialized, bar));
    ASSERT_EQ(bar.a, foo.a);
    ASSERT_EQ(bar.b, foo.b);
    ASSERT_EQ(bar.c, foo.c);
    ASSERT_EQ(bar.d, foo.d);
}

TEST(Serializer, serialize_twice_then_deserialize_expect_equal_to_original_struct)
{
    Foo foo{0, 1.0, 2.0, true};
    std::string serialized = Serialize(foo, foo);
    Foo bar;
    ASSERT_TRUE(Deserialize<Foo>(serialized, bar));
    ASSERT_EQ(bar.a, foo.a);
    ASSERT_EQ(bar.b, foo.b);
    ASSERT_EQ(bar.c, foo.c);
    ASSERT_EQ(bar.d, foo.d);
    ASSERT_TRUE(Deserialize<Foo>(serialized.substr(sizeof(Foo)), bar));
    ASSERT_EQ(bar.a, foo.a);
    ASSERT_EQ(bar.b, foo.b);
    ASSERT_EQ(bar.c, foo.c);
    ASSERT_EQ(bar.d, foo.d);
}

TEST(Serializer, deserialize_with_overflow_string_expect_return_true)
{
    Foo foo{0, 1.0, 2.0, true};
    std::string serialized = Serialize(foo);
    Foo bar;
    ASSERT_TRUE(Deserialize<Foo>(serialized + "overflow", bar));
}

TEST(Serializer, deserialize_with_shorter_string_expect_return_false)
{
    Foo foo{0, 1.0, 2.0, true};
    std::string serialized = Serialize(foo);
    Foo bar;
    ASSERT_FALSE(Deserialize<Foo>(serialized.substr(10), bar));
}
