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


#ifndef CORE_STUB_DEF_H
#define CORE_STUB_DEF_H

#include <cstdint>

extern "C"
{
void AscendCheckInit(const char *chipInfo); // 传入设备信息
void AscendCheckExit(uint8_t *addr, uint64_t size); // 把GM拷出的数据传入

void AscendCheckMalloc(uint64_t addr, uint32_t memSpace, uint64_t len,
    const char *fileName, uint64_t lineNo);
void AscendCheckFree(uint64_t addr, uint32_t memSpace, const char *fileName, uint64_t lineNo);
}

#endif