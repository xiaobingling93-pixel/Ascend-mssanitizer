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

#include "race_alg_factory.h"
#include "cross_core_race_alg_impl.h"
#include "single_pipe_race_alg_impl.h"
#include "race_alg_impl.h"

namespace Sanitizer {
std::shared_ptr<RaceAlgBase> RaceAlgFactory::Create(RaceCheckType racecheckType, KernelType kernelType,
    DeviceType deviceType, uint32_t blockDim)
{
    if (racecheckType == RaceCheckType::SINGLE_BLOCK_CHECK) {
        return std::make_shared<RaceAlgImpl>(kernelType, deviceType, blockDim);
    } else if (racecheckType == RaceCheckType::SINGLE_PIPE_CHECK) {
        return std::make_shared<SinglePipeRaceAlgImpl>(kernelType, deviceType, blockDim);
    } else {
        return std::make_shared<CrossCoreRaceAlgImpl>(kernelType, deviceType, blockDim);
    }
}
}