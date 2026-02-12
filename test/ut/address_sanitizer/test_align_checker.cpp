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
#include <any>
#include <mutex>
#include <vector>
#include <string>

#define private public
#include "align_checker.h"
#undef private

using namespace std;
using namespace Sanitizer;

SanEvent CreateSanEvent()
{
    uint16_t serialNo = 100;
    uint16_t blockSize = 100;
    Sanitizer::SanEvent event;
    event.serialNo = serialNo;
    event.loc.blockType = Sanitizer::BlockType::AIVEC;
    event.loc.coreId = 1;
    event.loc.fileNo = 1;
    event.loc.pc = 0x0;
    event.type = Sanitizer::EventType::MEM_EVENT;
    auto &memInfo = event.eventInfo.memInfo;
    memInfo.addr = 0x0;
    memInfo.blockNum = 1;
    memInfo.blockSize = 1;
    memInfo.repeatTimes = 1;
    memInfo.memType = Sanitizer::MemType::UB;
    memInfo.blockSize = blockSize;
    return event;
}

class TestAlignChecker : public testing::Test {
public:
    TestAlignChecker() : sanEvent(move(CreateSanEvent())), memOpInfo(sanEvent.eventInfo.memInfo)
    {}
    static void SetUpTestCase()
    {
    }
    void RegisterNotifyFunc(LogLv expectLv, vector<DetectionInfo> &detectInfo)
    {
        auto func = [expectLv, &detectInfo, this](const LogLv &lv, SanitizerBase::MSG_GEN &&gen) {
            if (lv >= expectLv) {
                detectInfo.push_back(gen());
            }
        };
        AlignChecker::Instance().RegisterNotifyFunc(func);
    }
    void SetUp() override
    {
        RegisterNotifyFunc(expectLv, detectInfo);
        detectInfo.clear();
    }

    void TearDown() override
    {
        AlignChecker::Instance().RegisterNotifyFunc(nullptr);
    }

protected:
    vector<DetectionInfo> detectInfo{};
    LogLv expectLv{LogLv::INFO};
    SanEvent sanEvent;
    MemOpInfo &memOpInfo;
};

TEST_F(TestAlignChecker, correct_align_expect_no_error)
{
    constexpr uint16_t expectedAlignSize = 32;
    memOpInfo.addr = 0x2000;
    Sanitizer::RecordType recordType = Sanitizer::RecordType::DMA_MOV;
    Sanitizer::AlignChecker::Instance().CheckAlign(sanEvent, recordType);
    ASSERT_EQ(memOpInfo.alignSize, expectedAlignSize);
    ASSERT_EQ(detectInfo.size(), 0);
   
    memOpInfo.memType = MemType::GM;
    Sanitizer::AlignChecker::Instance().CheckAlign(sanEvent, recordType);
    ASSERT_EQ(memOpInfo.alignSize, 1);
    ASSERT_EQ(detectInfo.size(), 0);
}

TEST_F(TestAlignChecker, misalign_expect_error)
{
    memOpInfo.addr = 0x2001;
    Sanitizer::RecordType recordType = Sanitizer::RecordType::DMA_MOV;
    Sanitizer::AlignChecker::Instance().CheckAlign(sanEvent, recordType);
    Sanitizer::AlignChecker::Instance().Notify();
    ASSERT_EQ(detectInfo.size(), 1);
}

TEST_F(TestAlignChecker, gm_memtype_with_special_align_rule_expect_error)
{
    constexpr uint16_t expectedAlignSize = 32;
    memOpInfo.addr = 0x2001;
    memOpInfo.memType = MemType::GM;
    Sanitizer::RecordType recordType = Sanitizer::RecordType::DECOMPRESS_HEADER;
    Sanitizer::AlignChecker::Instance().CheckAlign(sanEvent, recordType);
    Sanitizer::AlignChecker::Instance().Notify();
    ASSERT_EQ(memOpInfo.alignSize, expectedAlignSize);
    ASSERT_EQ(detectInfo.size(), 1);
}

TEST_F(TestAlignChecker, misalign_expect_notify_msg)
{
    constexpr uint16_t blockSize = 256;
    memOpInfo.addr = 0x2001;
    memOpInfo.blockSize = blockSize;
    memOpInfo.blockNum = 1;
    uint64_t repeatTimes = 2;
    memOpInfo.repeatTimes = repeatTimes;
    Sanitizer::RecordType recordType = Sanitizer::RecordType::DMA_MOV;
    Sanitizer::AlignChecker::Instance().CheckAlign(sanEvent, recordType);
    Sanitizer::AlignChecker::Instance().Notify();
    ASSERT_EQ(detectInfo.size(), 1);
    auto &msgStr = detectInfo[0].message;
    ASSERT_NE(msgStr.find("misaligned"), string::npos);
    ASSERT_NE(msgStr.find("0x2001"), string::npos);
    ASSERT_NE(msgStr.find("UB"), string::npos);
    string expectSizeStr = ToString(memOpInfo.blockSize);
    ASSERT_NE(msgStr.find(expectSizeStr), string::npos);
}

TEST_F(TestAlignChecker, misalign_without_notify_msg_func_expect_no_error)
{
    memOpInfo.addr = 0x2001;
    Sanitizer::RecordType recordType = Sanitizer::RecordType::DMA_MOV;
    Sanitizer::AlignChecker::Instance().UnregisterNotifyFunc();
    Sanitizer::AlignChecker::Instance().CheckAlign(sanEvent, recordType);
    Sanitizer::AlignChecker::Instance().Notify();
    ASSERT_EQ(detectInfo.size(), 0);
}

TEST_F(TestAlignChecker, block_info_equal_0_exext_no_error)
{
    memOpInfo.addr = 0x2001;
    memOpInfo.blockNum = 0;
    Sanitizer::RecordType recordType = Sanitizer::RecordType::UNARY_OP;
    Sanitizer::AlignChecker::Instance().CheckAlign(sanEvent, recordType);
    Sanitizer::AlignChecker::Instance().Notify();
    ASSERT_EQ(memOpInfo.alignSize, 0);
    uint64_t blockNum = 2;
    memOpInfo.blockNum = blockNum;
    memOpInfo.blockSize = 0;
    Sanitizer::AlignChecker::Instance().CheckAlign(sanEvent, recordType);
    uint64_t blockSize = 32;
    memOpInfo.blockSize = blockSize;
    memOpInfo.repeatTimes = 0;
    ASSERT_EQ(memOpInfo.alignSize, 0);
    ASSERT_EQ(detectInfo.size(), 0);
}

TEST_F(TestAlignChecker, test_310p_element_alignment_expect_no_misalignment)
{
    memOpInfo.addr = 0x2001;
    memOpInfo.dataBits = 8;
    memOpInfo.blockNum = 1;
    Sanitizer::RecordType recordType = Sanitizer::RecordType::UNARY_OP;
    Sanitizer::AlignChecker::Instance().SetDeviceType(DeviceType::ASCEND_310P);
    Sanitizer::AlignChecker::Instance().CheckAlign(sanEvent, recordType);
    ASSERT_EQ(Sanitizer::AlignChecker::Instance().errorBuffer_.GetBuffer().size(), 0);
    ASSERT_EQ(memOpInfo.alignSize, 1);

    memOpInfo.addr = 0x2002;
    memOpInfo.dataBits = 16;
    Sanitizer::AlignChecker::Instance().CheckAlign(sanEvent, recordType);
    ASSERT_EQ(Sanitizer::AlignChecker::Instance().errorBuffer_.GetBuffer().size(), 0);
    ASSERT_EQ(memOpInfo.alignSize, 2);

    recordType = Sanitizer::RecordType::REDUCE_OP;
    Sanitizer::AlignChecker::Instance().CheckAlign(sanEvent, recordType);
    ASSERT_EQ(Sanitizer::AlignChecker::Instance().errorBuffer_.GetBuffer().size(), 0);
    ASSERT_EQ(memOpInfo.alignSize, 2);

    recordType = Sanitizer::RecordType::CMPMASK_OP;
    Sanitizer::AlignChecker::Instance().CheckAlign(sanEvent, recordType);
    ASSERT_EQ(Sanitizer::AlignChecker::Instance().errorBuffer_.GetBuffer().size(), 0);
    ASSERT_EQ(memOpInfo.alignSize, 2);
}

TEST_F(TestAlignChecker, test_310p_element_alignment_expect_misalignment)
{
    memOpInfo.addr = 0x2010;
    memOpInfo.dataBits = 8;
    memOpInfo.blockNum = 1;
    Sanitizer::RecordType recordType = Sanitizer::RecordType::DMA_MOV;
    Sanitizer::AlignChecker::Instance().SetDeviceType(DeviceType::ASCEND_310P);
    Sanitizer::AlignChecker::Instance().CheckAlign(sanEvent, recordType);
    ASSERT_EQ(Sanitizer::AlignChecker::Instance().errorBuffer_.GetBuffer().size(), 1);
    ASSERT_EQ(memOpInfo.alignSize, 32);
}