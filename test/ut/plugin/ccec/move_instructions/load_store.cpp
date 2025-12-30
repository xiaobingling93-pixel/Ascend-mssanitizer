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


#include <random>
#include <utility>
#include <algorithm>
#include <gtest/gtest.h>

#include "../../ccec_defs.h"
#include "../data_process.h"
#include "plugin/ccec/move_instructions/load_store.cpp"

using namespace Sanitizer;

namespace SanitizerTest {

template<AddressSpace space>
LoadStoreRecord CreateRandomLoadStoreRecord()
{
    LoadStoreRecord record{};
    record.addr = RandInt(0, 0x1000000);
    record.size = RandInt(0, 0x1000000);
    record.location.fileNo = RandInt(0, 0x100000);
    record.location.lineNo = RandInt(0, 0x1000);
    record.space = space;
    record.alignSize = GetUintFromConf<7, 0>(record.size);
    return record;
}

bool CheckRecordEqual(uint8_t const *memInfo, RecordType recordType, LoadStoreRecord &record)
{
    /// 检查 RecordType 是否匹配
    RecordType actualType = *reinterpret_cast<RecordType const *>(memInfo);
    if (actualType != recordType) {
        return false;
    }

    /// 检查 Record 结构体是否一致
    LoadStoreRecord const &actualRecord = *reinterpret_cast<LoadStoreRecord const *>(memInfo + sizeof(RecordType));
    if (record.space == AddressSpace::UB) {
        record.addr = GetUintFromConf<18, 0>(record.addr);
    }
    return IsEqual(actualRecord, record);
}

TEST(MoveInstructions, dump_ub_load_expect_get_correct_record)
{
    auto record = CreateRandomLoadStoreRecord<AddressSpace::UB>();
    std::vector<uint8_t> memInfo = CreateMemInfo();

    __sanitizer_report_load(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                       record.space, record.addr, record.size);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual(ptr, RecordType::LOAD, record));
}

TEST(MoveInstructions, dump_gm_load_expect_get_correct_record)
{
    auto record = CreateRandomLoadStoreRecord<AddressSpace::GM>();
    std::vector<uint8_t> memInfo = CreateMemInfo();

    __sanitizer_report_load(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                       record.space, record.addr, record.size);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual(ptr, RecordType::LOAD, record));
}

TEST(MoveInstructions, dump_ub_store_expect_get_correct_record)
{
    auto record = CreateRandomLoadStoreRecord<AddressSpace::UB>();
    std::vector<uint8_t> memInfo = CreateMemInfo();

    __sanitizer_report_store(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                       record.space, record.addr, record.size);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual(ptr, RecordType::STORE, record));
}

TEST(MoveInstructions, dump_gm_store_expect_get_correct_record)
{
    auto record = CreateRandomLoadStoreRecord<AddressSpace::GM>();
    std::vector<uint8_t> memInfo = CreateMemInfo();

    __sanitizer_report_store(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                       record.space, record.addr, record.size);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual(ptr, RecordType::STORE, record));
}

TEST(MoveInstructions, dump_private_load_store_with_initcheck_expect_get_success)
{
    auto record = CreateRandomLoadStoreRecord<AddressSpace::GM>();
    std::vector<uint8_t> memInfo = CreateMemInfo();
    RecordGlobalHead globalHead{};
    globalHead.checkParms.initcheck = true,
    std::copy_n(reinterpret_cast<uint8_t const*>(&globalHead), sizeof(RecordGlobalHead), memInfo.begin());
    __sanitizer_report_store(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                       AddressSpace::PRIVATE, record.addr, record.size);
    __sanitizer_report_load(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                       AddressSpace::PRIVATE, record.addr, record.size);
    RecordBlockHead blockHead = *reinterpret_cast<RecordBlockHead const *>(memInfo.data() + sizeof(RecordGlobalHead));
    ASSERT_EQ(blockHead.recordWriteCount, 2);
    ASSERT_EQ(blockHead.recordCount, 2);
}

TEST(MoveInstructions, dump_private_load_store_with_no_initcheck_expect_get_none)
{
    auto record = CreateRandomLoadStoreRecord<AddressSpace::GM>();
    std::vector<uint8_t> memInfo = CreateMemInfo();
    __sanitizer_report_store(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                       AddressSpace::PRIVATE, record.addr, record.size);
    __sanitizer_report_load(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                       AddressSpace::PRIVATE, record.addr, record.size);
    RecordBlockHead blockHead = *reinterpret_cast<RecordBlockHead const *>(memInfo.data() + sizeof(RecordGlobalHead));
    ASSERT_EQ(blockHead.recordWriteCount, 0);
    ASSERT_EQ(blockHead.recordCount, 0);
}

TEST(MoveInstructions, dump_private_load_store_with_no_initcheck_expect_get_gm_records)
{
    auto record = CreateRandomLoadStoreRecord<AddressSpace::GM>();
    std::vector<uint8_t> memInfo = CreateMemInfo();
    __sanitizer_report_store(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                       AddressSpace::PRIVATE, record.addr, record.size);
    __sanitizer_report_store(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                       AddressSpace::GM, record.addr, record.size);
    __sanitizer_report_load(memInfo.data(), record.location.fileNo, record.location.lineNo, record.location.pc,
                                       AddressSpace::PRIVATE, record.addr, record.size);
    RecordBlockHead blockHead = *reinterpret_cast<RecordBlockHead const *>(memInfo.data() + sizeof(RecordGlobalHead));
    ASSERT_EQ(blockHead.recordWriteCount, 1);
    ASSERT_EQ(blockHead.recordCount, 1);
}

}
