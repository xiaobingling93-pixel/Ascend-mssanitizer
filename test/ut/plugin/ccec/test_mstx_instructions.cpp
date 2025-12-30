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

#include "ccec_defs.h"
#include "data_process.h"
#include "plugin/ccec/mstx_instructions.cpp"

using namespace Sanitizer;

namespace SanitizerTest {

TEST(MstxInstructions, dump_cross_records_expect_get_correct_records)
{
    MstxCrossRecord crossRecord1 = {
        .addr = 0x300,
        .flagId = 2,
        .pipe = PipeType::PIPE_V,
        .isMore = false,
        .isMerge = true,
    };

    MstxRecord record = {
        .interfaceType      = InterfaceType::MSTX_SET_CROSS_SYNC,
        .bufferLens         = sizeof(MstxCrossRecord),
        .location           = {10, 10, 0x200},
        .error              = false,
    };

    record.interface.mstxCrossRecord = crossRecord1;
    std::vector<uint8_t> memInfo(UT_RECORD_BUF_SIZE, 0);
    RecordGlobalHead head{};
    head.checkParms.racecheck = true;
    std::copy_n(reinterpret_cast<uint8_t const*>(&head), sizeof(RecordGlobalHead), memInfo.begin());
    __sanitizer_report___mstx_dfx_report_stub(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, static_cast<uint32_t>(record.interfaceType), record.bufferLens, &crossRecord1);

    MstxCrossRecord crossRecord2 = {
        .addr = 0x500,
        .flagId = 6,
        .pipe = PipeType::PIPE_MTE2,
        .isMore = true,
        .isMerge = false,
    };

    __sanitizer_report___mstx_dfx_report_stub(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, static_cast<uint32_t>(record.interfaceType), record.bufferLens, &crossRecord2);

    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::MSTX_STUB>(ptr, record));
    ptr += sizeof(RecordType) + sizeof(MstxRecord);
    record.interface.mstxCrossRecord = crossRecord2;
    ASSERT_TRUE(CheckRecordEqual<RecordType::MSTX_STUB>(ptr, record));
}

TEST(MstxInstructions, dump_cross_record_with_error_buffer_size_expect_get_error)
{
    MstxCrossRecord crossRecord = {
        .addr = 0x300,
        .flagId = 2,
        .pipe = PipeType::PIPE_V,
        .isMore = false,
        .isMerge = true,
    };

    MstxRecord record = {
        .interfaceType      = InterfaceType::MSTX_WAIT_CROSS_SYNC,
        .bufferLens         = sizeof(MstxCrossRecord),
        .location           = {10, 10, 0x200},
        .error              = false,
    };
    record.interface.mstxCrossRecord = crossRecord;
    std::vector<uint8_t> memInfo(UT_RECORD_BUF_SIZE, 0);
    RecordGlobalHead head{};
    head.checkParms.racecheck = true;
    std::copy_n(reinterpret_cast<uint8_t const*>(&head), sizeof(RecordGlobalHead), memInfo.begin());
    __sanitizer_report___mstx_dfx_report_stub(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, static_cast<uint32_t>(record.interfaceType), record.bufferLens + 1, &crossRecord);

    const MstxRecord mstxRecord = *reinterpret_cast<const MstxRecord *>(memInfo.data() + sizeof(RecordGlobalHead) +
        sizeof(RecordBlockHead) + sizeof(RecordType));
    ASSERT_TRUE(mstxRecord.error);
}

TEST(MstxInstructions, dump_cross_record_with_nullptr_expect_get_error)
{
    MstxRecord record = {
        .interfaceType      = InterfaceType::MSTX_WAIT_CROSS_SYNC,
        .bufferLens         = sizeof(MstxCrossRecord),
        .location           = {10, 10, 0x200},
        .error              = false,
    };
    std::vector<uint8_t> memInfo(UT_RECORD_BUF_SIZE, 0);
    RecordGlobalHead head{};
    head.checkParms.racecheck = true;
    std::copy_n(reinterpret_cast<uint8_t const*>(&head), sizeof(RecordGlobalHead), memInfo.begin());
    __sanitizer_report___mstx_dfx_report_stub(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, static_cast<uint32_t>(record.interfaceType), record.bufferLens, nullptr);

    const MstxRecord mstxRecord = *reinterpret_cast<const MstxRecord *>(memInfo.data() + sizeof(RecordGlobalHead) +
        sizeof(RecordBlockHead) + sizeof(RecordType));
    ASSERT_TRUE(mstxRecord.error);
}

TEST(MstxInstructions, dump_cross_record_with_illegal_interface_type_expect_get_error)
{
    MstxRecord record = {
        .interfaceType      = InterfaceType::MSTX_WAIT_CROSS_SYNC,
        .bufferLens         = sizeof(MstxCrossRecord),
        .location           = {10, 10, 0x200},
        .error              = false,
    };
    std::vector<uint8_t> memInfo(UT_RECORD_BUF_SIZE, 0);
    RecordGlobalHead head{};
    head.checkParms.racecheck = true;
    std::copy_n(reinterpret_cast<uint8_t const*>(&head), sizeof(RecordGlobalHead), memInfo.begin());
    __sanitizer_report___mstx_dfx_report_stub(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, -1, record.bufferLens, nullptr);

    const MstxRecord mstxRecord = *reinterpret_cast<const MstxRecord *>(memInfo.data() + sizeof(RecordGlobalHead) +
        sizeof(RecordBlockHead) + sizeof(RecordType));
    ASSERT_TRUE(mstxRecord.error);
}

TEST(MstxInstructions, dump_cross_record_with_initcheck_expect_get_none)
{
    MstxRecord record = {
        .interfaceType      = InterfaceType::MSTX_WAIT_CROSS_SYNC,
        .bufferLens         = sizeof(MstxCrossRecord),
        .location           = {10, 10, 0x200},
        .error              = false,
    };
    std::vector<uint8_t> memInfo(UT_RECORD_BUF_SIZE, 0);
    RecordGlobalHead head{};
    head.checkParms.initcheck = true;
    std::copy_n(reinterpret_cast<uint8_t const*>(&head), sizeof(RecordGlobalHead), memInfo.begin());
    __sanitizer_report___mstx_dfx_report_stub(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, static_cast<uint32_t>(record.interfaceType), record.bufferLens, nullptr);
    RecordBlockHead blockHead = *reinterpret_cast<RecordBlockHead const *>(memInfo.data() + sizeof(RecordGlobalHead));
    ASSERT_EQ(blockHead.recordWriteCount, 0);
}

TEST(MstxInstructions, dump_cross_record_with_memcheck_expect_get_none)
{
    MstxRecord record = {
        .interfaceType      = InterfaceType::MSTX_WAIT_CROSS_SYNC,
        .bufferLens         = sizeof(MstxCrossRecord),
        .location           = {10, 10, 0x200},
        .error              = false,
    };
    std::vector<uint8_t> memInfo = CreateMemInfo();
    __sanitizer_report___mstx_dfx_report_stub(memInfo.data(), record.location.fileNo, record.location.lineNo,
        record.location.pc, static_cast<uint32_t>(record.interfaceType), record.bufferLens, nullptr);
    RecordBlockHead blockHead = *reinterpret_cast<RecordBlockHead const *>(memInfo.data() + sizeof(RecordGlobalHead));
    ASSERT_EQ(blockHead.recordWriteCount, 0);
}

}
