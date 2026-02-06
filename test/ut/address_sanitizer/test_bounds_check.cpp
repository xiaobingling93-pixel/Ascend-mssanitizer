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


#include <gtest/gtest.h>

#include "address_sanitizer/bounds_check.h"
#include "address_sanitizer/mem_error_def.h"

using namespace Sanitizer;

TEST(BoundsCheck, discrete_bounds_add_range_overflow_uint64_max_expect_return_illegal_write)
{
    uint64_t addr = static_cast<uint64_t>(-1);
    uint64_t size = 100;
    DiscreteBounds bounds;
    ErrorMsg msg = bounds.Add(addr, size);
    ASSERT_TRUE(msg.isError);
    ASSERT_EQ(msg.type, MemErrorType::ILLEGAL_ADDR_WRITE);
    ASSERT_EQ(msg.auxData.badAddr.addr, addr);
    ASSERT_EQ(msg.auxData.nBadBytes, size);
}

TEST(BoundsCheck, discrete_bounds_add_range_fuse_with_sides_expect_return_success)
{
    DiscreteBounds bounds;
    ErrorMsg msg;
    // add first range [0, 100]
    msg = bounds.Add(0, 100);
    ASSERT_FALSE(msg.isError);
    // add range [200, 300], it should insert after [0, 100]
    msg = bounds.Add(200, 100);
    ASSERT_FALSE(msg.isError);
    // add range [150, 200], it should fuse with [200, 300], then got range [150, 300]
    msg = bounds.Add(150, 50);
    ASSERT_FALSE(msg.isError);
    // add range [100, 150], it should fuse with [0, 100] and [150, 300], got range [0, 300]
    msg = bounds.Add(100, 50);
    ASSERT_FALSE(msg.isError);
    // check ranges in bounds
    ASSERT_EQ(bounds.GetRanges().size(), 1UL);
    ASSERT_EQ(bounds.GetRanges()[0].addrL, 0);
    ASSERT_EQ(bounds.GetRanges()[0].addrR, 300);
}

TEST(BoundsCheck, discrete_bounds_remove_range_overflow_uint64_max_expect_return_illegal_write)
{
    uint64_t addr = static_cast<uint64_t>(-1);
    uint64_t size = 100;
    DiscreteBounds bounds;
    ErrorMsg msg = bounds.Remove(addr, size);
    ASSERT_TRUE(msg.isError);
    ASSERT_EQ(msg.type, MemErrorType::ILLEGAL_ADDR_WRITE);
    ASSERT_EQ(msg.auxData.badAddr.addr, addr);
    ASSERT_EQ(msg.auxData.nBadBytes, size);
}

TEST(BoundsCheck, discrete_bounds_remove_range_overflow_all_ranges_expect_return)
{
    DiscreteBounds bounds;
    ErrorMsg msg;
    // add range [0, 100]
    bounds.Add(0, 100);
    // add range [200, 300]
    bounds.Add(200, 100);
    // remove range over all ranges
    msg = bounds.Remove(400, 100);
    ASSERT_FALSE(msg.isError);
}

TEST(BoundsCheck, discrete_bounds_remove_range_out_of_range_expect_return)
{
    DiscreteBounds bounds;
    ErrorMsg msg;
    // add range [200, 300]
    bounds.Add(200, 100);
    // remove range out of left bound
    msg = bounds.Remove(150, 100);
    ASSERT_FALSE(msg.isError);
    // remove range out of right bound
    msg = bounds.Remove(250, 100);
    ASSERT_FALSE(msg.isError);
}

TEST(BoundsCheck, discrete_bounds_remove_range_inside_of_range_expect_return_success)
{
    DiscreteBounds bounds;
    ErrorMsg msg;
    // add range [200, 300]
    bounds.Add(200, 100);

    // remove range [250, 270] wholly inside of [200, 300] will split range into
    // [200, 250] and [270, 300]
    msg = bounds.Remove(250, 20);
    ASSERT_FALSE(msg.isError);
    ASSERT_EQ(bounds.GetRanges().size(), 2UL);
    ASSERT_EQ(bounds.GetRanges()[0].addrL, 200);
    ASSERT_EQ(bounds.GetRanges()[0].addrR, 250);
    ASSERT_EQ(bounds.GetRanges()[1].addrL, 270);
    ASSERT_EQ(bounds.GetRanges()[1].addrR, 300);

    // remove range [200, 220] from [200, 250] will get [220, 250]
    msg = bounds.Remove(200, 20);
    ASSERT_FALSE(msg.isError);
    ASSERT_EQ(bounds.GetRanges().size(), 2UL);
    ASSERT_EQ(bounds.GetRanges()[0].addrL, 220);
    ASSERT_EQ(bounds.GetRanges()[0].addrR, 250);

    // remove range [230, 250] from [220, 250] will get [220, 230]
    msg = bounds.Remove(230, 20);
    ASSERT_FALSE(msg.isError);
    ASSERT_EQ(bounds.GetRanges().size(), 2UL);
    ASSERT_EQ(bounds.GetRanges()[0].addrL, 220);
    ASSERT_EQ(bounds.GetRanges()[0].addrR, 230);

    // remove range [220, 230] will erase it
    msg = bounds.Remove(220, 10);
    ASSERT_FALSE(msg.isError);
    ASSERT_EQ(bounds.GetRanges().size(), 1UL);
}

TEST(BoundsCheck, discrete_bounds_check_range_inside_expect_return_success)
{
    DiscreteBounds bounds;
    ErrorMsg msg;
    // add range [200, 300]
    bounds.Add(200, 100);

    // check range [200, 300]
    msg = bounds.Check(200, 100);
    ASSERT_FALSE(msg.isError);
}

TEST(BoundsCheck, discrete_bounds_check_range_out_of_ranges_expect_return_illegal_access)
{
    DiscreteBounds bounds;
    ErrorMsg msg;

    // check range in empty bounds
    msg = bounds.Check(200, 100);
    ASSERT_TRUE(msg.isError);
    ASSERT_EQ(msg.auxData.badAddr.addr, 200);
    ASSERT_EQ(msg.auxData.nBadBytes, 100);

    // add range [200, 300]
    bounds.Add(200, 100);
    // add range [400, 500]
    bounds.Add(400, 100);

    // check range out of left bound
    msg = bounds.Check(150, 100);
    ASSERT_TRUE(msg.isError);
    ASSERT_EQ(msg.auxData.badAddr.addr, 150);
    ASSERT_EQ(msg.auxData.nBadBytes, 50);

    // check range out of right bound
    msg = bounds.Check(250, 100);
    ASSERT_TRUE(msg.isError);
    ASSERT_EQ(msg.auxData.badAddr.addr, 250);
    ASSERT_EQ(msg.auxData.nBadBytes, 50);

    // check range wholly out of ranges
    msg = bounds.Check(1000, 100);
    ASSERT_TRUE(msg.isError);
    ASSERT_EQ(msg.auxData.badAddr.addr, 1000);
    ASSERT_EQ(msg.auxData.nBadBytes, 100);

    // check range overlap with multiple ranges
    msg = bounds.Check(100, 1000);
    ASSERT_TRUE(msg.isError);
    ASSERT_EQ(msg.auxData.badAddr.addr, 100);
    ASSERT_EQ(msg.auxData.nBadBytes, 800);
}

TEST(BoundsCheck, union_bounds_add_and_remove_range_expect_return_success)
{
    UnionBounds bounds(200, 100);
    ErrorMsg msg;
    msg = bounds.Add(200, 100);
    ASSERT_FALSE(msg.isError);
    msg = bounds.Remove(200, 100);
    ASSERT_FALSE(msg.isError);
}

TEST(BoundsCheck, union_bounds_check_range_inside_expect_return_success)
{
    UnionBounds bounds(200, 100);
    ErrorMsg msg;

    // check range [200, 300]
    msg = bounds.Check(200, 100);
    ASSERT_FALSE(msg.isError);
}

TEST(BoundsCheck, union_bounds_check_range_out_of_ranges_expect_return_illegal_access)
{
    UnionBounds bounds(200, 100);
    ErrorMsg msg;

    // check range out of left bound
    msg = bounds.Check(150, 100);
    ASSERT_TRUE(msg.isError);
    ASSERT_EQ(msg.auxData.badAddr.addr, 150);
    ASSERT_EQ(msg.auxData.nBadBytes, 50);

    // check range out of right bound
    msg = bounds.Check(250, 100);
    ASSERT_TRUE(msg.isError);
    ASSERT_EQ(msg.auxData.badAddr.addr, 250);
    ASSERT_EQ(msg.auxData.nBadBytes, 50);

    // check range wholly out of ranges
    msg = bounds.Check(1000, 100);
    ASSERT_TRUE(msg.isError);
    ASSERT_EQ(msg.auxData.badAddr.addr, 1000);
    ASSERT_EQ(msg.auxData.nBadBytes, 100);
}
