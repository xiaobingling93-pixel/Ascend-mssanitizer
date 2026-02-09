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

#ifndef PLUGIN_RECORD_CTRL_INSTRUCTIONS_H
#define PLUGIN_RECORD_CTRL_INSTRUCTIONS_H

#include "core/framework/record_defs.h"
#include "kernel_pub_func.h"
#include "utils.h"
#include "recorder.h"

namespace Sanitizer {

template <typename T>
__aicore__ inline void RecordRegister(EXTRA_PARAMS_DEC, T Register::*reg, T value)
{
#if defined(BUILD_DYNAMIC_PROBE)
    (void)bid;
#else
    (void)fileNo;
    (void)lineNo;
#endif
    (void)pc;
    if (InvalidMemInfo(memInfo)) {
        return;
    }

    uint64_t blockIdx = GetBlockIdx();
    Recorder recorder(memInfo, blockIdx);
    recorder.SetRegister(reg, value);
}

__aicore__ inline void RecordVectorMask(EXTRA_PARAMS_DEC, uint64_t reg_idx, uint64_t reg_value)
{
    static constexpr uint64_t vectorMaskIdx0 = 0UL;
    static constexpr uint64_t vectorMaskIdx1 = 1UL;
    if (reg_idx == vectorMaskIdx0) {
        RecordRegister(EXTRA_PARAMS, &Register::vectorMask0, reg_value);
    } else if (reg_idx == vectorMaskIdx1) {
        RecordRegister(EXTRA_PARAMS, &Register::vectorMask1, reg_value);
    }
}

__aicore__ inline void RecordNdPara(EXTRA_PARAMS_DEC, uint64_t reg_value)
{
    RecordRegister(EXTRA_PARAMS, &Register::ndParaConfig, reg_value);
}


__aicore__ inline void RecordCmpMask(EXTRA_PARAMS_DEC, __ubuf__ void *addr, AccessType accessType, uint64_t size)
{
    if (InvalidMemInfo(memInfo)) {
        return;
    }
    uint64_t blockIdx = GetBlockIdx();
    CmpMaskRecord record {};
    record.addr = reinterpret_cast<uint64_t>(addr);
    record.size = size;
    record.location.blockId = blockIdx;
    record.accessType = accessType;
#if !defined(BUILD_DYNAMIC_PROBE)
    record.location.fileNo = fileNo;
    record.location.lineNo = lineNo;
#endif
    record.location.pc = static_cast<uint64_t>(pc);

    Recorder recorder(memInfo, blockIdx);
    recorder.DumpRecord<RecordType::CMPMASK_OP>(record);
}


__aicore__ inline uint64_t ExtractVaBits(uint64_t value)
{
    // 右移5位，使第5位成为最低位
    value >>= 5;
    // 使用掩码0x1FFF（即13个1）保留低13位
    return value & 0x1FFF;
}

 __aicore__ inline void SetVa32Bits(VaRegister & va, uint16_t Xn, uint16_t Xm, uint8_t idx)
 {
    // 检查idx是否为合法值（0、2、4、6）
    if ((idx != 0) && (idx != 2) && (idx != 4) && (idx != 6)) {
        return; // 可扩展错误处理
    }
 
    uint64_t* target = nullptr; // 目标uint64_t指针
    uint8_t blockIdx = 0;       // 块索引（0或2，表示起始的16位块）
 
    constexpr uint32_t lowIdxMax = 2;
    constexpr uint32_t highIdxOffset = 4;
    // 确定目标块（l64/h64）和块索引
    if (idx <= lowIdxMax) {
        target = &va.l64;
        blockIdx = idx;
    } else {
        target = &va.h64;
        blockIdx = idx - highIdxOffset;
    }
 
    // 构建32位数据：Xn在低16位，Xm在高16位
    uint32_t data32 = (uint32_t(Xm) << 16) | Xn;

    // 计算位移量（以0位为起点）
    uint8_t shift = blockIdx * 16;
 
    // 创建掩码：清除目标32位，其他位保留
    uint64_t mask = ~(uint64_t(0xFFFFFFFF) << shift);
 
    // 写入数据
    *target = (*target & mask) | (uint64_t(data32) << shift);
}

__aicore__ inline void UpdateVaRegister(EXTRA_PARAMS_DEC, VaRegister Register::*reg,
                                        uint32_t bitOffset, uint64_t xn, uint64_t xm)
{
    if (InvalidMemInfo(memInfo)) {
        return;
    }
    uint64_t blockIdx = GetBlockIdx();
    Recorder recorder(memInfo, blockIdx);
    uint16_t xnValue = static_cast<uint16_t>(ExtractVaBits(xn));
    uint16_t xmValue = static_cast<uint16_t>(ExtractVaBits(xm));
    VaRegister vaRegister;
    recorder.GetRegister(reg, vaRegister);
    SetVa32Bits(vaRegister, xnValue, xmValue, bitOffset);
    recorder.SetRegister(reg, vaRegister);
}

__aicore__ inline void RecordVAdPara(EXTRA_PARAMS_DEC, ub_addr8_t addr, uint32_t bitOffset, uint64_t xn, uint64_t xm)
{
    #if defined(__CCE_IS_AICORE__) && __CCE_IS_AICORE__ == 1
    switch (addr) {
        case VA0: UpdateVaRegister(EXTRA_PARAMS, &Register::va0, bitOffset, xn, xm); break;
        case VA1: UpdateVaRegister(EXTRA_PARAMS, &Register::va1, bitOffset, xn, xm); break;
        case VA2: UpdateVaRegister(EXTRA_PARAMS, &Register::va2, bitOffset, xn, xm); break;
        case VA3: UpdateVaRegister(EXTRA_PARAMS, &Register::va3, bitOffset, xn, xm); break;
        case VA4: UpdateVaRegister(EXTRA_PARAMS, &Register::va4, bitOffset, xn, xm); break;
        case VA5: UpdateVaRegister(EXTRA_PARAMS, &Register::va5, bitOffset, xn, xm); break;
        case VA6: UpdateVaRegister(EXTRA_PARAMS, &Register::va6, bitOffset, xn, xm); break;
        case VA7: UpdateVaRegister(EXTRA_PARAMS, &Register::va7, bitOffset, xn, xm); break;
        default:
            break;
    }
    #endif
}

__aicore__ inline void RecordLdva(EXTRA_PARAMS_DEC, ub_addr8_t dst, uint64_t src, bool h)
{
#if defined(__CCE_IS_AICORE__) && __CCE_IS_AICORE__ == 1
    auto *s = reinterpret_cast<__ubuf__ uint64_t *>(src);
    VaRegister va;
    // h=true表示取高128位
    if (h) {
        va.l64 = *(s + 0x2);
        va.h64 = *(s + 0x3);
    } else {
        va.l64 = *s;
        va.h64 = *(s + 0x1);
    }
    switch (dst) {
        case VA0: RecordRegister(EXTRA_PARAMS, &Register::va0, va); break;
        case VA1: RecordRegister(EXTRA_PARAMS, &Register::va1, va); break;
        case VA2: RecordRegister(EXTRA_PARAMS, &Register::va2, va); break;
        case VA3: RecordRegister(EXTRA_PARAMS, &Register::va3, va); break;
        case VA4: RecordRegister(EXTRA_PARAMS, &Register::va4, va); break;
        case VA5: RecordRegister(EXTRA_PARAMS, &Register::va5, va); break;
        case VA6: RecordRegister(EXTRA_PARAMS, &Register::va6, va); break;
        case VA7: RecordRegister(EXTRA_PARAMS, &Register::va7, va); break;
        default:
            break;
    }
#endif

    if (InvalidMemInfo(memInfo)) {
        return;
    }
    uint64_t blockIdx = GetBlockIdx();
    LoadStoreRecord record {};
    record.addr = src;
    record.size = 32U; // 固定读32B
#if !defined(BUILD_DYNAMIC_PROBE)
    record.location.fileNo = fileNo;
    record.location.lineNo = lineNo;
#endif
    record.location.pc = static_cast<uint64_t>(pc);
    record.space = AddressSpace::UB;
    record.location.blockId= blockIdx;
    record.alignSize = 32U;

    Recorder recorder(memInfo, blockIdx);
    recorder.DumpRecord<RecordType::LDVA>(record);
}

}  // namespace Sanitizer

#endif  // PLUGIN_RECORD_CTRL_INSTRUCTIONS_H
