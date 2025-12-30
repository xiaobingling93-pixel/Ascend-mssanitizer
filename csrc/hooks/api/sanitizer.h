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


#ifndef __HOOKS_API_SANITIZER_H__
#define __HOOKS_API_SANITIZER_H__

#ifdef __cplusplus
extern "C" {
#endif
__attribute__((weak)) void __sanitizer_report_malloc(void *ptr, uint64_t size);
__attribute__((weak)) void __sanitizer_report_free(void *ptr);
#ifdef __cplusplus
}
#endif

/// User interface
static void SanitizerReportMalloc(void *ptr, uint64_t size)
{
    if (__sanitizer_report_malloc) {
        __sanitizer_report_malloc(ptr, size);
    }
}

static void SanitizerReportFree(void *ptr)
{
    if (__sanitizer_report_free) {
        __sanitizer_report_free(ptr);
    }
}

#endif  // __HOOKS_API_SANITIZER_H__
