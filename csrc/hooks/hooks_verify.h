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

#ifndef __HOOKS_VERIFY_H__
#define __HOOKS_VERIFY_H__

#include <cstddef>
#include <cstdint>

#include "acl_hooks.h"
#include "hal_hooks.h"

namespace Sanitizer {

// Acl接口
bool IsValidRtMemcpy2dAsyncArg(
    void *dst, size_t dpitch, const void *src, size_t spitch, size_t height, aclrtMemcpyKind kind);
    
// Hal接口
inline bool halIsDevicePtr(DVdeviceptr pp)
{
    struct DVattribute attr = {0};
    if (drvMemGetAttribute(pp, &attr) != DRV_ERROR_NONE) {
        /// 使用非法地址时会导致获取地址空间失败，此时我们无法区分此地址为 host 侧或 device 侧
        /// 地址，为防止漏检始终对此类地址进行检测
        return true;
    }

    return attr.memType == DV_MEM_LOCK_DEV || attr.memType == DV_MEM_LOCK_DEV_DVPP;
}

bool IsValidHalMemcpy2DArg(MEMCPY2D *pCopy);

} // namespace Sanitizer

#endif

