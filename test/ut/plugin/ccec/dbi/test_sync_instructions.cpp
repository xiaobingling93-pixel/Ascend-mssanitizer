#include <gtest/gtest.h>

#include "../../ccec_defs.h"
#include "../data_process.h"
#include "plugin/ccec/dbi/probes/sync_instructions.cpp"

using namespace Sanitizer;

namespace SanitizerTest {

constexpr uint64_t MEM_INFO_SIZE = 1024 * 1024;


TEST(SyncInstructions, dbi_ffts_cross_core_sync_expect_get_correct_records)
{
    FftsSyncRecord record{};
    record.location = {10, 10};
    record.dst = PipeType::PIPE_MTE3;
    record.mode = 0;
    record.flagID = 2;
    uint8_t mode = 0;
    uint8_t id = 2;
    uint64_t config = (0x1 | (mode << 4) | (id << 8));
    std::vector<uint8_t> memInfo(MEM_INFO_SIZE, 0);
    __sanitizer_report_set_cross_core(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                            record.location.pc, record.dst, config);
    RecordBlockHead blockHead = *reinterpret_cast<RecordBlockHead const *>(memInfo.data() + sizeof(RecordGlobalHead));
    ASSERT_EQ(blockHead.recordWriteCount, 0);

    RecordGlobalHead head{};
    head.checkParms.initcheck = true;
    std::copy_n(reinterpret_cast<uint8_t const*>(&head), sizeof(RecordGlobalHead), memInfo.begin());
    __sanitizer_report_set_cross_core(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                            record.location.pc, record.dst, config);
    ASSERT_EQ(blockHead.recordWriteCount, 0);

    head.checkParms.racecheck = true;
    std::copy_n(reinterpret_cast<uint8_t const*>(&head), sizeof(RecordGlobalHead), memInfo.begin());
    __sanitizer_report_set_cross_core(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                            record.location.pc, record.dst, config);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::FFTS_SYNC>(ptr, record));
}

TEST(SyncInstructions, dbi_wait_flag_dev_expect_get_correct_records)
{
    WaitFlagDevPipeRecord record{};
    record.location = {10, 10};
    record.flagID = 2;
    record.pipe = PipeType::PIPE_MTE3;
    std::vector<uint8_t> memInfo(MEM_INFO_SIZE, 0);
    __sanitizer_report_wait_flag_dev_pipe(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                     record.location.pc, PipeType::PIPE_S, 2);
    RecordBlockHead blockHead = *reinterpret_cast<RecordBlockHead const *>(memInfo.data() + sizeof(RecordGlobalHead));
    ASSERT_EQ(blockHead.recordWriteCount, 0);

    RecordGlobalHead head{};
    head.checkParms.initcheck = true;
    std::copy_n(reinterpret_cast<uint8_t const*>(&head), sizeof(RecordGlobalHead), memInfo.begin());
    __sanitizer_report_wait_flag_dev_pipe(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                     record.location.pc, PipeType::PIPE_MTE2, 2);
    ASSERT_EQ(blockHead.recordWriteCount, 0);

    head.checkParms.racecheck = true;
    std::copy_n(reinterpret_cast<uint8_t const*>(&head), sizeof(RecordGlobalHead), memInfo.begin());
    __sanitizer_report_wait_flag_dev_pipe(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                     record.location.pc, PipeType::PIPE_MTE3, 2);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::WAIT_FLAG_DEV_PIPE>(ptr, record));
}

TEST(SyncInstructions, dbi_wait_flag_devi_expect_get_correct_records)
{
    WaitFlagDevPipeRecord record{};
    record.location = {10, 10};
    record.flagID = 2;
    record.pipe = PipeType::PIPE_MTE3;
    std::vector<uint8_t> memInfo(MEM_INFO_SIZE, 0);
    __sanitizer_report_wait_flag_devi_pipe(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                     record.location.pc, PipeType::PIPE_S, 2);
    RecordBlockHead blockHead = *reinterpret_cast<RecordBlockHead const *>(memInfo.data() + sizeof(RecordGlobalHead));
    ASSERT_EQ(blockHead.recordWriteCount, 0);

    RecordGlobalHead head{};
    head.checkParms.initcheck = true;
    std::copy_n(reinterpret_cast<uint8_t const*>(&head), sizeof(RecordGlobalHead), memInfo.begin());
    __sanitizer_report_wait_flag_devi_pipe(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                     record.location.pc, PipeType::PIPE_MTE2, 2);
    ASSERT_EQ(blockHead.recordWriteCount, 0);

    head.checkParms.racecheck = true;
    std::copy_n(reinterpret_cast<uint8_t const*>(&head), sizeof(RecordGlobalHead), memInfo.begin());
    __sanitizer_report_wait_flag_devi_pipe(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                     record.location.pc, PipeType::PIPE_MTE3, 2);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::WAIT_FLAG_DEVI_PIPE>(ptr, record));
}

TEST(SyncInstructions, dbi_set_intra_block_sync_expect_get_correct_records)
{
    IntraBlockSyncRecord record{};
    record.location = {10, 10};
    record.pipe = PipeType::PIPE_MTE3;
    record.syncID = 2;
    std::vector<uint8_t> memInfo(MEM_INFO_SIZE, 0);
    __sanitizer_report_set_intra_block(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                            record.location.pc, record.pipe, record.syncID);
    RecordBlockHead blockHead = *reinterpret_cast<RecordBlockHead const *>(memInfo.data() + sizeof(RecordGlobalHead));
    ASSERT_EQ(blockHead.recordWriteCount, 0);

    RecordGlobalHead head{};
    head.checkParms.initcheck = true;
    std::copy_n(reinterpret_cast<uint8_t const*>(&head), sizeof(RecordGlobalHead), memInfo.begin());
    __sanitizer_report_set_intra_block(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                            record.location.pc, record.pipe, record.syncID);
    ASSERT_EQ(blockHead.recordWriteCount, 0);

    head.checkParms.racecheck = true;
    std::copy_n(reinterpret_cast<uint8_t const*>(&head), sizeof(RecordGlobalHead), memInfo.begin());
    __sanitizer_report_set_intra_block(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                            record.location.pc, record.pipe, record.syncID);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_INTRA_BLOCK>(ptr, record));
}

TEST(SyncInstructions, dbi_wait_intra_block_sync_expect_get_correct_records)
{
    IntraBlockSyncRecord record{};
    record.location = {10, 10};
    record.pipe = PipeType::PIPE_MTE3;
    record.syncID = 2;
    std::vector<uint8_t> memInfo(MEM_INFO_SIZE, 0);
    __sanitizer_report_wait_intra_block(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                            record.location.pc, record.pipe, record.syncID);
    RecordBlockHead blockHead = *reinterpret_cast<RecordBlockHead const *>(memInfo.data() + sizeof(RecordGlobalHead));
    ASSERT_EQ(blockHead.recordWriteCount, 0);

    RecordGlobalHead head{};
    head.checkParms.initcheck = true;
    std::copy_n(reinterpret_cast<uint8_t const*>(&head), sizeof(RecordGlobalHead), memInfo.begin());
    __sanitizer_report_wait_intra_block(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                            record.location.pc, record.pipe, record.syncID);
    ASSERT_EQ(blockHead.recordWriteCount, 0);

    head.checkParms.racecheck = true;
    std::copy_n(reinterpret_cast<uint8_t const*>(&head), sizeof(RecordGlobalHead), memInfo.begin());
    __sanitizer_report_wait_intra_block(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                            record.location.pc, record.pipe, record.syncID);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::WAIT_INTRA_BLOCK>(ptr, record));
}

TEST(SyncInstructions, dbi_set_intra_blocki_sync_expect_get_correct_records)
{
    IntraBlockSyncRecord record{};
    record.location = {10, 10};
    record.pipe = PipeType::PIPE_MTE2;
    record.syncID = 2;
    std::vector<uint8_t> memInfo(MEM_INFO_SIZE, 0);
    __sanitizer_report_set_intra_blocki(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                            record.location.pc, record.pipe, record.syncID);
    RecordBlockHead blockHead = *reinterpret_cast<RecordBlockHead const *>(memInfo.data() + sizeof(RecordGlobalHead));
    ASSERT_EQ(blockHead.recordWriteCount, 0);

    RecordGlobalHead head{};
    head.checkParms.initcheck = true;
    std::copy_n(reinterpret_cast<uint8_t const*>(&head), sizeof(RecordGlobalHead), memInfo.begin());
    __sanitizer_report_set_intra_blocki(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                            record.location.pc, record.pipe, record.syncID);
    ASSERT_EQ(blockHead.recordWriteCount, 0);

    head.checkParms.racecheck = true;
    std::copy_n(reinterpret_cast<uint8_t const*>(&head), sizeof(RecordGlobalHead), memInfo.begin());
    __sanitizer_report_set_intra_blocki(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                            record.location.pc, record.pipe, record.syncID);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::SET_INTRA_BLOCKI>(ptr, record));
}

TEST(SyncInstructions, dbi_wait_intra_blocki_sync_expect_get_correct_records)
{
    IntraBlockSyncRecord record{};
    record.location = {10, 10};
    record.pipe = PipeType::PIPE_MTE2;
    record.syncID = 2;
    std::vector<uint8_t> memInfo(MEM_INFO_SIZE, 0);
    __sanitizer_report_wait_intra_blocki(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                            record.location.pc, record.pipe, record.syncID);
    RecordBlockHead blockHead = *reinterpret_cast<RecordBlockHead const *>(memInfo.data() + sizeof(RecordGlobalHead));
    ASSERT_EQ(blockHead.recordWriteCount, 0);

    RecordGlobalHead head{};
    head.checkParms.initcheck = true;
    std::copy_n(reinterpret_cast<uint8_t const*>(&head), sizeof(RecordGlobalHead), memInfo.begin());
    __sanitizer_report_wait_intra_blocki(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                            record.location.pc, record.pipe, record.syncID);
    ASSERT_EQ(blockHead.recordWriteCount, 0);

    head.checkParms.racecheck = true;
    std::copy_n(reinterpret_cast<uint8_t const*>(&head), sizeof(RecordGlobalHead), memInfo.begin());
    __sanitizer_report_wait_intra_blocki(memInfo.data(), record.location.fileNo, record.location.lineNo,
                                            record.location.pc, record.pipe, record.syncID);
    uint8_t *ptr = memInfo.data() + sizeof(RecordGlobalHead) + sizeof(RecordBlockHead);
    ASSERT_TRUE(CheckRecordEqual<RecordType::WAIT_INTRA_BLOCKI>(ptr, record));
}

}