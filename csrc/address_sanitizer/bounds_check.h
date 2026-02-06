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


#ifndef ADDRESS_SANITIZER_BOUNDS_CHECK_H
#define ADDRESS_SANITIZER_BOUNDS_CHECK_H

#include <cstdint>
#include <unordered_map>

#include "core/framework/record_defs.h"
#include "mem_error_def.h"

namespace Sanitizer {

class Bounds {
public:
    struct Range {
        uint64_t addrL;
        uint64_t addrR;
    };

    virtual ErrorMsg Add(uint64_t addr, uint64_t size) = 0;
    virtual ErrorMsg Remove(uint64_t addr, uint64_t size) = 0;
    virtual ErrorMsg Check(uint64_t addr, uint64_t size) const = 0;
};

class DiscreteBounds : public Bounds {
public:
    ErrorMsg Add(uint64_t addr, uint64_t size) override;
    ErrorMsg Remove(uint64_t addr, uint64_t size) override;
    ErrorMsg Check(uint64_t addr, uint64_t size) const override;

#if defined(__BUILD_TESTS__)
    // interface for ut
    std::vector<Range> &GetRanges(void) { return ranges_; }
#endif

private:
    ErrorMsg CheckOverflow(uint64_t addr, uint64_t size) const;
    ErrorMsg CheckAddrOnly(uint64_t addr) const;

private:
    std::vector<Range> ranges_;
};

class UnionBounds : public Bounds {
public:
    UnionBounds(uint64_t addr, uint64_t size) : range_{addr, addr + size} { }
    ErrorMsg Add(uint64_t addr, uint64_t size) override { return {}; }
    ErrorMsg Remove(uint64_t addr, uint64_t size) override { return {}; };
    ErrorMsg Check(uint64_t addr, uint64_t size) const override;

private:
    ErrorMsg CheckAddrOnly(uint64_t addr) const;

private:
    Range range_;
};

class BoundsCheck {
public:
    BoundsCheck(bool localMemoryNeedAlloc = false);
    void Init(ChipInfo const &chipInfo);
    ErrorMsg Add(AddressSpace space, uint64_t addr, uint64_t size);
    ErrorMsg Remove(AddressSpace space, uint64_t addr, uint64_t size);
    ErrorMsg Check(AddressSpace space, uint64_t addr, uint64_t size) const;

private:
    bool localMemoryNeedAlloc_;
    std::unordered_map<AddressSpace, std::unique_ptr<Bounds>> bounds_;
};

} // namespace Sanitizer

#endif // ADDRESS_SANITIZER_BOUNDS_CHECK_H
