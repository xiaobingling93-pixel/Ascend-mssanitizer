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
#include "sanitizer_base.h"
#include "record_pre_process.h"
#define private public
#include "sync_sanitizer.h"

namespace {
using namespace Sanitizer;

auto g_fillSyncRecord = [](SanitizerRecord& record, uint16_t coreId = 0U, RecordType type = RecordType::SET_FLAG,
    PipeType srcPipe = PipeType::PIPE_V, PipeType dstPipe = PipeType::PIPE_MTE1, EventID eventId = EventID::EVENT_ID0) {
    record.version = RecordVersion::KERNEL_RECORD;
    record.payload.kernelRecord.recordType = type;
    auto& syncRecord = record.payload.kernelRecord.payload.syncRecord;
    syncRecord.location.blockId = coreId;
    syncRecord.src = srcPipe;
    syncRecord.dst = dstPipe;
    syncRecord.eventID = static_cast<uint64_t>(eventId);
};

TEST(SyncSanitizer, unpaired_set_flag_instruction_expect_return_synccheck_detection)
{
    Config config {};
    config.syncCheck = true;
    DeviceInfoSummary deviceInfoSummary {};
    deviceInfoSummary.device = DeviceType::ASCEND_910B1;
    auto syncSan = SanitizerFactory::GetInstance().Create(ToolType::SYNCCHECK);
    ASSERT_FALSE(syncSan->SetDeviceInfo(deviceInfoSummary, config));

    SanitizerRecord record {};
    g_fillSyncRecord(record);
    std::vector<SanEvent> events {};
    RecordPreProcess::GetInstance().Process(record, events);
    std::string msg {};
    syncSan->RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });
    ASSERT_TRUE(syncSan->CheckRecordBeforeProcess(record));

    SanEvent event {};
    event.isEndFrame = true;
    events.emplace_back(event);
    syncSan->Do(record, events);
    ASSERT_TRUE(msg.find("Unpaired set_flag instructions detected") != std::string::npos);
}

TEST(SyncSanitizer, unpaired_set_flag_instructions_on_different_blocks_expect_all_blocks)
{
    SanitizerRecord record {};
    std::vector<SanEvent> events {};
    g_fillSyncRecord(record, 0U);
    RecordPreProcess::GetInstance().Process(record, events);
    g_fillSyncRecord(record, 1U);
    RecordPreProcess::GetInstance().Process(record, events);
    g_fillSyncRecord(record, 2U);
    RecordPreProcess::GetInstance().Process(record, events);
    ASSERT_EQ(events.size(), 3U);

    SyncSanitizer syncSan {};
    std::string msg {};
    syncSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });
    SanEvent event {};
    event.isEndFrame = true;
    events.emplace_back(event);
    syncSan.Do(record, events);
    ASSERT_EQ(syncSan.syncEvents_.size(), 3U);
    ASSERT_TRUE(msg.find("in block aiv(0) on device 0") != std::string::npos);
    ASSERT_TRUE(msg.find("in block aiv(1) on device 0") != std::string::npos);
    ASSERT_TRUE(msg.find("in block aiv(2) on device 0") != std::string::npos);
}

TEST(SyncSanitizer, unpaired_set_flag_instructions_on_different_blocks_expect_specified_block)
{
    SanitizerRecord record {};
    std::vector<SanEvent> events {};
    g_fillSyncRecord(record, 0U);
    RecordPreProcess::GetInstance().Process(record, events);
    g_fillSyncRecord(record, 1U);
    RecordPreProcess::GetInstance().Process(record, events);
    g_fillSyncRecord(record, 2U);
    RecordPreProcess::GetInstance().Process(record, events);
    ASSERT_EQ(events.size(), 3U);

    SyncSanitizer syncSan {};
    syncSan.checkBlockId_ = 1U;
    std::string msg {};
    syncSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });
    SanEvent event {};
    event.isEndFrame = true;
    events.emplace_back(event);
    syncSan.Do(record, events);
    ASSERT_EQ(syncSan.syncEvents_.size(), 1U);
    ASSERT_TRUE(msg.find("in block aiv(0) on device 0") == std::string::npos);
    ASSERT_TRUE(msg.find("in block aiv(1) on device 0") != std::string::npos);
    ASSERT_TRUE(msg.find("in block aiv(2) on device 0") == std::string::npos);
}

TEST(SyncSanitizer, multi_instructions_with_no_paired_flag)
{
    SanitizerRecord record {};
    std::vector<SanEvent> events {};
    g_fillSyncRecord(record, 0U, RecordType::SET_FLAG, PipeType::PIPE_V, PipeType::PIPE_MTE1, EventID::EVENT_ID0);
    RecordPreProcess::GetInstance().Process(record, events);
    g_fillSyncRecord(record, 1U, RecordType::WAIT_FLAG, PipeType::PIPE_V, PipeType::PIPE_MTE1, EventID::EVENT_ID0);
    RecordPreProcess::GetInstance().Process(record, events);
    g_fillSyncRecord(record, 0U, RecordType::WAIT_FLAG, PipeType::PIPE_MTE1, PipeType::PIPE_V, EventID::EVENT_ID0);
    RecordPreProcess::GetInstance().Process(record, events);
    g_fillSyncRecord(record, 0U, RecordType::WAIT_FLAG, PipeType::PIPE_V, PipeType::PIPE_MTE1, EventID::EVENT_ID1);
    RecordPreProcess::GetInstance().Process(record, events);

    SyncSanitizer syncSan {};
    std::string msg {};
    syncSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });
    SanEvent event {};
    event.isEndFrame = true;
    events.emplace_back(event);
    syncSan.Do(record, events);
    ASSERT_EQ(syncSan.syncEvents_.size(), 4U);
    ASSERT_TRUE(msg.find("from PIPE_V to PIPE_MTE1") != std::string::npos);
    ASSERT_TRUE(msg.find("in block aiv(0) on device 0") != std::string::npos);
}

TEST(SyncSanitizer, multi_instructions_with_one_paired_flag)
{
    SanitizerRecord record {};
    std::vector<SanEvent> events {};
    g_fillSyncRecord(record, 0U, RecordType::SET_FLAG, PipeType::PIPE_V, PipeType::PIPE_MTE1, EventID::EVENT_ID0);
    RecordPreProcess::GetInstance().Process(record, events);
    g_fillSyncRecord(record, 1U, RecordType::WAIT_FLAG, PipeType::PIPE_V, PipeType::PIPE_MTE1, EventID::EVENT_ID0);
    RecordPreProcess::GetInstance().Process(record, events);
    g_fillSyncRecord(record, 0U, RecordType::WAIT_FLAG, PipeType::PIPE_MTE1, PipeType::PIPE_V, EventID::EVENT_ID0);
    RecordPreProcess::GetInstance().Process(record, events);
    g_fillSyncRecord(record, 0U, RecordType::WAIT_FLAG, PipeType::PIPE_V, PipeType::PIPE_MTE1, EventID::EVENT_ID0);
    RecordPreProcess::GetInstance().Process(record, events);

    SyncSanitizer syncSan {};
    std::string msg {};
    syncSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });
    SanEvent event {};
    event.isEndFrame = true;
    events.emplace_back(event);
    syncSan.Do(record, events);
    ASSERT_EQ(syncSan.syncEvents_.size(), 2U);
    ASSERT_TRUE(msg.find("Unpaired set_flag instructions detected") == std::string::npos);
}

}
