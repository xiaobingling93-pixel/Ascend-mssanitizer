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

#ifndef CORE_FRAMEWORK_TYPE_MAP_H
#define CORE_FRAMEWORK_TYPE_MAP_H

#include <utility>
#include <type_traits>

/* 编译期 值 => 类型 映射(Value to Type mapping in compile time)
 * 原理: 多继承和重载
 * 每一个type_pair类型里都定义了一个签名不同的get函数，返回一个Value类型的对象
 * type_map会多继承若干个type_pair并将他们的get函数暴露给自己
 * 再定义一个别名模板(alias template)，其模板参数为KeyType的值
 * 在其定义里使用重载决议来得到映射的类型ValueType 从而达到 值 => 类型 映射的效果
 */

template<typename KeyType, KeyType key, typename ValueType>
struct type_pair {
    // 用于重载决议的get函数
    // 这里的integral_constant是一个模板，对不同的值生成不同的类型
    // 使下面的重载决议能够达到不同的值使用不同的重载的效果
    // 此函数应该仅用于未求值上下文(unevaluated context)，否则编译会报错
    static ValueType get(std::integral_constant<KeyType, key> /* unused */)
    {
        return std::declval<ValueType>();
    }
};

/* type_map 类型通过递归继承的方式实现，每一层都继承可变参数中的第一个 TypePair
 * 和剩余参数 TypePairs 实例化出的 type_map，每一层 type_map 实例向外提供对应的
 * type 成员。
 * type_map 继承关系示意图如下：
 *       type_map<TypePair0, TypePair1, ..., TypePairN>
 *                  /                     \
 *              TypePair0     type_map<TypePair1, ..., TypePairN>
 *                                    /               \
 *                                TypePair1    type_map<..., TypePairN>
 *                                                       \
 *                                                      (...)
 *                                                         \
 *                                                  type_map<TypePairN>
 *                                                        /
 *                                                    TypePairN
 */
template <typename KeyType, typename TypePair, typename... TypePairs>
struct type_map : public TypePair, public type_map<KeyType, TypePairs...> {
    using TypePair::get;
    using type_map<KeyType, TypePairs...>::get;

    // 使用decltype和重载决议实现 值 => 类型 的映射
    // 具体使用的样例，请参见plugin/recorder.h
    template <KeyType key>
    using type = decltype(get(std::declval<std::integral_constant<KeyType, key>>()));
};

/* type_map 类型 partial-specialization 实现，用于表示递归继承的终点
 */
template <typename KeyType, typename TypePair>
struct type_map<KeyType, TypePair> : public TypePair {
    using TypePair::get;

    template <KeyType key>
    using type = decltype(get(std::declval<std::integral_constant<KeyType, key>>()));
};

#endif // !CORE_FRAMEWORK_TYPE_MAP_H
