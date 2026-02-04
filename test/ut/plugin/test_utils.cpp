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
#include "plugin/utils.h"
#include "ccec/data_process.h"
#include <cstdint>

using namespace std;

TEST(Utils, test_get_uint_from_conf_expect_success)
{
    uint64_t config{0};
    config = 0xea00;
    ASSERT_EQ((GetUintFromConf<15, 8>(config)), 0xea);

    config = 0xe00a0;
    ASSERT_EQ((GetUintFromConf<19, 4>(config)), 0xe00a);

    config = 0xe0104050a0;
    ASSERT_EQ((GetUintFromConf<32, 8>(config)), 0x104050);

    config = 0xfbca8ae0104050a0;
    ASSERT_EQ((GetUintFromConf<63, 8>(config)), 0xfbca8ae0104050);

    config = 0xfbca8ae010412345;
    ASSERT_EQ((GetUintFromConf<23, 0>(config)), 0x412345);

    // 1010 0100 0101
    config = 0xa45;
    ASSERT_EQ((GetUintFromConf<10, 2>(config)), 0b10010001);

    for (size_t i = 0 ;i < 100; ++i) {
        uint64_t config1 =
            (static_cast<uint64_t>(Sanitizer::RandInt(0, INT32_MAX)) << 32) + static_cast<uint64_t>(Sanitizer::RandInt(0, INT32_MAX));
        ASSERT_EQ((GetUintFromConf<13, 7>(config1)), config1 >> 7 & 0x7f);
        ASSERT_EQ((GetUintFromConf<35, 21>(config1)), config1 >> 21 & 0x7fff);
        ASSERT_EQ((GetUintFromConf<63, 2>(config1)), config1 >> 2 & (~0x0ULL >> 2));
    }

    ASSERT_EQ(GetUintFromConf<2>(config), 1);
    ASSERT_EQ(GetUintFromConf<3>(config), 0);
    ASSERT_EQ(GetUintFromConf<4>(config), 0);
    ASSERT_EQ(GetUintFromConf<6>(config), 1);
}

TEST(Utils, test_set_conf_by_unit_expect_success)
{
    uint64_t config{0};
    uint16_t val = 0x1234;
    SetConfByUint<23, 8>(config, val);
    ASSERT_EQ(config, 0x123400);

    uint64_t val64{0};
    SetConfByUint<24, 8>(config, val64 - 1);
    ASSERT_EQ(config, 0x1ffff00);

    val64 = 0;
    SetConfByUint<7, 0>(config, 0xef);
    SetConfByUint<23, 8>(config, 0xabcd);
    SetConfByUint<63, 24>(config, 0x0123456789);
    ASSERT_EQ(config, 0x123456789abcdef);
}