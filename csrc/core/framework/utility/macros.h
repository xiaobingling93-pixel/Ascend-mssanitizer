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

#ifndef __CORE_FRAMEWORK_UTILITY_MACROS_H__
#define __CORE_FRAMEWORK_UTILITY_MACROS_H__

// 预处理是对 token 进行扫描，因此无法直接对 token 的一部分进行替换，需要借助此辅助宏完成。
// 如 __COUNTER__ 作为一个宏无法直接通过 `x##__COUNTER__` 的方式进行拼接，可以使用如下方式：
// MACRO_CONCAT(x, __COUNTER__)
#define CONCAT_IMPL(x, y) x##y
#define MACRO_CONCAT(x, y) CONCAT_IMPL(x, y)

#endif // __CORE_FRAMEWORK_UTILITY_MACROS_H__
