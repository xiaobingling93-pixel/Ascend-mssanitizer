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


#include <cstdint>

#include "hook_report.h"
#include "record_defs.h"

extern "C" {
void __sanitizer_report_malloc(void *ptr, uint64_t size);
void __sanitizer_report_free(void *ptr);
}

using namespace Sanitizer;

void __sanitizer_report_malloc(void *ptr, uint64_t size)
{
    uint64_t addr = reinterpret_cast<int64_t>(ptr);
    HookReport::Instance().ReportMalloc(addr, size, MemInfoSrc::MANUAL);
    // 在对算子进行检测时不关心实际的内存申请和释放，手动上报的 ReportMalloc 和
    // ReportFree 用于上报在算子设计中可访问的 GM 范围。为了防止手动上报内存结合检测未
    // 使用内存功能同时使用时会产生误报，通过模拟对手动分配内存的 store 操作使该片内
    // 存变为 DEFINED 状态。同时为了兼容日后的 read-before-write 和 write-before-read
    // 检测，依次使用 store 和 load 操作将内存变为可读可写状态。状态变化示意如下：
    // NOACCESS -malloc-> UNDEFINED -store-> DEFINED(R) -load-> DEFINED(RW)
    HookReport::Instance().ReportStore(addr, size, MemInfoSrc::MANUAL);
    HookReport::Instance().ReportLoad(addr, size, MemInfoSrc::MANUAL);
}

void __sanitizer_report_free(void *ptr)
{
    uint64_t addr = reinterpret_cast<int64_t>(ptr);
    HookReport::Instance().ReportFree(addr, MemInfoSrc::MANUAL);
}
