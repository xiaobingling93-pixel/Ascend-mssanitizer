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

#include "hooks_verify.h"
#include "hook_logger.h"

namespace Sanitizer {

bool IsValidHalMemcpy2DArg(MEMCPY2D *pCopy)
{
    if (!pCopy) {
        HOOK_LOG("[ERROR] Argument pCopy is null.");
        return false;
    }
    uint64_t dst = reinterpret_cast<uint64_t>(pCopy->copy2d.dst);
    uint64_t src = reinterpret_cast<uint64_t>(pCopy->copy2d.src);
    bool isDstDevicePtr = halIsDevicePtr(dst);
    bool isSrcDevicePtr = halIsDevicePtr(src);
    uint64_t dpitch = pCopy->copy2d.dpitch;
    uint64_t spitch = pCopy->copy2d.spitch;

    if (pCopy->type == DEVMM_MEMCPY2D_SYNC || pCopy->type == DEVMM_MEMCPY2D_ASYNC_CONVERT) {
        // 地址参数回绕防护
        if (isDstDevicePtr && (pCopy->copy2d.height > 1) && (dpitch != 0)) {
            if (UINT64_MAX / dpitch < pCopy->copy2d.height - 1) {
                HOOK_LOG("[ERROR] The product of height - 1(%llu) and dpitch(%llu) exceeds the scale.",
                    pCopy->copy2d.height - 1,
                    dpitch);
                return false;
            }
            if (UINT64_MAX - dst < dpitch * (pCopy->copy2d.height - 1)) {
                HOOK_LOG("[ERROR] The real dst address exceeds the scale.");
                return false;
            }
        }
        if (isSrcDevicePtr && (pCopy->copy2d.height > 1) && (spitch != 0)) {
            if (UINT64_MAX / spitch < pCopy->copy2d.height - 1) {
                HOOK_LOG("[ERROR] The product of height - 1(%llu) and spitch(%llu) exceeds the scale.",
                    pCopy->copy2d.height - 1,
                    spitch);
                return false;
            }
            if (UINT64_MAX - src < spitch * (pCopy->copy2d.height - 1)) {
                HOOK_LOG("[ERROR] The real src address exceeds the scale.");
                return false;
            }
        }
    }
    return true;
}

bool IsValidRtMemcpy2dAsyncArg(
    void *dst, size_t dpitch, const void *src, size_t spitch, size_t height, aclrtMemcpyKind kind)
{
    uint64_t dstAddr = reinterpret_cast<uint64_t>(dst);
    uint64_t srcAddr = reinterpret_cast<uint64_t>(src);
    if ((kind == ACL_MEMCPY_HOST_TO_DEVICE || kind == ACL_MEMCPY_DEVICE_TO_DEVICE) && (height > 1) && (dpitch != 0)) {
        if (UINT64_MAX / dpitch < height - 1) {
            HOOK_LOG(
                "[ERROR] The product of height - 1(%llu) and dpitch(%llu) exceeds the scale.", height - 1, dpitch);
            return false;
        }
        if (UINT64_MAX - dstAddr < dpitch * (height - 1)) {
            HOOK_LOG("[ERROR] real dst address exceeds the scale.");
            return false;
        }
    }
    if ((kind == ACL_MEMCPY_DEVICE_TO_HOST || kind == ACL_MEMCPY_DEVICE_TO_DEVICE) && (height > 1) && (spitch != 0)) {
        if (UINT64_MAX / spitch < height - 1) {
            HOOK_LOG(
                "[ERROR] The product of height - 1(%llu) and spitch(%llu) exceeds the scale.", height - 1, spitch);
            return false;
        }
        if (UINT64_MAX - srcAddr < spitch * (height - 1)) {
            HOOK_LOG("[ERROR] real src address exceeds the scale.");
            return false;
        }
    }
    return true;
}

} // namespace Sanitizer
