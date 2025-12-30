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

#ifndef __CORE_FRAMEWORK_UTILITY_TYPE_TRAITS_H__
#define __CORE_FRAMEWORK_UTILITY_TYPE_TRAITS_H__

#include <type_traits>

namespace Sanitizer {

template <typename T>
using add_const_t = typename std::add_const<T>::type;

template <typename T>
constexpr add_const_t<T>& as_const(T &t) noexcept
{
    return t;
}

} // namespace Sanitizer

#endif // __CORE_FRAMEWORK_UTILITY_TYPE_TRAITS_H__
