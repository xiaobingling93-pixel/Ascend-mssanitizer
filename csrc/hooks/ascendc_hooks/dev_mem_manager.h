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


#ifndef HOOKS_ASCENDC_HOOKS_DEV_MEM_MANAGER_H
#define HOOKS_ASCENDC_HOOKS_DEV_MEM_MANAGER_H

#include "runtime.h"

namespace Sanitizer {
class DevMemManager {
public:
    static DevMemManager &GetInstance()
    {
        static DevMemManager instance;
        return instance;
    }
    ~DevMemManager()
    {
        Free();
    }

    rtError_t MallocMemory(void*& memPtr, uint64_t size)
    {
        if (memPtr_ != nullptr) {
            if (memSize_ < size) {
                Free();
            } else {
                memPtr = memPtr_;
                return RT_ERROR_NONE;
            }
        }

        rtError_t error = rtMalloc(&memPtr_, size, 0x00, -1);
        if (error != RT_ERROR_NONE) {
            return error;
        }
        // 记录内存信息的GM，在桩函数内部的写入，该写入操作无法传回Host侧进行记录，因为将其设为Store状态，防止未初始化读误报
        HookReport::Instance().ReportStore(reinterpret_cast<uint64_t>(memPtr_), size, MemInfoSrc::RT);
        memPtr = memPtr_;
        memSize_ = size;
        return RT_ERROR_NONE;
    }

    void Free()
    {
        // rtFree不会将指针置成空指针，需要自行重设
        if (memPtr_ != nullptr) {
            rtFree(memPtr_);
            memPtr_ = nullptr;
        }
        memSize_ = 0;
        SetMemoryInitFlag(false);
    }

    void SetMemoryInitFlag(bool flag)
    {
        isMemoryInit_ = flag;
    }

    // <<<>>>的运行方式在检测工具插桩首尾执行__sanitizer_init和__sanitizer_finalize的基础上还会包一层
    // 即 init1 -> init2 -> .... -> finalize2 -> finalize1
    // 在该场景下进行init申请的内存复用，会导致连续两次上报record记录，故使用该标志位识别
    bool IsMemoryInit() const
    {
        return isMemoryInit_;
    }

private:
    DevMemManager() = default;
    DevMemManager(const DevMemManager&) = delete;
    DevMemManager& operator=(const DevMemManager&) = delete;
    uint64_t memSize_ = 0;
    void *memPtr_ = nullptr;
    bool isMemoryInit_ = false;
};
}
#endif
