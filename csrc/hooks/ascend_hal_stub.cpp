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


#include "ascend_hal.h"

drvError_t drvMemGetAttribute(DVdeviceptr vptr, struct DVattribute *attr)
{
    (void)vptr;
    (void)attr;
    //  返回异常值用于ut
    return DRV_ERROR_BAD_ADDRESS;
}

drvError_t halMemAllocInner(void **pp, unsigned long long size, unsigned long long flag)
{
    (void)pp;
    (void)size;
    (void)flag;
    return DRV_ERROR_NONE;
}

drvError_t halMemFreeInner(void *pp)
{
    (void)pp;
    return DRV_ERROR_NONE;
}

drvError_t drvMemsetD8Inner(DVdeviceptr dst, size_t destMax, uint8_t value, size_t num)
{
    (void)dst;
    (void)destMax;
    (void)value;
    (void)num;
    return DRV_ERROR_NONE;
}

drvError_t drvMemcpyInner(DVdeviceptr dst, size_t destMax, DVdeviceptr src, size_t byteCount)
{
    (void)dst;
    (void)destMax;
    (void)src;
    (void)byteCount;
    return DRV_ERROR_NONE;
}

drvError_t halMemCpyAsyncInner(DVdeviceptr dst, size_t destMax, DVdeviceptr src, size_t byteCount, uint64_t *copyFd)
{
    (void)dst;
    (void)destMax;
    (void)src;
    (void)byteCount;
    (void)copyFd;
    return DRV_ERROR_NONE;
}

drvError_t halMemcpy2DInner(MEMCPY2D *pCopy)
{
    (void)pCopy;
    return DRV_ERROR_NONE;
}
