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

#ifndef CORE_FRAMEWORK_ARCH_DEF_H
#define CORE_FRAMEWORK_ARCH_DEF_H

#include <cstdint>

// 定义和芯片体系相关的数据信息

namespace Sanitizer {

/// AclrtMemcpy2d的高度最大值
constexpr uint64_t MAX_MEMORY_RECORD_HEIGHT = 60ULL * 1024 * 1024 * 1024;

/// 此枚举定义与编译器内置类型 pipe_t 保持一致，编译器内存类型为uint32_t，不会变动
enum class PipeType : uint32_t {
    PIPE_S = 0,      // Scalar Pipe
    PIPE_V,          // Vector Pipe, including{VectorOP write UB,  L0C->UB write}
    PIPE_M,          // Matrix Pipe, including{}
    PIPE_MTE1,       // L1->L0{A,B}
    PIPE_MTE2,       // OUT ->{L1, L0{A,B}, UB}
    PIPE_MTE3,       // UB ->{OUT,L1}
    PIPE_ALL,
    PIPE_MTE4 = 7,   // MOV_UB_TO_OUT
    PIPE_MTE5 = 8,   // MOV_OUT_TO_UB
    PIPE_V2 = 9,     // Lower priority vector pipe,
    PIPE_FIX = 10,   // {L0C} ->{L1,UB,L1UB}
    SIZE,
};

/// 此枚举定义与编译器内置类型 mem_t 保持一致
enum class MemType : uint8_t {
    L1 = 0,
    L0A,
    L0B,
    L0C,
    UB,
    BT,         // bias table
    GM,
    PRIVATE,
    FB,         // fixPipe buffer
    INVALID,
};

enum class DeviceType : uint32_t {
    ASCEND_910_PREMIUM_A = 0U,
    ASCEND_910B1,
    ASCEND_910B2,
    ASCEND_910B3,
    ASCEND_910B4,
    ASCEND_310P,
    ASCEND_910_950z, // start of 910_95 enum, all 910_95 chip type enum should be in range below
    ASCEND_910_9579,
    ASCEND_910_957b,
    ASCEND_910_957d,
    ASCEND_910_9581,
    ASCEND_910_9589,
    ASCEND_910_958a,
    ASCEND_910_958b,
    ASCEND_910_9599, // end of 910_95 enum
    INVALID = 0xFF,
};

enum class KernelType : uint8_t {
    AICPU = 0,        /// 对应 RTS 中的 RT_DEV_BINARY_MAGIC_ELF_AICPU
    AIVEC,            /// 对应 RTS 中的 RT_DEV_BINARY_MAGIC_ELF_AIVEC
    AICUBE,           /// 对应 RTS 中的 RT_DEV_BINARY_MAGIC_ELF_AICUBE
    MIX,              /// 对应 RTS 中的 RT_DEV_BINARY_MAGIC_ELF
};

}  // namespace Sanitizer

#endif
