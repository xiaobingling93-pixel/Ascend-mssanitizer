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

#ifndef __CORE_FRAMEWORK_UTILITY_TYPES_H__
#define __CORE_FRAMEWORK_UTILITY_TYPES_H__

#include <functional>

namespace Sanitizer {

/* 通过 Tag 为类型 T 生成不同的类型包装，以实现对相同类型生成不同的重载函数。
 *
 * 如我们有一个类 A 包含 int 型的成员 a 和 b，希望生成两个构造函数分别对 a 和
 * b 进行初始化。因成员 a 和 b 具有相同类型，无法通过类型来实现构造函数的重载，
 * 可以借助 TaggedType 将 int 类型打上标签来实现：
 * class A {
 *     int a;
 *     int b;
 *     using VA = TaggedType<int, struct ATag>;
 *     using VB = TaggedType<int, struct BTag>;
 *     A(VA const & a) : a{a.value} { }
 *     A(VB const & b) : b{b.value} { }
 * };
 */
template <typename T, typename Tag>
struct TaggedType {
    T value;
};

/* 生成器类型
 * 在一些需要惰性求值的场景下传输生成器而不是直接传值
 */
template <typename T>
using Generator = std::function<T(void)>;

} // namespace Sanitizer

#endif // __CORE_FRAMEWORK_UTILITY_TYPES_H__
