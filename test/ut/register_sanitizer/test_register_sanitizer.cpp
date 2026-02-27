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
#include "sanitizer_base.h"
#include "record_pre_process.h"
#define private public
#include "register_sanitizer.h"
#undef private

namespace {
using namespace Sanitizer;

void UT_FillRegRecord(SanitizerRecord& record, RecordType type, uint16_t coreId, RegisterValueType regValType, uint64_t regVal)
{
    record.version = RecordVersion::KERNEL_RECORD;
    record.payload.kernelRecord.recordType = type;
    
    auto& regRecord = record.payload.kernelRecord.payload.registerSetRecord;
    regRecord.location.blockId = coreId;
    regRecord.regPayLoad.regValType = regValType;
    regRecord.regPayLoad.regVal = regVal;
}

TEST(RegisterSanitizer, set_vector_mask_0_expect_report_exception)
{
    Config config {};
    config.registerCheck = true;
    DeviceInfoSummary deviceInfoSummary {};
    deviceInfoSummary.device = DeviceType::ASCEND_910B1;
    auto regSanitizer = SanitizerFactory::GetInstance().Create(ToolType::REGISTERCHECK);
    ASSERT_FALSE(regSanitizer->SetDeviceInfo(deviceInfoSummary, config));

    RegisterSanitizer regSan {};
    regSan.Init();
    std::string msg {};
    regSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });

    std::vector<SanEvent> events {};
    SanEvent endEvent {};
    endEvent.isEndFrame = true;
    SanitizerRecord record {};
    uint64_t regvalU64 = 123456;
    RegisterType regType = RegisterType::VECTOR_MASK_0;
    size_t regIdx = static_cast<size_t>(regType);
    uint64_t regvalU64Dft = Sanitizer::g_regInfoTbl[regIdx].regDftVal;
    RecordType recordType = RecordType::SET_VECTOR_MASK_0;
    uint16_t coreId = 0;
    
    // 未重置为默认值报告警
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64);
    RecordPreProcess::GetInstance().Process(record, events);
    ASSERT_EQ(events.size(), 1U);
    events.emplace_back(endEvent);
    regSan.Do(record, events);

    std::cout << "[UT_DEBUG]msg: \n" << msg << std::endl;
    ASSERT_EQ(regSan.regValActual_[coreId][regIdx], regvalU64);
    ASSERT_TRUE(msg.find("Register " + Sanitizer::g_regInfoTbl[regIdx].regNameStr + " was not reset to default in block aiv("
        + std::to_string(coreId) + ")") != std::string::npos);
    ASSERT_TRUE(msg.find("current value is (" + std::to_string(regvalU64) + ")") != std::string::npos);
}

TEST(RegisterSanitizer, set_vector_mask_0_expect_not_report_exception)
{
    RegisterSanitizer regSan {};
    regSan.Init();
    std::string msg {};
    regSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });

    std::vector<SanEvent> events {};
    SanEvent endEvent {};
    endEvent.isEndFrame = true;
    SanitizerRecord record {};
    uint64_t regvalU64 = 123456;
    RegisterType regType = RegisterType::VECTOR_MASK_0;
    size_t regIdx = static_cast<size_t>(regType);
    uint64_t regvalU64Dft = Sanitizer::g_regInfoTbl[regIdx].regDftVal;
    RecordType recordType = RecordType::SET_VECTOR_MASK_0;
    uint16_t coreId = 0;

    // 重置为默认值不报告警
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64);
    RecordPreProcess::GetInstance().Process(record, events);
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64Dft);
    RecordPreProcess::GetInstance().Process(record, events);
    ASSERT_EQ(events.size(), 2U);
    events.emplace_back(endEvent);
    regSan.Do(record, events);

    std::cout << "[UT_DEBUG]msg: \n" << msg << std::endl;
    ASSERT_EQ(regSan.regValActual_[coreId][regIdx], regvalU64Dft);
    ASSERT_TRUE(msg.find("Register " + Sanitizer::g_regInfoTbl[regIdx].regNameStr + " was not reset to default in block aiv("
        + std::to_string(coreId) + ")") == std::string::npos);
    ASSERT_TRUE(msg.find("current valure is (" + std::to_string(regvalU64) + ")") == std::string::npos);
}

TEST(RegisterSanitizer, set_vector_mask_1_expect_report_exception)
{
    RegisterSanitizer regSan {};
    regSan.Init();
    std::string msg {};
    regSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });

    std::vector<SanEvent> events {};
    SanEvent endEvent {};
    endEvent.isEndFrame = true;
    SanitizerRecord record {};
    uint64_t regvalU64 = 123457;
    RegisterType regType = RegisterType::VECTOR_MASK_1;
    size_t regIdx = static_cast<size_t>(regType);
    uint64_t regvalU64Dft = Sanitizer::g_regInfoTbl[regIdx].regDftVal;
    RecordType recordType = RecordType::SET_VECTOR_MASK_1;
    uint16_t coreId = 1;
    
    // 未重置为默认值报告警
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64);
    RecordPreProcess::GetInstance().Process(record, events);
    ASSERT_EQ(events.size(), 1U);
    events.emplace_back(endEvent);
    regSan.Do(record, events);

    std::cout << "[UT_DEBUG]msg: \n" << msg << std::endl;
    ASSERT_EQ(regSan.regValActual_[coreId][regIdx], regvalU64);
    ASSERT_TRUE(msg.find("Register " + Sanitizer::g_regInfoTbl[regIdx].regNameStr + " was not reset to default in block aiv("
        + std::to_string(coreId) + ")") != std::string::npos);
    ASSERT_TRUE(msg.find("current value is (" + std::to_string(regvalU64) + ")") != std::string::npos);

}

TEST(RegisterSanitizer, set_vector_mask_1_expect_not_report_exception)
{
    RegisterSanitizer regSan {};
    regSan.Init();
    std::string msg {};
    regSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });

    std::vector<SanEvent> events {};
    SanEvent endEvent {};
    endEvent.isEndFrame = true;
    SanitizerRecord record {};
    uint64_t regvalU64 = 123457;
    RegisterType regType = RegisterType::VECTOR_MASK_1;
    size_t regIdx = static_cast<size_t>(regType);
    uint64_t regvalU64Dft = Sanitizer::g_regInfoTbl[regIdx].regDftVal;
    RecordType recordType = RecordType::SET_VECTOR_MASK_1;
    uint16_t coreId = 1;

    // 重置为默认值不报告警
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64);
    RecordPreProcess::GetInstance().Process(record, events);
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64Dft);
    RecordPreProcess::GetInstance().Process(record, events);
    ASSERT_EQ(events.size(), 2U);
    events.emplace_back(endEvent);
    regSan.Do(record, events);

    std::cout << "[UT_DEBUG]msg: \n" << msg << std::endl;
    ASSERT_EQ(regSan.regValActual_[coreId][regIdx], regvalU64Dft);
    ASSERT_TRUE(msg.find("Register " + Sanitizer::g_regInfoTbl[regIdx].regNameStr + " was not reset to default in block aiv("
        + std::to_string(coreId) + ")") == std::string::npos);
    ASSERT_TRUE(msg.find("current valure is (" + std::to_string(regvalU64) + ")") == std::string::npos);
}

TEST(RegisterSanitizer, set_ctrl_expect_report_exception)
{
    RegisterSanitizer regSan {};
    regSan.Init();
    std::string msg {};
    regSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });

    std::vector<SanEvent> events {};
    SanEvent endEvent {};
    endEvent.isEndFrame = true;
    SanitizerRecord record {};
    uint64_t regvalU64 = 123458;
    RegisterType regType = RegisterType::CTRL;
    size_t regIdx = static_cast<size_t>(regType);
    uint64_t regvalU64Dft = Sanitizer::g_regInfoTbl[regIdx].regDftVal;
    RecordType recordType = RecordType::SET_CTRL;
    uint16_t coreId = 0;
    
    // 未重置为默认值报告警
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64);
    RecordPreProcess::GetInstance().Process(record, events);
    ASSERT_EQ(events.size(), 1U);
    events.emplace_back(endEvent);
    regSan.Do(record, events);

    std::cout << "[UT_DEBUG]msg: \n" << msg << std::endl;
    ASSERT_EQ(regSan.regValActual_[coreId][regIdx], regvalU64);
    ASSERT_TRUE(msg.find("Register " + Sanitizer::g_regInfoTbl[regIdx].regNameStr + " was not reset to default in block aiv("
        + std::to_string(coreId) + ")") != std::string::npos);
    ASSERT_TRUE(msg.find("current value is (" + std::to_string(regvalU64) + ")") != std::string::npos);
}

TEST(RegisterSanitizer, set_ctrl_expect_not_report_exception)
{
    RegisterSanitizer regSan {};
    regSan.Init();
    std::string msg {};
    regSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });

    std::vector<SanEvent> events {};
    SanEvent endEvent {};
    endEvent.isEndFrame = true;
    SanitizerRecord record {};
    uint64_t regvalU64 = 123458;
    RegisterType regType = RegisterType::CTRL;
    size_t regIdx = static_cast<size_t>(regType);
    uint64_t regvalU64Dft = Sanitizer::g_regInfoTbl[regIdx].regDftVal;
    RecordType recordType = RecordType::SET_CTRL;
    uint16_t coreId = 0;

    // 重置为默认值不报告警
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64);
    RecordPreProcess::GetInstance().Process(record, events);
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64Dft);
    RecordPreProcess::GetInstance().Process(record, events);
    ASSERT_EQ(events.size(), 2U);
    events.emplace_back(endEvent);
    regSan.Do(record, events);

    std::cout << "[UT_DEBUG]msg: \n" << msg << std::endl;
    ASSERT_EQ(regSan.regValActual_[coreId][regIdx], regvalU64Dft);
    ASSERT_TRUE(msg.find("Register " + Sanitizer::g_regInfoTbl[regIdx].regNameStr + " was not reset to default in block aiv("
        + std::to_string(coreId) + ")") == std::string::npos);
    ASSERT_TRUE(msg.find("current valure is (" + std::to_string(regvalU64) + ")") == std::string::npos);
}

TEST(RegisterSanitizer, set_ffts_base_addr_expect_report_exception)
{
    RegisterSanitizer regSan {};
    regSan.Init();
    std::string msg {};
    regSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });

    std::vector<SanEvent> events {};
    SanEvent endEvent {};
    endEvent.isEndFrame = true;
    SanitizerRecord record {};
    uint64_t regvalU64 = 123459;
    RegisterType regType = RegisterType::FFTS_BASE_ADDR;
    size_t regIdx = static_cast<size_t>(regType);
    uint64_t regvalU64Dft = Sanitizer::g_regInfoTbl[regIdx].regDftVal;
    RecordType recordType = RecordType::SET_FFTS_BASE_ADDR;
    uint16_t coreId = 1;
    
    // 未重置为默认值报告警
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64);
    RecordPreProcess::GetInstance().Process(record, events);
    ASSERT_EQ(events.size(), 1U);
    events.emplace_back(endEvent);
    regSan.Do(record, events);

    std::cout << "[UT_DEBUG]msg: \n" << msg << std::endl;
    ASSERT_EQ(regSan.regValActual_[coreId][regIdx], regvalU64);
    ASSERT_TRUE(msg.find("Register " + Sanitizer::g_regInfoTbl[regIdx].regNameStr + " was not reset to default in block aiv("
        + std::to_string(coreId) + ")") != std::string::npos);
    ASSERT_TRUE(msg.find("current value is (" + std::to_string(regvalU64) + ")") != std::string::npos);
}

TEST(RegisterSanitizer, set_ffts_base_addr_expect_not_report_exception)
{
    RegisterSanitizer regSan {};
    regSan.Init();
    std::string msg {};
    regSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });

    std::vector<SanEvent> events {};
    SanEvent endEvent {};
    endEvent.isEndFrame = true;
    SanitizerRecord record {};
    uint64_t regvalU64 = 123459;
    RegisterType regType = RegisterType::FFTS_BASE_ADDR;
    size_t regIdx = static_cast<size_t>(regType);
    uint64_t regvalU64Dft = Sanitizer::g_regInfoTbl[regIdx].regDftVal;
    RecordType recordType = RecordType::SET_FFTS_BASE_ADDR;
    uint16_t coreId = 1;

    // 重置为默认值不报告警
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64);
    RecordPreProcess::GetInstance().Process(record, events);
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64Dft);
    RecordPreProcess::GetInstance().Process(record, events);
    ASSERT_EQ(events.size(), 2U);
    events.emplace_back(endEvent);
    regSan.Do(record, events);

    std::cout << "[UT_DEBUG]msg: \n" << msg << std::endl;
    ASSERT_EQ(regSan.regValActual_[coreId][regIdx], regvalU64Dft);
    ASSERT_TRUE(msg.find("Register " + Sanitizer::g_regInfoTbl[regIdx].regNameStr + " was not reset to default in block aiv("
        + std::to_string(coreId) + ")") == std::string::npos);
    ASSERT_TRUE(msg.find("current valure is (" + std::to_string(regvalU64) + ")") == std::string::npos);
}

TEST(RegisterSanitizer, set_fpc_expect_report_exception)
{
    RegisterSanitizer regSan {};
    regSan.Init();
    std::string msg {};
    regSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });

    std::vector<SanEvent> events {};
    SanEvent endEvent {};
    endEvent.isEndFrame = true;
    SanitizerRecord record {};
    uint64_t regvalU64 = 123460;
    RegisterType regType = RegisterType::FPC;
    size_t regIdx = static_cast<size_t>(regType);
    uint64_t regvalU64Dft = Sanitizer::g_regInfoTbl[regIdx].regDftVal;
    RecordType recordType = RecordType::SET_FPC;
    uint16_t coreId = 0;

    // 重置为默认值不报告警
    regSan.Init();
    events.clear();
    msg = "";

    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64);
    RecordPreProcess::GetInstance().Process(record, events);
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64Dft);
    RecordPreProcess::GetInstance().Process(record, events);
    ASSERT_EQ(events.size(), 2U);
    events.emplace_back(endEvent);
    regSan.Do(record, events);

    std::cout << "[UT_DEBUG]msg: \n" << msg << std::endl;
    ASSERT_EQ(regSan.regValActual_[coreId][regIdx], regvalU64Dft);
    ASSERT_TRUE(msg.find("Register " + Sanitizer::g_regInfoTbl[regIdx].regNameStr + " was not reset to default in block aiv("
        + std::to_string(coreId) + ")") == std::string::npos);
    ASSERT_TRUE(msg.find("current valure is (" + std::to_string(regvalU64) + ")") == std::string::npos);
}

TEST(RegisterSanitizer, set_fpc_expect_not_report_exception)
{
    RegisterSanitizer regSan {};
    regSan.Init();
    std::string msg {};
    regSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });

    std::vector<SanEvent> events {};
    SanEvent endEvent {};
    endEvent.isEndFrame = true;
    SanitizerRecord record {};
    uint64_t regvalU64 = 123460;
    RegisterType regType = RegisterType::FPC;
    size_t regIdx = static_cast<size_t>(regType);
    uint64_t regvalU64Dft = Sanitizer::g_regInfoTbl[regIdx].regDftVal;
    RecordType recordType = RecordType::SET_FPC;
    uint16_t coreId = 0;

    // 重置为默认值不报告警
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64);
    RecordPreProcess::GetInstance().Process(record, events);
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64Dft);
    RecordPreProcess::GetInstance().Process(record, events);
    ASSERT_EQ(events.size(), 2U);
    events.emplace_back(endEvent);
    regSan.Do(record, events);

    std::cout << "[UT_DEBUG]msg: \n" << msg << std::endl;
    ASSERT_EQ(regSan.regValActual_[coreId][regIdx], regvalU64Dft);
    ASSERT_TRUE(msg.find("Register " + Sanitizer::g_regInfoTbl[regIdx].regNameStr + " was not reset to default in block aiv("
        + std::to_string(coreId) + ")") == std::string::npos);
    ASSERT_TRUE(msg.find("current valure is (" + std::to_string(regvalU64) + ")") == std::string::npos);
}


TEST(RegisterSanitizer, set_quant_pre_expect_report_exception)
{
    RegisterSanitizer regSan {};
    regSan.Init();
    std::string msg {};
    regSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });

    std::vector<SanEvent> events {};
    SanEvent endEvent {};
    endEvent.isEndFrame = true;
    SanitizerRecord record {};
    uint64_t regvalU64 = 123461;
    RegisterType regType = RegisterType::QUANT_PRE;
    size_t regIdx = static_cast<size_t>(regType);
    uint64_t regvalU64Dft = Sanitizer::g_regInfoTbl[regIdx].regDftVal;
    RecordType recordType = RecordType::SET_QUANT_PRE;
    uint16_t coreId = 1;
    
    // 未重置为默认值报告警
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64);
    RecordPreProcess::GetInstance().Process(record, events);
    ASSERT_EQ(events.size(), 1U);
    events.emplace_back(endEvent);
    regSan.Do(record, events);

    std::cout << "[UT_DEBUG]msg: \n" << msg << std::endl;
    ASSERT_EQ(regSan.regValActual_[coreId][regIdx], regvalU64);
    ASSERT_TRUE(msg.find("Register " + Sanitizer::g_regInfoTbl[regIdx].regNameStr + " was not reset to default in block aiv("
        + std::to_string(coreId) + ")") != std::string::npos);
    ASSERT_TRUE(msg.find("current value is (" + std::to_string(regvalU64) + ")") != std::string::npos);
}

TEST(RegisterSanitizer, set_quant_pre_expect_not_report_exception)
{
    RegisterSanitizer regSan {};
    regSan.Init();
    std::string msg {};
    regSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });

    std::vector<SanEvent> events {};
    SanEvent endEvent {};
    endEvent.isEndFrame = true;
    SanitizerRecord record {};
    uint64_t regvalU64 = 123461;
    RegisterType regType = RegisterType::QUANT_PRE;
    size_t regIdx = static_cast<size_t>(regType);
    uint64_t regvalU64Dft = Sanitizer::g_regInfoTbl[regIdx].regDftVal;
    RecordType recordType = RecordType::SET_QUANT_PRE;
    uint16_t coreId = 1;

    // 重置为默认值不报告警
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64);
    RecordPreProcess::GetInstance().Process(record, events);
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64Dft);
    RecordPreProcess::GetInstance().Process(record, events);
    ASSERT_EQ(events.size(), 2U);
    events.emplace_back(endEvent);
    regSan.Do(record, events);

    std::cout << "[UT_DEBUG]msg: \n" << msg << std::endl;
    ASSERT_EQ(regSan.regValActual_[coreId][regIdx], regvalU64Dft);
    ASSERT_TRUE(msg.find("Register " + Sanitizer::g_regInfoTbl[regIdx].regNameStr + " was not reset to default in block aiv("
        + std::to_string(coreId) + ")") == std::string::npos);
    ASSERT_TRUE(msg.find("current valure is (" + std::to_string(regvalU64) + ")") == std::string::npos);
}

TEST(RegisterSanitizer, set_quant_post_expect_not_report_exception)
{
    RegisterSanitizer regSan {};
    regSan.Init();
    std::string msg {};
    regSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });

    std::vector<SanEvent> events {};
    SanEvent endEvent {};
    endEvent.isEndFrame = true;
    SanitizerRecord record {};
    uint64_t regvalU64 = 123462;
    RegisterType regType = RegisterType::QUANT_POST;
    size_t regIdx = static_cast<size_t>(regType);
    uint64_t regvalU64Dft = Sanitizer::g_regInfoTbl[regIdx].regDftVal;
    RecordType recordType = RecordType::SET_QUANT_POST;
    uint16_t coreId = 0;
    
    // 未重置为默认值不报告警，因为post_expect寄存器不检测
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64);
    RecordPreProcess::GetInstance().Process(record, events);
    ASSERT_EQ(events.size(), 1U);
    events.emplace_back(endEvent);
    regSan.Do(record, events);

    std::cout << "[UT_DEBUG]msg: \n" << msg << std::endl;
    ASSERT_EQ(regSan.regValActual_[coreId][regIdx], regvalU64);
    ASSERT_TRUE(msg.find("Register " + Sanitizer::g_regInfoTbl[regIdx].regNameStr + " was not reset to default in block aiv("
        + std::to_string(coreId) + ")") == std::string::npos);
    ASSERT_TRUE(msg.find("current value is (" + std::to_string(regvalU64) + ")") == std::string::npos);
}

TEST(RegisterSanitizer, set_quant_post_expect_not_report_exception_2)
{
    RegisterSanitizer regSan {};
    regSan.Init();
    std::string msg {};
    regSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });

    std::vector<SanEvent> events {};
    SanEvent endEvent {};
    endEvent.isEndFrame = true;
    SanitizerRecord record {};
    uint64_t regvalU64 = 123462;
    RegisterType regType = RegisterType::QUANT_POST;
    size_t regIdx = static_cast<size_t>(regType);
    uint64_t regvalU64Dft = Sanitizer::g_regInfoTbl[regIdx].regDftVal;
    RecordType recordType = RecordType::SET_QUANT_POST;
    uint16_t coreId = 0;

    // 重置为默认值不报告警
    regSan.Init();
    events.clear();
    msg = "";

    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64);
    RecordPreProcess::GetInstance().Process(record, events);
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64Dft);
    RecordPreProcess::GetInstance().Process(record, events);
    ASSERT_EQ(events.size(), 2U);
    events.emplace_back(endEvent);
    regSan.Do(record, events);

    std::cout << "[UT_DEBUG]msg: \n" << msg << std::endl;
    ASSERT_EQ(regSan.regValActual_[coreId][regIdx], regvalU64Dft);
    ASSERT_TRUE(msg.find("Register " + Sanitizer::g_regInfoTbl[regIdx].regNameStr + " was not reset to default in block aiv("
        + std::to_string(coreId) + ")") == std::string::npos);
    ASSERT_TRUE(msg.find("current valure is (" + std::to_string(regvalU64) + ")") == std::string::npos);
}

TEST(RegisterSanitizer, set_lrelu_alpha_uint64_expect_report_exception)
{
    RegisterSanitizer regSan {};
    regSan.Init();
    std::string msg {};
    regSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });

    std::vector<SanEvent> events {};
    SanEvent endEvent {};
    endEvent.isEndFrame = true;
    SanitizerRecord record {};
    uint64_t regvalU64 = 123463;
    uint16_t regvalHalf = 123464;
    uint32_t regvalFloat = 123465;
    RegisterType regType = RegisterType::LRELU_ALPHA;
    size_t regIdx = static_cast<size_t>(regType);
    uint64_t regvalDft = Sanitizer::g_regInfoTbl[regIdx].regDftVal;
    RecordType recordType = RecordType::SET_LRELU_ALPHA;
    uint16_t coreId = 1;
    
    // 未重置为默认值报告警 - UINT64
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64);
    RecordPreProcess::GetInstance().Process(record, events);
    ASSERT_EQ(events.size(), 1U);
    events.emplace_back(endEvent);
    regSan.Do(record, events);

    std::cout << "[UT_DEBUG]msg: \n" << msg << std::endl;
    ASSERT_EQ(regSan.regValActual_[coreId][regIdx], regvalU64);
    ASSERT_TRUE(msg.find("Register " + Sanitizer::g_regInfoTbl[regIdx].regNameStr + " was not reset to default in block aiv("
        + std::to_string(coreId) + ")") != std::string::npos);
    ASSERT_TRUE(msg.find("current value is (" + std::to_string(regvalU64) + ")") != std::string::npos);
}

TEST(RegisterSanitizer, set_lrelu_alpha_uint64_expect_not_report_exception)
{
    RegisterSanitizer regSan {};
    regSan.Init();
    std::string msg {};
    regSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });

    std::vector<SanEvent> events {};
    SanEvent endEvent {};
    endEvent.isEndFrame = true;
    SanitizerRecord record {};
    uint64_t regvalU64 = 123463;
    uint16_t regvalHalf = 123464;
    uint32_t regvalFloat = 123465;
    RegisterType regType = RegisterType::LRELU_ALPHA;
    size_t regIdx = static_cast<size_t>(regType);
    uint64_t regvalDft = Sanitizer::g_regInfoTbl[regIdx].regDftVal;
    RecordType recordType = RecordType::SET_LRELU_ALPHA;
    uint16_t coreId = 1;
    
    // 重置为默认值不报告警 - UINT64
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalU64);
    RecordPreProcess::GetInstance().Process(record, events);
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalDft);
    RecordPreProcess::GetInstance().Process(record, events);
    ASSERT_EQ(events.size(), 2U);
    events.emplace_back(endEvent);
    regSan.Do(record, events);

    std::cout << "[UT_DEBUG]msg: \n" << msg << std::endl;
    ASSERT_EQ(regSan.regValActual_[coreId][regIdx], regvalDft);
    ASSERT_TRUE(msg.find("Register " + Sanitizer::g_regInfoTbl[regIdx].regNameStr + " was not reset to default in block aiv("
        + std::to_string(coreId) + ")") == std::string::npos);
    ASSERT_TRUE(msg.find("current valure is (" + std::to_string(regvalU64) + ")") == std::string::npos);
}

TEST(RegisterSanitizer, set_lrelu_alpha_half_expect_report_exception)
{
    RegisterSanitizer regSan {};
    regSan.Init();
    std::string msg {};
    regSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });

    std::vector<SanEvent> events {};
    SanEvent endEvent {};
    endEvent.isEndFrame = true;
    SanitizerRecord record {};
    uint64_t regvalU64 = 123463;
    uint16_t regvalHalf = 123464;
    uint32_t regvalFloat = 123465;
    RegisterType regType = RegisterType::LRELU_ALPHA;
    size_t regIdx = static_cast<size_t>(regType);
    uint64_t regvalDft = Sanitizer::g_regInfoTbl[regIdx].regDftVal;
    RecordType recordType = RecordType::SET_LRELU_ALPHA;
    uint16_t coreId = 1;

    // 未重置为默认值报告警 - half
    regSan.Init();
    events.clear();
    msg = "";
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_HALF, regvalHalf);
    RecordPreProcess::GetInstance().Process(record, events);
    ASSERT_EQ(events.size(), 1U);
    events.emplace_back(endEvent);
    regSan.Do(record, events);

    std::cout << "[UT_DEBUG]msg: \n" << msg << std::endl;
    ASSERT_EQ(regSan.regValActual_[coreId][regIdx], regvalHalf);
    ASSERT_TRUE(msg.find("Register " + Sanitizer::g_regInfoTbl[regIdx].regNameStr + " was not reset to default in block aiv("
        + std::to_string(coreId) + ")") != std::string::npos);
    ASSERT_TRUE(msg.find("current value is (" + std::to_string(regvalHalf) + ")") != std::string::npos);
}

TEST(RegisterSanitizer, set_lrelu_alpha_half_expect_not_report_exception)
{
    RegisterSanitizer regSan {};
    regSan.Init();
    std::string msg {};
    regSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });

    std::vector<SanEvent> events {};
    SanEvent endEvent {};
    endEvent.isEndFrame = true;
    SanitizerRecord record {};
    uint64_t regvalU64 = 123463;
    uint16_t regvalHalf = 123464;
    uint32_t regvalFloat = 123465;
    RegisterType regType = RegisterType::LRELU_ALPHA;
    size_t regIdx = static_cast<size_t>(regType);
    uint64_t regvalDft = Sanitizer::g_regInfoTbl[regIdx].regDftVal;
    RecordType recordType = RecordType::SET_LRELU_ALPHA;
    uint16_t coreId = 1;

    // 重置为默认值不报告警 - half
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_HALF, static_cast<uint64_t>(regvalU64));
    RecordPreProcess::GetInstance().Process(record, events);
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_HALF, regvalDft);
    RecordPreProcess::GetInstance().Process(record, events);
    ASSERT_EQ(events.size(), 2U);
    events.emplace_back(endEvent);
    regSan.Do(record, events);

    std::cout << "[UT_DEBUG]msg: \n" << msg << std::endl;
    ASSERT_EQ(regSan.regValActual_[coreId][regIdx], regvalDft);
    ASSERT_TRUE(msg.find("Register " + Sanitizer::g_regInfoTbl[regIdx].regNameStr + " was not reset to default in block aiv("
        + std::to_string(coreId) + ")") == std::string::npos);
    ASSERT_TRUE(msg.find("current valure is (" + std::to_string(regvalU64) + ")") == std::string::npos);
}

TEST(RegisterSanitizer, set_lrelu_alpha_float_expect_report_exception)
{
    RegisterSanitizer regSan {};
    regSan.Init();
    std::string msg {};
    regSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });

    std::vector<SanEvent> events {};
    SanEvent endEvent {};
    endEvent.isEndFrame = true;
    SanitizerRecord record {};
    uint64_t regvalU64 = 123463;
    uint16_t regvalHalf = 123464;
    uint32_t regvalFloat = 123465;
    RegisterType regType = RegisterType::LRELU_ALPHA;
    size_t regIdx = static_cast<size_t>(regType);
    uint64_t regvalDft = Sanitizer::g_regInfoTbl[regIdx].regDftVal;
    RecordType recordType = RecordType::SET_LRELU_ALPHA;
    uint16_t coreId = 1;

    // 未重置为默认值报告警 - float
    regSan.Init();
    events.clear();
    msg = "";
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_UINT64, regvalFloat);
    RecordPreProcess::GetInstance().Process(record, events);
    ASSERT_EQ(events.size(), 1U);
    events.emplace_back(endEvent);
    regSan.Do(record, events);

    std::cout << "[UT_DEBUG]msg: \n" << msg << std::endl;
    ASSERT_EQ(regSan.regValActual_[coreId][regIdx], regvalFloat);
    ASSERT_TRUE(msg.find("Register " + Sanitizer::g_regInfoTbl[regIdx].regNameStr + " was not reset to default in block aiv("
        + std::to_string(coreId) + ")") != std::string::npos);
    ASSERT_TRUE(msg.find("current value is (" + std::to_string(regvalFloat) + ")") != std::string::npos);

    // 重置为默认值不报告警 - float
    regSan.Init();
    events.clear();
    msg = "";

    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_FLOAT, static_cast<uint64_t>(regvalFloat));
    RecordPreProcess::GetInstance().Process(record, events);
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_FLOAT, regvalDft);
    RecordPreProcess::GetInstance().Process(record, events);
    ASSERT_EQ(events.size(), 2U);
    events.emplace_back(endEvent);
    regSan.Do(record, events);

    std::cout << "[UT_DEBUG]msg: \n" << msg << std::endl;
    ASSERT_EQ(regSan.regValActual_[coreId][regIdx], regvalDft);
    ASSERT_TRUE(msg.find("Register " + Sanitizer::g_regInfoTbl[regIdx].regNameStr + " was not reset to default in block aiv("
        + std::to_string(coreId) + ")") == std::string::npos);
    ASSERT_TRUE(msg.find("current valure is (" + std::to_string(regvalFloat) + ")") == std::string::npos);
}

TEST(RegisterSanitizer, set_lrelu_alpha_float_expect_not_report_exception)
{
    RegisterSanitizer regSan {};
    regSan.Init();
    std::string msg {};
    regSan.RegisterNotifyFunc([&msg](LogLv const&, SanitizerBase::MSG_GEN &&gen) { msg += gen().message; });

    std::vector<SanEvent> events {};
    SanEvent endEvent {};
    endEvent.isEndFrame = true;
    SanitizerRecord record {};
    uint64_t regvalU64 = 123463;
    uint16_t regvalHalf = 123464;
    uint32_t regvalFloat = 123465;
    RegisterType regType = RegisterType::LRELU_ALPHA;
    size_t regIdx = static_cast<size_t>(regType);
    uint64_t regvalDft = Sanitizer::g_regInfoTbl[regIdx].regDftVal;
    RecordType recordType = RecordType::SET_LRELU_ALPHA;
    uint16_t coreId = 1;

    // 重置为默认值不报告警 - float
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_FLOAT, static_cast<uint64_t>(regvalFloat));
    RecordPreProcess::GetInstance().Process(record, events);
    UT_FillRegRecord(record, recordType, coreId, RegisterValueType::VAL_FLOAT, regvalDft);
    RecordPreProcess::GetInstance().Process(record, events);
    ASSERT_EQ(events.size(), 2U);
    events.emplace_back(endEvent);
    regSan.Do(record, events);

    std::cout << "[UT_DEBUG]msg: \n" << msg << std::endl;
    ASSERT_EQ(regSan.regValActual_[coreId][regIdx], regvalDft);
    ASSERT_TRUE(msg.find("Register " + Sanitizer::g_regInfoTbl[regIdx].regNameStr + " was not reset to default in block aiv("
        + std::to_string(coreId) + ")") == std::string::npos);
    ASSERT_TRUE(msg.find("current valure is (" + std::to_string(regvalFloat) + ")") == std::string::npos);
}

}