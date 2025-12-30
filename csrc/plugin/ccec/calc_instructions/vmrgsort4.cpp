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


#include "plugin/kernel_pub_func.h"
#include "plugin/utils.h"
#include "plugin/record_calc_instructions.h"

using namespace Sanitizer;

#if !(defined(__CCE_IS_AICORE__) && __CCE_IS_AICORE__ == 1 && defined(__DAV_C220_CUBE__))
// c220的两条指令: half/float
SANITIZER_REPORT(vmrgsort4, __ubuf__ half *dst, __ubuf__ half *src0, uint64_t src1, uint64_t config)
{
    RecordVmrgsort4C220Config(EXTRA_PARAMS, dst, src0, src1, config);
}

SANITIZER_REPORT(vmrgsort4, __ubuf__ float *dst, __ubuf__ float *src0, uint64_t src1, uint64_t config)
{
    RecordVmrgsort4C220Config(EXTRA_PARAMS, dst, src0, src1, config);
}

// m200的两条指令: half/float
SANITIZER_REPORT(vmrgsort4, __ubuf__ half *dst, __ubuf__ half *src, uint64_t config)
{
    RecordVmrgsort4M200Config(EXTRA_PARAMS, dst, src, config);
}

SANITIZER_REPORT(vmrgsort4, __ubuf__ float *dst, __ubuf__ float *src, uint64_t config)
{
    RecordVmrgsort4M200Config(EXTRA_PARAMS, dst, src, config);
}

// c220和m200通用的两条指令: half/float
SANITIZER_REPORT(vmrgsort4, __ubuf__ half *dst, __ubuf__ half *src, uint8_t repeat, uint16_t regionProposalLi0,
                 uint16_t regionProposalLi1, uint16_t regionProposalLi2, uint16_t regionProposalLi3,
                 bool isAllStored, uint8_t maskSignal)
{
#if defined(__DAV_C220__) || defined(__DAV_C220_VEC__)
    RecordVmrgsort4OpFunc<RecordType::VMRGSORT4_OP_C220>(EXTRA_PARAMS, dst, src, repeat, regionProposalLi0,
        regionProposalLi1, regionProposalLi2, regionProposalLi3, isAllStored, maskSignal);
#else
    RecordVmrgsort4OpFunc<RecordType::VMRGSORT4_OP_M200>(EXTRA_PARAMS, dst, src, repeat, regionProposalLi0,
        regionProposalLi1, regionProposalLi2, regionProposalLi3, isAllStored, maskSignal);
#endif
}

SANITIZER_REPORT(vmrgsort4, __ubuf__ float *dst, __ubuf__ float *src, uint8_t repeat, uint16_t regionProposalLi0,
                 uint16_t regionProposalLi1, uint16_t regionProposalLi2, uint16_t regionProposalLi3,
                 bool isAllStored, uint8_t maskSignal)
{
#if defined(__DAV_C220__) || defined(__DAV_C220_VEC__)
    RecordVmrgsort4OpFunc<RecordType::VMRGSORT4_OP_C220>(EXTRA_PARAMS, dst, src, repeat, regionProposalLi0,
        regionProposalLi1, regionProposalLi2, regionProposalLi3, isAllStored, maskSignal);
#else
    RecordVmrgsort4OpFunc<RecordType::VMRGSORT4_OP_M200>(EXTRA_PARAMS, dst, src, repeat, regionProposalLi0,
        regionProposalLi1, regionProposalLi2, regionProposalLi3, isAllStored, maskSignal);
#endif
}

#endif