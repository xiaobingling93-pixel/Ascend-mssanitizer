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


#include <utility>
#include <gtest/gtest.h>

#include "../data_process.h"
#include "../../ccec_defs.h"
#include "plugin/ccec/move_instructions/set_2d.cpp"

using namespace Sanitizer;

template<MemType dstMemType>
Set2DRecord CreateRandomSet2DRecord()
{
    Set2DRecord record{};
    record.dst = RandInt(0, 0x1000000);
    record.location.fileNo = RandInt(0, 0x100000);
    record.location.lineNo = RandInt(0, 0x1000);
    record.repeat = RandInt(1, 0xF);
    record.dstBlockNum = RandInt(1, 0xF);
    record.repeatGap = RandInt(1, 0xF);
    record.dstMemType = dstMemType;
    return record;
}

int64_t ExtractConfigFromSet2DRecord(const Set2DRecord &record)
{
    constexpr uint64_t mask = 0x7FFF;

    constexpr uint64_t dstBlockNumShift = 16;
    constexpr uint64_t repeatGapShift = 32;

    return (record.repeat & mask) |
           ((record.dstBlockNum & mask) << dstBlockNumShift) |
           ((record.repeatGap & mask) << repeatGapShift);
}

TEST(Set2DInstructions, set_l0a_2d)
{
    auto record = CreateRandomSet2DRecord<MemType::L0A>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    auto config = ExtractConfigFromSet2DRecord(record);

    // #2138
    __sanitizer_report_set_l0a_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                  reinterpret_cast<__ca__ __bf16*>(record.dst), config);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #2139
    __sanitizer_report_set_l0a_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                  reinterpret_cast<__ca__ half*>(record.dst), config);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #2140
    __sanitizer_report_set_l0a_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                  reinterpret_cast<__ca__ float*>(record.dst), config);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #2141
    __sanitizer_report_set_l0a_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                  reinterpret_cast<__ca__ int16_t*>(record.dst), config);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #2142
    __sanitizer_report_set_l0a_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                  reinterpret_cast<__ca__ int32_t*>(record.dst), config);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #2143
    __sanitizer_report_set_l0a_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                  reinterpret_cast<__ca__ uint16_t*>(record.dst), config);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #2145
    __sanitizer_report_set_l0a_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                  reinterpret_cast<__ca__ uint32_t*>(record.dst), config);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));
}

TEST(Set2DInstructions, set_l0b_2d)
{
    auto record = CreateRandomSet2DRecord<MemType::L0B>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    auto config = ExtractConfigFromSet2DRecord(record);

    // #2148
    __sanitizer_report_set_l0b_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                  reinterpret_cast<__cb__ __bf16*>(record.dst), config);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #2149
    __sanitizer_report_set_l0b_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                  reinterpret_cast<__cb__ half*>(record.dst), config);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #2150
    __sanitizer_report_set_l0b_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                  reinterpret_cast<__cb__ float*>(record.dst), config);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #2151
    __sanitizer_report_set_l0b_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                  reinterpret_cast<__cb__ int16_t*>(record.dst), config);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #2152
    __sanitizer_report_set_l0b_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                  reinterpret_cast<__cb__ int32_t*>(record.dst), config);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #2153
    __sanitizer_report_set_l0b_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                  reinterpret_cast<__cb__ uint16_t*>(record.dst), config);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #2155
    __sanitizer_report_set_l0b_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                  reinterpret_cast<__cb__ uint32_t*>(record.dst), config);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));
}

TEST(Set2DInstructions, set_l1_2d)
{
    auto record = CreateRandomSet2DRecord<MemType::L1>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    auto config = ExtractConfigFromSet2DRecord(record);

    // #2158
    __sanitizer_report_set_l1_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                  reinterpret_cast<__cbuf__ __bf16*>(record.dst), config);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #2159
    __sanitizer_report_set_l1_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                 reinterpret_cast<__cbuf__ half*>(record.dst), config);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #2160
    __sanitizer_report_set_l1_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                 reinterpret_cast<__cbuf__ float*>(record.dst), config);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #2161
    __sanitizer_report_set_l1_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                 reinterpret_cast<__cbuf__ int16_t*>(record.dst), config);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #2162
    __sanitizer_report_set_l1_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                 reinterpret_cast<__cbuf__ int32_t*>(record.dst), config);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #2163
    __sanitizer_report_set_l1_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                 reinterpret_cast<__cbuf__ uint16_t*>(record.dst), config);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #2165
    __sanitizer_report_set_l1_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                 reinterpret_cast<__cbuf__ uint32_t*>(record.dst), config);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #2165
    __sanitizer_report_set_l1_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                 reinterpret_cast<__cbuf__ uint32_t*>(record.dst), config);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));
}

TEST(Set2DInstructions, create_ca_matrix_0)
{
    auto record = CreateRandomSet2DRecord<MemType::L0A>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    auto config = ExtractConfigFromSet2DRecord(record);

    // #574
    __sanitizer_report_create_ca_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__ca__ __bf16*>(record.dst), config, Half{});
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #575
    __sanitizer_report_create_ca_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__ca__ __bf16*>(record.dst), config, Bf16{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #576
    __sanitizer_report_create_ca_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__ca__ __bf16*>(record.dst), config, 1U);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #577
    __sanitizer_report_create_ca_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__ca__ half*>(record.dst), config, Half{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #578
    __sanitizer_report_create_ca_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__ca__ int16_t*>(record.dst), config, Half{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #579
    __sanitizer_report_create_ca_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__ca__ uint16_t*>(record.dst), config, Half{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #580
    __sanitizer_report_create_ca_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__ca__ uint16_t*>(record.dst), config, Half{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));
}

TEST(Set2DInstructions, create_ca_matrix_1)
{
    auto record = CreateRandomSet2DRecord<MemType::L0A>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    auto config = ExtractConfigFromSet2DRecord(record);

    // #581
    __sanitizer_report_create_ca_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__ca__ float*>(record.dst), config, 1U);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #582
    __sanitizer_report_create_ca_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__ca__ float*>(record.dst), config, Half{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #583
    __sanitizer_report_create_ca_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__ca__ int16_t*>(record.dst), config, 1U);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #584
    __sanitizer_report_create_ca_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__ca__ int32_t*>(record.dst), config, 1U);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #585
    __sanitizer_report_create_ca_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__ca__ int32_t*>(record.dst), config, Half{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #586
    __sanitizer_report_create_ca_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__ca__ uint16_t*>(record.dst), config, 1U);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #587
    __sanitizer_report_create_ca_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__ca__ uint32_t*>(record.dst), config, 1U);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #588
    __sanitizer_report_create_ca_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__ca__ uint32_t*>(record.dst), config, Half{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));
}

TEST(Set2DInstructions, create_cb_matrix_0)
{
    auto record = CreateRandomSet2DRecord<MemType::L0B>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    auto config = ExtractConfigFromSet2DRecord(record);

    // #594
    __sanitizer_report_create_cb_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__cb__ __bf16*>(record.dst), config, Half{});
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #595
    __sanitizer_report_create_cb_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__cb__ __bf16*>(record.dst), config, Bf16{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #596
    __sanitizer_report_create_cb_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__cb__ __bf16*>(record.dst), config, 1U);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #597
    __sanitizer_report_create_cb_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__cb__ half*>(record.dst), config, Half{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #598
    __sanitizer_report_create_cb_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__cb__ int16_t*>(record.dst), config, Half{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #599
    __sanitizer_report_create_cb_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__cb__ uint16_t*>(record.dst), config, Half{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #600
    __sanitizer_report_create_cb_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__cb__ uint16_t*>(record.dst), config, Half{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));
}

TEST(Set2DInstructions, create_cb_matrix_1)
{
    auto record = CreateRandomSet2DRecord<MemType::L0B>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    auto config = ExtractConfigFromSet2DRecord(record);

    // #601
    __sanitizer_report_create_cb_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__cb__ float*>(record.dst), config, 1U);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #602
    __sanitizer_report_create_cb_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__cb__ float*>(record.dst), config, Half{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #603
    __sanitizer_report_create_cb_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__cb__ int16_t*>(record.dst), config, 1U);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #604
    __sanitizer_report_create_cb_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__cb__ int32_t*>(record.dst), config, 1U);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #605
    __sanitizer_report_create_cb_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__cb__ int32_t*>(record.dst), config, Half{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #606
    __sanitizer_report_create_cb_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__cb__ uint16_t*>(record.dst), config, 1U);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #607
    __sanitizer_report_create_cb_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__cb__ uint32_t*>(record.dst), config, 1U);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #608
    __sanitizer_report_create_cb_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__cb__ uint32_t*>(record.dst), config, Half{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));
}

TEST(Set2DInstructions, create_cbuf_matrix_0)
{
    auto record = CreateRandomSet2DRecord<MemType::L1>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    auto config = ExtractConfigFromSet2DRecord(record);

    // #614
    __sanitizer_report_create_cbuf_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__cbuf__ __bf16*>(record.dst), config, Half{});
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #615
    __sanitizer_report_create_cbuf_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__cbuf__ __bf16*>(record.dst), config, Bf16{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #616
    __sanitizer_report_create_cbuf_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__cbuf__ __bf16*>(record.dst), config, 1U);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #617
    __sanitizer_report_create_cbuf_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__cbuf__ half*>(record.dst), config, Half{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #618
    __sanitizer_report_create_cbuf_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__cbuf__ int16_t*>(record.dst), config, Half{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #619
    __sanitizer_report_create_cbuf_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__cbuf__ uint16_t*>(record.dst), config, Half{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #620
    __sanitizer_report_create_cbuf_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                        record.location.pc,
                                        reinterpret_cast<__cbuf__ uint16_t*>(record.dst), config, Half{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));
}

TEST(Set2DInstructions, create_cbuf_matrix_1)
{
    auto record = CreateRandomSet2DRecord<MemType::L1>();

    std::vector<uint8_t> memInfo = CreateMemInfo();

    auto config = ExtractConfigFromSet2DRecord(record);

    // #621
    __sanitizer_report_create_cbuf_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                          record.location.pc,
                                          reinterpret_cast<__cbuf__ float*>(record.dst), config, 1U);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #622
    __sanitizer_report_create_cbuf_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                          record.location.pc,
                                          reinterpret_cast<__cbuf__ float*>(record.dst), config, Half{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #623
    __sanitizer_report_create_cbuf_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                          record.location.pc,
                                          reinterpret_cast<__cbuf__ int16_t*>(record.dst), config, 1U);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #624
    __sanitizer_report_create_cbuf_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                          record.location.pc,
                                          reinterpret_cast<__cbuf__ int32_t*>(record.dst), config, 1U);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #625
    __sanitizer_report_create_cbuf_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                          record.location.pc,
                                          reinterpret_cast<__cbuf__ int32_t*>(record.dst), config, Half{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #626
    __sanitizer_report_create_cbuf_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                          record.location.pc,
                                          reinterpret_cast<__cbuf__ uint16_t*>(record.dst), config, 1U);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #627
    __sanitizer_report_create_cbuf_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                          record.location.pc,
                                          reinterpret_cast<__cbuf__ uint32_t*>(record.dst), config, 1U);
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));

    // #628
    __sanitizer_report_create_cbuf_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                          record.location.pc,
                                          reinterpret_cast<__cbuf__ uint32_t*>(record.dst), config, Half{});
    ptr += sizeof(RecordType) + sizeof(Set2DRecord);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_2D>(ptr, record));
}

TEST(Set2DInstructions, test_m200_instructions_expect_success)
{
    auto record = CreateRandomSet2DRecord<MemType::L1>();
    std::vector<uint8_t> memInfo = CreateMemInfo();
    int64_t config = 12;

    // #613
    __sanitizer_report_create_cbuf_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ void*>(record.dst), config, Half{});
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead) + sizeof(RecordType);
    auto actualRecord = *reinterpret_cast<Set2DRecord const *>(ptr);
    ASSERT_EQ(actualRecord.repeat, 1);
    ASSERT_EQ(actualRecord.dstBlockNum, 1);
    ASSERT_EQ(actualRecord.dstMemType, MemType::L1);
    ASSERT_EQ(actualRecord.dstBlockSize, config * 512);

    /// #2137
    record.dstMemType = MemType::L0A;
    __sanitizer_report_set_l0a_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                  reinterpret_cast<__ca__ void*>(record.dst), config);
    ptr += sizeof(Set2DRecord) + sizeof(RecordType);
    actualRecord = *reinterpret_cast<Set2DRecord const *>(ptr);
    ASSERT_EQ(actualRecord.repeat, 1);
    ASSERT_EQ(actualRecord.dstBlockNum, 1);
    ASSERT_EQ(actualRecord.dstMemType, MemType::L0A);
    ASSERT_EQ(actualRecord.dstBlockSize, config * 512);

    /// #2157
    record.dstMemType = MemType::L1;
    __sanitizer_report_set_l1_2d(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                  reinterpret_cast<__ca__ void*>(record.dst), config);
    ptr += sizeof(Set2DRecord) + sizeof(RecordType);
    actualRecord = *reinterpret_cast<Set2DRecord const *>(ptr);
    ASSERT_EQ(actualRecord.repeat, 1);
    ASSERT_EQ(actualRecord.dstBlockNum, 1);
    ASSERT_EQ(actualRecord.dstMemType, MemType::L1);
    ASSERT_EQ(actualRecord.dstBlockSize, config * 512);

    // #573
    record.dstMemType = MemType::L0A;
    __sanitizer_report_create_ca_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ void*>(record.dst), config, Half{});
    ptr += sizeof(Set2DRecord) + sizeof(RecordType);
    actualRecord = *reinterpret_cast<Set2DRecord const *>(ptr);
    ASSERT_EQ(actualRecord.repeat, 1);
    ASSERT_EQ(actualRecord.dstBlockNum, 1);
    ASSERT_EQ(actualRecord.dstMemType, MemType::L0A);
    ASSERT_EQ(actualRecord.dstBlockSize, config * 512);

    // #593
    record.dstMemType = MemType::L0B;
    __sanitizer_report_create_cb_matrix(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, reinterpret_cast<__cbuf__ void*>(record.dst), config, Half{});
    ptr += sizeof(Set2DRecord) + sizeof(RecordType);
    actualRecord = *reinterpret_cast<Set2DRecord const *>(ptr);
    ASSERT_EQ(actualRecord.repeat, 1);
    ASSERT_EQ(actualRecord.dstBlockNum, 1);
    ASSERT_EQ(actualRecord.dstMemType, MemType::L0B);
    ASSERT_EQ(actualRecord.dstBlockSize, config * 512);
}
