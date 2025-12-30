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


#ifndef CORE_FRAMEWORK_PLATFORM_CONFIG_H
#define CORE_FRAMEWORK_PLATFORM_CONFIG_H

#include <cstdint>
#include <iostream>
#include <unordered_map>

#include "arch_def.h"

namespace Sanitizer {
constexpr uint32_t UB_ALIGN_SIZE = 32U;
// 根据ASCEND C口径：栈内存按75、32、18（分别对应910B、910A、310P）个核计算，一个核按最大32KB计算（算子编译时可按16/32K配置）
constexpr uint32_t ASCEND_910B_PRIVATE_SIZE = 75 * 32 * 1024;
constexpr uint32_t ASCEND_910A_PRIVATE_SIZE = 32 * 32 * 1024;
constexpr uint32_t ASCEND_310P_PRIVATE_SIZE = 18 * 32 * 1024;
constexpr uint32_t ASCEND_A5_PRIVATE_SIZE = 96 * 32 * 1024;

struct ChipInfo {
    uint32_t aiCoreCnt;
    uint32_t l0aSize;
    uint32_t l0bSize;
    uint32_t l0cSize;
    uint32_t l1Size;
    uint32_t ubSize;
    uint32_t ubBlockSize;
    uint32_t privateSize;
    uint64_t hbmSize;
};

inline bool HasSubBlocks(DeviceType deviceType)
{
    switch (deviceType) {
        case DeviceType::ASCEND_910B1:
        case DeviceType::ASCEND_910B2:
        case DeviceType::ASCEND_910B3:
        case DeviceType::ASCEND_910B4:
        case DeviceType::ASCEND_910_950z:
        case DeviceType::ASCEND_910_9579:
        case DeviceType::ASCEND_910_957b:
        case DeviceType::ASCEND_910_957d:
        case DeviceType::ASCEND_910_9581:
        case DeviceType::ASCEND_910_9589:
        case DeviceType::ASCEND_910_958a:
        case DeviceType::ASCEND_910_958b:
        case DeviceType::ASCEND_910_9599:
            return true;
        case DeviceType::ASCEND_910_PREMIUM_A:
        case DeviceType::ASCEND_310P:
            return false;
        case DeviceType::INVALID:
        default :
            return false;
    }
}

/// 芯片内存信息，通过 ${ASCEND_HOME_PATH}/compiler/data/platform_config 路径获取
const std::unordered_map<DeviceType, ChipInfo> CHIP_INFO_MAP = {
    {DeviceType::ASCEND_910_PREMIUM_A, {32U, 65536U, 65536U, 262144U, 1048576U, 262144U, 32U, ASCEND_910A_PRIVATE_SIZE,
        0x800000000ULL}},
    {DeviceType::ASCEND_910B1, {24U, 65536U, 65536U, 131072U, 524288U, 196608U, 32U, ASCEND_910B_PRIVATE_SIZE,
        0x1000000000ULL}},
    {DeviceType::ASCEND_910B2, {24U, 65536U, 65536U, 131072U, 524288U, 196608U, 32U, ASCEND_910B_PRIVATE_SIZE,
        0x1000000000ULL}},
    {DeviceType::ASCEND_910B3, {20U, 65536U, 65536U, 131072U, 524288U, 196608U, 32U, ASCEND_910B_PRIVATE_SIZE,
        0x1000000000ULL}},
    {DeviceType::ASCEND_910B4, {20U, 65536U, 65536U, 131072U, 524288U, 196608U, 32U, ASCEND_910B_PRIVATE_SIZE,
        0x1000000000ULL}},
    {DeviceType::ASCEND_310P, {8U, 65536U, 65536U, 262144U, 1048576U, 262144U, 32U, ASCEND_310P_PRIVATE_SIZE,
        0x800000000ULL}},
    {DeviceType::ASCEND_910_950z, {8U, 65536U, 65536U, 262144U, 524288U, 253952U, 32U, ASCEND_A5_PRIVATE_SIZE,
        0x400000000ULL}},
    {DeviceType::ASCEND_910_9579, {28U, 65536U, 65536U, 262144U, 524288U, 253952U, 32U, ASCEND_A5_PRIVATE_SIZE,
        0x2000000000ULL}},
    {DeviceType::ASCEND_910_957b, {28U, 65536U, 65536U, 262144U, 524288U, 253952U, 32U, ASCEND_A5_PRIVATE_SIZE,
        0x1C00000000ULL}},
    {DeviceType::ASCEND_910_957d, {28U, 65536U, 65536U, 262144U, 524288U, 253952U, 32U, ASCEND_A5_PRIVATE_SIZE,
        0x1800000000ULL}},
    {DeviceType::ASCEND_910_9581, {32U, 65536U, 65536U, 262144U, 524288U, 253952U, 32U, ASCEND_A5_PRIVATE_SIZE,
        0x2400000000ULL}},
    {DeviceType::ASCEND_910_9589, {32U, 65536U, 65536U, 262144U, 524288U, 253952U, 32U, ASCEND_A5_PRIVATE_SIZE,
        0x2000000000ULL}},
    {DeviceType::ASCEND_910_958a, {32U, 65536U, 65536U, 262144U, 524288U, 253952U, 32U, ASCEND_A5_PRIVATE_SIZE,
        0x1800000000ULL}},
    {DeviceType::ASCEND_910_958b, {32U, 65536U, 65536U, 262144U, 524288U, 253952U, 32U, ASCEND_A5_PRIVATE_SIZE,
        0x1C00000000ULL}},
    {DeviceType::ASCEND_910_9599, {36U, 65536U, 65536U, 262144U, 524288U, 253952U, 32U, ASCEND_A5_PRIVATE_SIZE,
        0x2000000000ULL}},
};

const std::unordered_map<std::string, DeviceType> SOC_VERSION_MAP = {
    {"Ascend910PremiumA", DeviceType::ASCEND_910_PREMIUM_A},
    {"Ascend910B1", DeviceType::ASCEND_910B1},
    {"Ascend910B2", DeviceType::ASCEND_910B2},
    {"Ascend910B2C", DeviceType::ASCEND_910B2},
    {"Ascend910B3", DeviceType::ASCEND_910B3},
    {"Ascend910B4", DeviceType::ASCEND_910B4},
    {"Ascend910B4-1", DeviceType::ASCEND_910B4},
    {"Ascend910_9391", DeviceType::ASCEND_910B1},
    {"Ascend910_9392", DeviceType::ASCEND_910B1},
    {"Ascend910_9381", DeviceType::ASCEND_910B2},
    {"Ascend910_9382", DeviceType::ASCEND_910B2},
    {"Ascend910_9372", DeviceType::ASCEND_910B3},
    {"Ascend910_9362", DeviceType::ASCEND_910B4},
    {"Ascend910_950z", DeviceType::ASCEND_910_950z},
    {"Ascend910_9579", DeviceType::ASCEND_910_9579},
    {"Ascend910_957b", DeviceType::ASCEND_910_957b},
    {"Ascend910_957d", DeviceType::ASCEND_910_957d},
    {"Ascend910_9581", DeviceType::ASCEND_910_9581},
    {"Ascend910_9589", DeviceType::ASCEND_910_9589},
    {"Ascend910_958a", DeviceType::ASCEND_910_958a},
    {"Ascend910_958b", DeviceType::ASCEND_910_958b},
    {"Ascend910_9599", DeviceType::ASCEND_910_9599},
    {"Ascend310P1", DeviceType::ASCEND_310P},
    {"Ascend310P3", DeviceType::ASCEND_310P},
    {"Ascend310P5", DeviceType::ASCEND_310P},
    {"Ascend310P7", DeviceType::ASCEND_310P},
};

const std::unordered_map<std::string, uint16_t> ALIGN_MAP = {
    {"UB_NORMAL_ALIGN_SIZE", 32U},
    {"L0A_L0B_L0C_NORMAL_ALIGN_SIZE",  512U},
    {"L1_NORMAL_ALIGN_SIZE", 32U},
    {"VMRGSORET4_ALIGN_SIZE", 8U},
};
} // namespace Sanitizer

#endif
