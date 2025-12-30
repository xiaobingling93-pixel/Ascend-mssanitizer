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
#include "plugin/record_move_instructions.h"
#include "plugin/record_ctrl_instructions.h"
 
using namespace Sanitizer;
 
// 2126
SANITIZER_REPORT(set_fmatrix, uint64_t config)
{
    RecordRegister(EXTRA_PARAMS, &Register::fmatrix, config);
}
 
// 2127
SANITIZER_REPORT(set_fmatrixB, uint64_t config)
{
    RecordRegister(EXTRA_PARAMS, &Register::fmatrixB, config);
}
 
// 2168
SANITIZER_REPORT(set_l3d_rpt, uint64_t config)
{
    RecordRegister(EXTRA_PARAMS, &Register::l3dRpt, config);
}
 
// 895
SANITIZER_REPORT(img2colv2_cbuf_to_ca, __ca__ __bf16 *dst, __cbuf__ __bf16 *src, uint64_t config0, uint64_t config1)
{
    RecordLoad3DEvent<MemType::L1, MemType::L0A, DataType::DATA_B16>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config0, config1);
}
 
// 897
SANITIZER_REPORT(img2colv2_cbuf_to_ca, __ca__ half *dst, __cbuf__ half *src, uint64_t config0, uint64_t config1)
{
    RecordLoad3DEvent<MemType::L1, MemType::L0A, DataType::DATA_B16>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config0, config1);
}
 
// 904
SANITIZER_REPORT(img2colv2_cbuf_to_ca, __ca__ float *dst, __cbuf__ float *src, uint64_t config0, uint64_t config1)
{
    RecordLoad3DEvent<MemType::L1, MemType::L0A, DataType::DATA_B32>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config0, config1);
}
 
// 935
SANITIZER_REPORT(img2colv2_cbuf_to_ca, __ca__ int32_t *dst, __cbuf__ int32_t *src, uint64_t config0, uint64_t config1)
{
    RecordLoad3DEvent<MemType::L1, MemType::L0A, DataType::DATA_B32>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config0, config1);
}
 
// 942
SANITIZER_REPORT(img2colv2_cbuf_to_ca, __ca__ int8_t *dst, __cbuf__ int8_t *src, uint64_t config0, uint64_t config1)
{
    RecordLoad3DEvent<MemType::L1, MemType::L0A, DataType::DATA_B8>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config0, config1);
}
 
// 949
SANITIZER_REPORT(img2colv2_cbuf_to_ca, __ca__ uint32_t *dst, __cbuf__ uint32_t *src, uint64_t config0, uint64_t config1)
{
    RecordLoad3DEvent<MemType::L1, MemType::L0A, DataType::DATA_B32>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config0, config1);
}
 
// 956
SANITIZER_REPORT(img2colv2_cbuf_to_ca, __ca__ uint8_t *dst, __cbuf__ uint8_t *src, uint64_t config0, uint64_t config1)
{
    RecordLoad3DEvent<MemType::L1, MemType::L0A, DataType::DATA_B8>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config0, config1);
}
 
// 967
SANITIZER_REPORT(img2colv2_cbuf_to_ca_s4, __ca__ void *dst, __cbuf__ void *src, uint64_t config0, uint64_t config1)
{
    RecordLoad3DEvent<MemType::L1, MemType::L0A, DataType::DATA_B4>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config0, config1);
}
 
// 974
SANITIZER_REPORT(img2colv2_cbuf_to_cb, __cb__ __bf16 *dst, __cbuf__ __bf16 *src, uint64_t config0, uint64_t config1)
{
    RecordLoad3DEvent<MemType::L1, MemType::L0B, DataType::DATA_B16>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config0, config1);
}
 
// 976
SANITIZER_REPORT(img2colv2_cbuf_to_cb, __cb__ half *dst, __cbuf__ half *src, uint64_t config0, uint64_t config1)
{
    RecordLoad3DEvent<MemType::L1, MemType::L0B, DataType::DATA_B16>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config0, config1);
}
 
// 983
SANITIZER_REPORT(img2colv2_cbuf_to_cb, __cb__ float *dst, __cbuf__ float *src, uint64_t config0, uint64_t config1)
{
    RecordLoad3DEvent<MemType::L1, MemType::L0B, DataType::DATA_B32>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config0, config1);
}
 
// 998
SANITIZER_REPORT(img2colv2_cbuf_to_cb, __cb__ int32_t *dst, __cbuf__ int32_t *src, uint64_t config0, uint64_t config1)
{
    RecordLoad3DEvent<MemType::L1, MemType::L0B, DataType::DATA_B32>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config0, config1);
}
 
// 1012
SANITIZER_REPORT(img2colv2_cbuf_to_cb, __cb__ uint32_t *dst, __cbuf__ uint32_t *src, uint64_t config0, uint64_t config1)
{
    RecordLoad3DEvent<MemType::L1, MemType::L0B, DataType::DATA_B32>(EXTRA_PARAMS, reinterpret_cast<uint64_t>(dst),
        reinterpret_cast<uint64_t>(src), config0, config1);
}
