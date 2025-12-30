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


#ifndef __HOOK_HANDLE_MAPPING_H__
#define __HOOK_HANDLE_MAPPING_H__

namespace Sanitizer {

struct BinKernel {
    std::vector<char> bin;
    KernelType kernelType;
};

// 使用单例而不是全局变量维护两个映射表，避免segmentation fault
class HandleMapping {
public:
    static HandleMapping &GetInstance()
    {
        static HandleMapping instance;
        return instance;
    }

    ~HandleMapping()
    {
        /// 预期在UnRegister函数中，通过erase删去映射关系
        /// 某些情况下，单例在UnRegister函数调用前被析构，析构后仍然可以访问，属于非法访问内存
        /// 为了避免非法访问时erase造成segmentation fault，在析构时clear，保证析构后无法访问到handle
        handleBinKernelMap_.clear();
        stubHandleMap_.clear();
    }

    /* 两个映射表：
    * 1.HandleBinKernelMap 用于绑定算子 handle 与 kernel 二进制以及 KernelType 的映射关系
    * 2.StubHandleMap 用于绑定算子 stubFunc 与 handle 的映射关系
    */
    using HandleBinKernelMapType = std::map<const void*, BinKernel>;
    using StubHandleMapType = std::map<const void*, const void*>;

    HandleBinKernelMapType handleBinKernelMap_;
    StubHandleMapType stubHandleMap_;

private:
    HandleMapping() = default;
    HandleMapping(const HandleMapping&) = delete;
    HandleMapping& operator=(const HandleMapping&) = delete;
};

}  // namespace Sanitizer
#endif
