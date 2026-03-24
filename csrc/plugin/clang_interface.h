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


#ifndef PLUGIN_CLANG_INTERFACE_H
#define PLUGIN_CLANG_INTERFACE_H

#include <map>
#include <cstdint>
#include <string>

#include "core/framework/utility/macros.h"

// 该文件是检测插件与编译器的接口,本身包含2部分内容：
// 1. 对外的接口定义，用于编译器判断哪些需要插桩
// 2. 对内接口注册，用于内部桩接口的自动注册
// 说明：之所以不分离是因为这些接口都很少，请阅读时主动区分

// 唯一的与编译器交互的接口(report接口本身与编译器遵从一样的生成规范)：
extern "C" {
// 由于涉及跨系统的接口，我们直接使用数字代表不同含义，不额外设计枚举
// 0: 不用插桩； 1. 插在前面； 2. 插在后面； 3. 函数替换
#define NO_INSTRUMENTATION 0
#define INSTRUMENTATION_BEFORE 1
#define INSTRUMENTATION_AFTER 2
#define FUNC_SUBSTITUTION 3
uint32_t NeedReport(const char *decoratedName);
}

namespace Sanitizer {

// 对内接口注册部分
class SanitizerAPIRegister {
public:
    void Register(std::string decoratedName, uint32_t place);
    uint32_t Get(std::string decoratedName);
    static SanitizerAPIRegister *GetInstance();

private:
    SanitizerAPIRegister() = default;
    std::map<std::string, uint32_t> decoratedNames_;
};

class SanitizerAPIBuiler {
public:
    SanitizerAPIBuiler(std::string decoratedName, uint32_t place);
};

}  // namespace Sanitizer

// 下面接口用于内部注册需要report的接口和其相对功能接口的位置。在实例化
// SanitizerApiBuiler 对象时通过 __COUNTER__ 计数器为对象变量名加上计数，
// 防止对同名接口重复注册时产生重复定义错误
#define REPORT_API_REGISTER_WITH_PLACE(place, name) \
    static SanitizerAPIBuiler MACRO_CONCAT(sanitizerApiOf##name, __COUNTER__) = \
        Sanitizer::SanitizerAPIBuiler(#name, place)

#define REPORT_API_REGISTER(name) REPORT_API_REGISTER_WITH_PLACE(INSTRUMENTATION_BEFORE, name)

#endif
