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


#ifndef __HOOKS_API_ACL_H__
#define __HOOKS_API_ACL_H__

#include <acl/acl.h>

#ifdef __cplusplus
extern "C" {
#endif
aclError sanitizerRtMalloc(void **devPtr, size_t size, aclrtMemMallocPolicy policy,
                           char const *filename, int lineno);
aclError sanitizerRtMallocCached(void **devPtr, size_t size, aclrtMemMallocPolicy policy,
                                 char const *filename, int lineno);
aclError sanitizerRtFree(void *devPtr, char const *filename, int lineno);
aclError sanitizerRtMemset(void *devPtr, size_t maxCount, int32_t value, size_t count,
                           char const *filename, int lineno);
aclError sanitizerRtMemsetAsync(void *devPtr, size_t maxCount, int32_t value, size_t count, aclrtStream stream,
                                char const *filename, int lineno);
aclError sanitizerRtMemcpy(void *dst, size_t destMax, const void *src, size_t count, aclrtMemcpyKind kind,
                           char const *filename, int lineno);
aclError sanitizerRtMemcpyAsync(void *dst, size_t destMax, const void *src, size_t count, aclrtMemcpyKind kind,
                                aclrtStream stream, char const *filename, int lineno);
aclError sanitizerRtMemcpy2d(void *dst, size_t dpitch, const void *src, size_t spitch, size_t width, size_t height,
                             aclrtMemcpyKind kind, char const *filename, int lineno);
aclError sanitizerRtMemcpy2dAsync(void *dst, size_t dpitch, const void *src, size_t spitch, size_t width, size_t height,
                                  aclrtMemcpyKind kind, aclrtStream stream, char const *filename, int lineno);
#ifdef __cplusplus
}
#endif

/// User interface
#define aclrtMalloc(devPtr, size, policy) sanitizerRtMalloc(devPtr, size, policy, __FILE__, __LINE__)
#define aclrtMallocCached(devPtr, size, policy) sanitizerRtMallocCached(devPtr, size, policy, __FILE__, __LINE__)
#define aclrtFree(devPtr) sanitizerRtFree(devPtr, __FILE__, __LINE__)
#define aclrtMemset(devPtr, maxCount, value, count) \
    sanitizerRtMemset(devPtr, maxCount, value, count, __FILE__, __LINE__)
#define aclrtMemsetAsync(devPtr, maxCount, value, count, stream) \
    sanitizerRtMemsetAsync(devPtr, maxCount, value, count, stream, __FILE__, __LINE__)
#define aclrtMemcpy(dst, destMax, src, count, kind) \
    sanitizerRtMemcpy(dst, destMax, src, count, kind, __FILE__, __LINE__)
#define aclrtMemcpyAsync(dst, destMax, src, count, kind, stream) \
    sanitizerRtMemcpyAsync(dst, destMax, src, count, kind, stream, __FILE__, __LINE__)
#define aclrtMemcpy2d(dst, dpitch, src, spitch, width, height, kind) \
    sanitizerRtMemcpy2d(dst, dpitch, src, spitch, width, height, kind, __FILE__, __LINE__)
#define aclrtMemcpy2dAsync(dst, dpitch, src, spitch, width, height, kind, stream) \
    sanitizerRtMemcpy2dAsync(dst, dpitch, src, spitch, width, height, kind, stream, __FILE__, __LINE__)

#endif  // __HOOKS_API_ACL_H__
