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


#include <type_traits>
#include <gtest/gtest.h>
#include "type_map.h"

enum class Type {
    INVALID,
    CHAR,
    SHORT,
    INT,
    LONG,
    I32,
};

template<Type key, typename ValueType>
using TypePair = type_pair<Type, key, ValueType>;

template<typename... TypePairs>
using TypeMap = type_map<Type, TypePairs...>;

using Mapping = TypeMap<
    TypePair<Type::CHAR, char>,
    TypePair<Type::SHORT, short>,
    TypePair<Type::INT, int>,
    TypePair<Type::LONG, long>,
    TypePair<Type::I32, int>
>;

TEST(TypeMap, test_match)
{
    static_assert(std::is_same<Mapping::type<Type::CHAR>, char>::value, "Type::CHAR must match with char!");
    static_assert(std::is_same<Mapping::type<Type::SHORT>, short>::value, "Type::SHORT must match with short!");
    static_assert(std::is_same<Mapping::type<Type::INT>, int>::value, "Type::INT must match with int!");
    static_assert(std::is_same<Mapping::type<Type::LONG>, long>::value, "Type::LONG must match with long!");
    static_assert(std::is_same<Mapping::type<Type::I32>, int>::value, "Type::I32 must match with int!");
}

TEST(TypeMap, test_mismatch)
{
    static_assert(!std::is_same<Mapping::type<Type::CHAR>, short>::value, "Type::CHAR must not match with short!");
    static_assert(!std::is_same<Mapping::type<Type::CHAR>, int>::value, "Type::CHAR must not match with int!");
    static_assert(!std::is_same<Mapping::type<Type::CHAR>, long>::value, "Type::CHAR must not match with long!");
    static_assert(!std::is_same<Mapping::type<Type::CHAR>, float>::value, "Type::CHAR must not match with float!");
    static_assert(!std::is_same<Mapping::type<Type::CHAR>, Type>::value, "Type::CHAR must not match with Type!");
}
