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
#include <memory>
#include <thread>
#include <chrono>
#include <cstdint>
#include <unistd.h>
 
#include "communication.h"
#include "config.h"
#include "process.h"
#include "platform_config.h"
#include "protocol.h"
#include "stub_def/stub_def.h"
#include "stub_def/stub_def.cpp"
#include "utility/serializer.h"
 
using namespace Sanitizer;
 
constexpr decltype(MemOpRecord::lineNo) fakeLineNo = 61;
 
void assertMallocAndFreeRecord(MemOpRecord const &record, MemOpType type)
{
    ASSERT_EQ(record.type, type);
    ASSERT_EQ(record.dstAddr, 0x61);
    ASSERT_EQ(record.dstSpace, AddressSpace::GM);
    if (type == MemOpType::MALLOC) {
        ASSERT_EQ(record.memSize, 0x10);
    }
    ASSERT_EQ(record.fileName, std::string("test.cpp"));
    ASSERT_EQ(record.lineNo, fakeLineNo);
}
 
TEST(StubDef, ascend_check_init_with_invalid_device_type_expect_invalid_g_devicetype)
{
    /// reset client
    g_client = nullptr;
 
    std::string socketPath = "/tmp/msop_connect.202511121028.352154.sock";
    CommunicationServer server(socketPath);
    Config config {
            .defaultCheck = true,
            .memCheck = true,
        };
    server.SetClientConnectHook([&config, &server](std::size_t clientId) {
            server.Write(clientId, Serialize<Config>(config));
    });
    server.StartListen();
 
    int status;
    pid_t pid = fork();
    if (pid == 0) {
        setenv("MSSANITIZER_PID", socketPath.c_str(), 1);
        AscendCheckInit("invalid device type");
        ASSERT_EQ(g_deviceType, DeviceType::INVALID);
        exit(0);
    } else if (pid > 0) {
        waitpid(pid, &status, 0);
        server.Close();
    }
}
 
TEST(StubDef, ascend_check_init_with_910A_device_type_expect_910A)
{
    /// reset client
    g_client = nullptr;
 
    std::string socketPath = "/tmp/msop_connect.202511121028.352154.sock";
    CommunicationServer server(socketPath);
    Config config {
            .defaultCheck = true,
            .memCheck = true,
        };
    server.SetClientConnectHook([&config, &server](std::size_t clientId) {
            server.Write(clientId, Serialize<Config>(config));
    });
    server.StartListen();
 
    int status;
    pid_t pid = fork();
    if (pid == 0) {
        setenv("MSSANITIZER_PID", socketPath.c_str(), 1);
        AscendCheckInit("Ascend910PremiumA");
        ASSERT_EQ(g_deviceType, DeviceType::ASCEND_910_PREMIUM_A);
        exit(0);
    } else if (pid > 0) {
        waitpid(pid, &status, 0);
        server.Close();
    }
}

TEST(StubDef, check_exit_with_malloc_free_expect_server_receive_malloc_free_records)
{
    /// reset client
    g_client = nullptr;
    /// reset host records;
    g_hostRecords.clear();
 
    DeviceInfoSummary deviceInfo;
    deviceInfo.blockNum = 0;
 
    std::string msg;
    std::string socketPath = "/tmp/msop_connect.202511121028.352154.sock";
    CommunicationServer server(socketPath);
    server.RegisterMsgHandler([&msg](std::string const &m, Process::MsgRspFunc &func) { msg.append(m); });
    Config config {
            .defaultCheck = true,
            .memCheck = true,
        };
    server.SetClientConnectHook([&config, &server](std::size_t clientId) {
            server.Write(clientId, Serialize<Config>(config));
    });
    server.StartListen();
 
    int status;
    pid_t pid = fork();
    if (pid == 0) {
        setenv("MSSANITIZER_PID", socketPath.c_str(), 1);
        AscendCheckInit("Ascend910PremiumA");
        AscendCheckMalloc(0x61, static_cast<uint32_t>(AddressSpace::GM), 0x10, "test.cpp", fakeLineNo);
        AscendCheckFree(0x61, static_cast<uint32_t>(AddressSpace::GM), "test.cpp", fakeLineNo);
        AscendCheckExit(reinterpret_cast<uint8_t*>(&deviceInfo), sizeof(DeviceInfoSummary));
        exit(0);
    } else if (pid > 0) {
        waitpid(pid, &status, 0);
        server.Close();
 
        /// receive single record header
        PacketHead head;
        ASSERT_TRUE(Deserialize<PacketHead>(msg.substr(0, sizeof(PacketHead)), head));
        ASSERT_EQ(head.type, PacketType::SANITIZER_RECORD);
        /// receive free record
        msg = msg.substr(sizeof(PacketHead));
        SanitizerRecord record;
        ASSERT_TRUE(Deserialize<SanitizerRecord>(msg.substr(0, sizeof(SanitizerRecord)), record));
        ASSERT_EQ(record.version, RecordVersion::MEMORY_RECORD);
        assertMallocAndFreeRecord(record.payload.memoryRecord, MemOpType::FREE);
        /// drop summary packet head and summary
        /// receive single record header
        msg = msg.substr(sizeof(SanitizerRecord) + sizeof(PacketHead) + sizeof(DeviceInfoSummary));
        ASSERT_TRUE(Deserialize<PacketHead>(msg.substr(0, sizeof(PacketHead)), head));
        ASSERT_EQ(head.type, PacketType::SANITIZER_RECORD);
        /// receive malloc record
        msg = msg.substr(sizeof(PacketHead));
        ASSERT_TRUE(Deserialize<SanitizerRecord>(msg.substr(0, sizeof(SanitizerRecord)), record));
        ASSERT_EQ(record.version, RecordVersion::MEMORY_RECORD);
        assertMallocAndFreeRecord(record.payload.memoryRecord, MemOpType::MALLOC);
        /// receive single record header
        msg = msg.substr(sizeof(SanitizerRecord));
        ASSERT_TRUE(Deserialize<PacketHead>(msg.substr(0, sizeof(PacketHead)), head));
        ASSERT_EQ(head.type, PacketType::SANITIZER_RECORD);
        /// receive free record
        msg = msg.substr(sizeof(PacketHead));
        ASSERT_TRUE(Deserialize<SanitizerRecord>(msg.substr(0, sizeof(SanitizerRecord)), record));
        ASSERT_EQ(record.version, RecordVersion::MEMORY_RECORD);
        assertMallocAndFreeRecord(record.payload.memoryRecord, MemOpType::FREE);
    }
}

TEST(StubDef, ascend_check_exit_without_memcheck_enabled_expect_server_receive_nothing)
{
    /// reset client
    g_client = nullptr;
 
    std::string msg;
    std::string socketPath = "/tmp/msop_connect.202511121028.352154.sock";
    CommunicationServer server(socketPath);
    server.RegisterMsgHandler([&msg](std::string const &m, Process::MsgRspFunc &func) { msg.append(m); });
    Config config {
            .defaultCheck = true,
            .memCheck = true,
        };
    server.SetClientConnectHook([&config, &server](std::size_t clientId) {
            server.Write(clientId, Serialize<Config>(config));
    });
    server.StartListen();
 
    int status;
    pid_t pid = fork();
    if (pid == 0) {
        setenv("MSSANITIZER_PID", socketPath.c_str(), 1);
        AscendCheckInit("Ascend910PremiumA");
        AscendCheckExit(nullptr, 0);
        exit(0);
    } else if (pid > 0) {
        Config config {
            .defaultCheck = true,
            .memCheck = true,
        };
        waitpid(pid, &status, 0);
        server.Close();
        ASSERT_TRUE(msg.empty());
    }
}
 
TEST(StubDef, ascend_check_exit_with_memcheck_enabled_expect_server_receive_summary_with_memcheck)
{
    /// reset client
    g_client = nullptr;
    g_hostRecords.clear();
 
    DeviceInfoSummary deviceInfo;
    deviceInfo.blockNum = 0;
 
    std::string msg;
    std::string socketPath = "/tmp/msop_connect.202511121028.352154.sock";
    CommunicationServer server(socketPath);
    server.RegisterMsgHandler([&msg](std::string const &m, Process::MsgRspFunc &func) { msg.append(m); });
    Config config {
            .defaultCheck = true,
            .memCheck = true,
        };
    server.SetClientConnectHook([&config, &server](std::size_t clientId) {
            server.Write(clientId, Serialize<Config>(config));
    });
    server.StartListen();
 
    int status;
    pid_t pid = fork();
    if (pid == 0) {
        setenv("MSSANITIZER_PID", socketPath.c_str(), 1);
        AscendCheckInit("Ascend910PremiumA");
        AscendCheckExit(reinterpret_cast<uint8_t*>(&deviceInfo), sizeof(DeviceInfoSummary));
        exit(0);
    } else if (pid > 0) {
        Config config {
            .defaultCheck = true,
            .memCheck = true,
        };
        waitpid(pid, &status, 0);
        server.Close();
 
        /// receive summary packet head
        PacketHead head;
        ASSERT_TRUE(Deserialize<PacketHead>(msg.substr(0, sizeof(PacketHead)), head));
        ASSERT_EQ(head.type, PacketType::DEVICE_SUMMARY);
        /// drop summary packet head
        msg = msg.substr(sizeof(PacketHead));
        DeviceInfoSummary summary;
        ASSERT_TRUE(Deserialize<DeviceInfoSummary>(msg, summary));
        ASSERT_EQ(summary.blockNum, deviceInfo.blockNum + 1);
        ASSERT_EQ(summary.device, DeviceType::ASCEND_910_PREMIUM_A);
    }
}
 
TEST(StubDef, ascend_check_exit_with_two_bisheng_record_enabled_expect_server_receive_summary_with_memcheck)
{
    g_client = nullptr;
    g_hostRecords.clear();
 
    DeviceInfoSummary deviceInfo{};
    deviceInfo.blockNum = 1;
 
    MemOpRecordBisheng record{};
    record.type = MemOpType::MEMCPY_BLOCKS;
    record.padMode = 1;
 
    BlockSummary blockSum{};
    blockSum.validSize = sizeof(MemOpRecordBisheng) * 2;
 
    std::vector<uint8_t> buf;
    uint16_t bufSize = sizeof(DeviceInfoSummary) + sizeof(BlockSummary) + sizeof(MemOpRecordBisheng) * 2;
 
    buf.resize(bufSize);
    std::copy_n(reinterpret_cast<uint8_t*>(&deviceInfo), sizeof(DeviceInfoSummary), buf.begin());
    std::copy_n(reinterpret_cast<uint8_t*>(&blockSum), sizeof(BlockSummary), buf.begin() + sizeof(DeviceInfoSummary));
    std::copy_n(reinterpret_cast<uint8_t*>(&record), sizeof(MemOpRecordBisheng),
        buf.begin() + sizeof(DeviceInfoSummary) + sizeof(BlockSummary));
    record.padMode = 7;
    std::copy_n(reinterpret_cast<uint8_t*>(&record), sizeof(MemOpRecordBisheng),
        buf.begin() + sizeof(DeviceInfoSummary) + sizeof(BlockSummary) + sizeof(MemOpRecordBisheng));
 
    std::string msg;
    std::string socketPath = "/tmp/msop_connect.202511121028.352154.sock";
    CommunicationServer server(socketPath);
    server.RegisterMsgHandler([&msg](std::string const &m, Process::MsgRspFunc &func) { msg.append(m); });
    Config config {
            .defaultCheck = true,
            .memCheck = true,
        };
    server.SetClientConnectHook([&config, &server](std::size_t clientId) {
            server.Write(clientId, Serialize<Config>(config));
    });
    server.StartListen();
 
    int status;
    pid_t pid = fork();
    if (pid == 0) {
        setenv("MSSANITIZER_PID", socketPath.c_str(), 1);
        AscendCheckInit("Ascend910PremiumA");
        AscendCheckExit(reinterpret_cast<uint8_t*>(buf.data()), bufSize);
        exit(0);
    } else if (pid > 0) {
        waitpid(pid, &status, 0);
        server.Close();
 
        /// receive summary packet head
        PacketHead head;
        ASSERT_TRUE(Deserialize<PacketHead>(msg.substr(0, sizeof(PacketHead)), head));
        ASSERT_EQ(head.type, PacketType::DEVICE_SUMMARY);
        /// drop summary packet head
        msg = msg.substr(sizeof(PacketHead));
        DeviceInfoSummary summary;
        ASSERT_TRUE(Deserialize<DeviceInfoSummary>(msg, summary));
        ASSERT_EQ(summary.blockNum, deviceInfo.blockNum + 1);
        ASSERT_EQ(summary.device, DeviceType::ASCEND_910_PREMIUM_A);
    }
}

TEST(StubDef, ascend_check_exit_with_three_bisheng_record_enabled_expect_server_receive_summary_with_memcheck)
{
    g_client = nullptr;
    g_hostRecords.clear();
 
    DeviceInfoSummary deviceInfo{};
    deviceInfo.blockNum = 1;
 
    MemOpRecordBisheng record{};
    record.type = MemOpType::MEMCPY_BLOCKS;
    record.padMode = 1;
 
    BlockSummary blockSum{};
    blockSum.validSize = sizeof(MemOpRecordBisheng) * 3;
 
    std::vector<uint8_t> buf;
    uint16_t bufSize = sizeof(DeviceInfoSummary) + sizeof(BlockSummary) + sizeof(MemOpRecordBisheng) * 2;
 
    buf.resize(bufSize);
    std::copy_n(reinterpret_cast<uint8_t*>(&deviceInfo), sizeof(DeviceInfoSummary), buf.begin());
    std::copy_n(reinterpret_cast<uint8_t*>(&blockSum), sizeof(BlockSummary), buf.begin() + sizeof(DeviceInfoSummary));
    std::copy_n(reinterpret_cast<uint8_t*>(&record), sizeof(MemOpRecordBisheng),
        buf.begin() + sizeof(DeviceInfoSummary) + sizeof(BlockSummary));
    record.padMode = 7;
    std::copy_n(reinterpret_cast<uint8_t*>(&record), sizeof(MemOpRecordBisheng),
        buf.begin() + sizeof(DeviceInfoSummary) + sizeof(BlockSummary) + sizeof(MemOpRecordBisheng));
 
    std::string msg;
    std::string socketPath = "/tmp/msop_connect.202511121028.352154.sock";
    CommunicationServer server(socketPath);
    server.RegisterMsgHandler([&msg](std::string const &m, Process::MsgRspFunc &func) { msg.append(m); });
    Config config {
            .defaultCheck = true,
            .memCheck = true,
        };
    server.SetClientConnectHook([&config, &server](std::size_t clientId) {
            server.Write(clientId, Serialize<Config>(config));
    });
    server.StartListen();
 
    int status;
    pid_t pid = fork();
    if (pid == 0) {
        setenv("MSSANITIZER_PID", socketPath.c_str(), 1);
        AscendCheckInit("Ascend910PremiumA");
        AscendCheckExit(reinterpret_cast<uint8_t*>(buf.data()), bufSize);
        exit(0);
    } else if (pid > 0) {
        waitpid(pid, &status, 0);
        server.Close();
 
        /// receive summary packet head
        PacketHead head;
        ASSERT_TRUE(Deserialize<PacketHead>(msg.substr(0, sizeof(PacketHead)), head));
        ASSERT_EQ(head.type, PacketType::DEVICE_SUMMARY);
        /// drop summary packet head
        msg = msg.substr(sizeof(PacketHead));
        DeviceInfoSummary summary;
        ASSERT_TRUE(Deserialize<DeviceInfoSummary>(msg, summary));
        ASSERT_EQ(summary.blockNum, deviceInfo.blockNum + 1);
        ASSERT_EQ(summary.device, DeviceType::ASCEND_910_PREMIUM_A);
    }
}
 
TEST(StubDef, ascend_check_exit_with_malloc_free_records_expect_server_receive_malloc_free_records)
{
    /// reset client
    g_client = nullptr;
    /// reset host records;
    g_hostRecords.clear();
 
    DeviceInfoSummary deviceInfo;
    deviceInfo.blockNum = 0;
 
    std::string msg;
    std::string socketPath = "/tmp/msop_connect.202511121028.352154.sock";
    CommunicationServer server(socketPath);
    server.RegisterMsgHandler([&msg](std::string const &m, Process::MsgRspFunc &func) { msg.append(m); });
    Config config {
            .defaultCheck = true,
            .memCheck = true,
        };
    server.SetClientConnectHook([&config, &server](std::size_t clientId) {
            server.Write(clientId, Serialize<Config>(config));
    });
    server.StartListen();
 
    int status;
    pid_t pid = fork();
    if (pid == 0) {
        setenv("MSSANITIZER_PID", socketPath.c_str(), 1);
        AscendCheckInit("Ascend910PremiumA");
        AscendCheckMalloc(0x61, static_cast<uint32_t>(AddressSpace::GM), 0x10, "test.cpp", fakeLineNo);
        AscendCheckFree(0x61, static_cast<uint32_t>(AddressSpace::GM), "test.cpp", fakeLineNo);
        AscendCheckExit(reinterpret_cast<uint8_t*>(&deviceInfo), sizeof(DeviceInfoSummary));
        exit(0);
    } else if (pid > 0) {
        waitpid(pid, &status, 0);
        server.Close();
 
        /// receive single record header
        PacketHead head;
        ASSERT_TRUE(Deserialize<PacketHead>(msg.substr(0, sizeof(PacketHead)), head));
        ASSERT_EQ(head.type, PacketType::SANITIZER_RECORD);
        /// receive free record
        msg = msg.substr(sizeof(PacketHead));
        SanitizerRecord record;
        ASSERT_TRUE(Deserialize<SanitizerRecord>(msg.substr(0, sizeof(SanitizerRecord)), record));
        ASSERT_EQ(record.version, RecordVersion::MEMORY_RECORD);
        assertMallocAndFreeRecord(record.payload.memoryRecord, MemOpType::FREE);
        /// drop summary packet head and summary
        /// receive single record header
        msg = msg.substr(sizeof(SanitizerRecord) + sizeof(PacketHead) + sizeof(DeviceInfoSummary));
        ASSERT_TRUE(Deserialize<PacketHead>(msg.substr(0, sizeof(PacketHead)), head));
        ASSERT_EQ(head.type, PacketType::SANITIZER_RECORD);
        /// receive malloc record
        msg = msg.substr(sizeof(PacketHead));
        ASSERT_TRUE(Deserialize<SanitizerRecord>(msg.substr(0, sizeof(SanitizerRecord)), record));
        ASSERT_EQ(record.version, RecordVersion::MEMORY_RECORD);
        assertMallocAndFreeRecord(record.payload.memoryRecord, MemOpType::MALLOC);
        /// receive single record header
        msg = msg.substr(sizeof(SanitizerRecord));
        ASSERT_TRUE(Deserialize<PacketHead>(msg.substr(0, sizeof(PacketHead)), head));
        ASSERT_EQ(head.type, PacketType::SANITIZER_RECORD);
        /// receive free record
        msg = msg.substr(sizeof(PacketHead));
        ASSERT_TRUE(Deserialize<SanitizerRecord>(msg.substr(0, sizeof(SanitizerRecord)), record));
        ASSERT_EQ(record.version, RecordVersion::MEMORY_RECORD);
        assertMallocAndFreeRecord(record.payload.memoryRecord, MemOpType::FREE);
    }
}