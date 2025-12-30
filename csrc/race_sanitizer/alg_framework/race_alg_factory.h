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

#ifndef RACE_SANITIZER_ALG_FRAMEWORK_RACE_ALG_FACTORY_H
#define RACE_SANITIZER_ALG_FRAMEWORK_RACE_ALG_FACTORY_H

#include "race_alg_base.h"

namespace Sanitizer {
class RaceAlgFactory {
public:
    // 根据输入创建具体的算法对象
    static std::shared_ptr<RaceAlgBase> Create(RaceCheckType racecheckType, KernelType kernelType,
        DeviceType deviceType, uint32_t blockDim);
};
}
#endif