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


#include "stub_def.h"

#include <memory>
#include <map>
#include <unistd.h>
#include <vector>
#include <string>
#include <mutex>
#include <sstream>

#include "securec.h"
#include "record_defs.h"
#include "record_format.h"
#include "communication.h"
#include "config.h"
#include "protocol.h"
#include "platform_config.h"
#include "utility/result.h"
#include "utility/serializer.h"

using namespace Sanitizer;

// 编译器按照如下结构体把内存操作记录写入GM内存
struct MemOpRecordBisheng {
    int32_t recordId; // 各核串行执行顺序的标识
    MemOpType type;
    int32_t coreId;
    int64_t srcAddr;
    int64_t dstAddr;
    AddressSpace srcSpace;
    AddressSpace dstSpace;
    int32_t memSize;
    int32_t blockSize;
    int32_t blockNum;
    int32_t dstStride;
    int32_t srcStride;
    int32_t repeat;
    int32_t padMode;
    int32_t lineNo;
    char fileName[64U]; // 后续优化为文件编码
};

static std::shared_ptr<CommunicationClient> g_client;
static Config g_config;
static DeviceType g_deviceType = DeviceType::INVALID;
static std::vector<MemOpRecord> g_hostRecords;
static std::mutex g_mtx;

namespace {
enum class StubType : uint8_t {
    ASCEND_CHECK_INIT,
    ASCEND_CHECK_EXIT,
    ASCEND_CHECK_MALLOC,
    ASCEND_CHECK_FREE,
};

const std::map<StubType, ToolType> STUB_ENABLE_MAP = {
    {StubType::ASCEND_CHECK_INIT, ToolType::MEMCHECK},
    {StubType::ASCEND_CHECK_EXIT, ToolType::MEMCHECK},
    {StubType::ASCEND_CHECK_MALLOC, ToolType::MEMCHECK},
    {StubType::ASCEND_CHECK_FREE, ToolType::MEMCHECK},
};

inline int32_t CalcMemcpyBlocksSize(const MemOpRecordBisheng &record)
{
    PadMode padMode = static_cast<PadMode>(record.padMode);
    int32_t size = record.blockSize * record.dstStride * (record.repeat - 1) +
                   record.blockSize * record.blockNum;

    if (padMode >= PadMode::PAD_MODE1 && padMode <= PadMode::PAD_MODE5) {
        static std::map<PadMode, int32_t> const upScaleMap = {
            {PadMode::PAD_MODE1, 32},
            {PadMode::PAD_MODE2, 16},
            {PadMode::PAD_MODE3, 8},
            {PadMode::PAD_MODE4, 4},
            {PadMode::PAD_MODE5, 2},
        };
        auto it = upScaleMap.find(padMode);
        if (it != upScaleMap.cend()) {
            size *= it->second;
        }
    } else if (padMode >= PadMode::PAD_MODE6 && padMode <= PadMode::PAD_MODE8) {
        static std::map<PadMode, int32_t> const downScaleMap = {
            {PadMode::PAD_MODE6, 8},
            {PadMode::PAD_MODE7, 4},
            {PadMode::PAD_MODE8, 2},
        };
        auto it = downScaleMap.find(padMode);
        if (it != downScaleMap.cend()) {
            size /= it->second;
        }
    }
    return size;
}

Sanitizer::AddressSpace ConvertAddrSpaceHostToKernel(const uint32_t &space)
{
    using namespace Sanitizer;
    static const std::map<uint32_t, AddressSpace> SPACE_LOOKUP_TABLE = {
        {1U, AddressSpace::GM},
        {3U, AddressSpace::UB},
        {6U, AddressSpace::L1},
        {8U, AddressSpace::L0A},
        {9U, AddressSpace::L0B},
        {10U, AddressSpace::L0C},
    };
    auto it = SPACE_LOOKUP_TABLE.find(space);
    if (it != SPACE_LOOKUP_TABLE.end()) {
        return it->second;
    }
    return AddressSpace::INVALID;
}

void PrintIllegalStrideError(const MemOpRecordBisheng &bishengRecord, const std::string &strideType, int32_t stride)
{
    std::cout << "====== ERROR: " << strideType << " " << stride
              << " should never smaller than blockNum " << bishengRecord.blockNum << std::endl
              << "======    at 0x" << std::hex << bishengRecord.srcAddr << std::dec
              << " on " << bishengRecord.srcSpace << std::endl
              << "======    code in fileName:" << bishengRecord.fileName
              << ":" << bishengRecord.lineNo << std::endl;
}

Sanitizer::MemOpRecord ConvertToNormalRecord(const MemOpRecordBisheng &bishengRecord)
{
    using namespace Sanitizer;
    auto record = MemOpRecord {};
    record.type = bishengRecord.type;
    record.coreId = bishengRecord.coreId;
    record.moduleId = -1;
    record.srcAddr = static_cast<uint64_t>(bishengRecord.srcAddr);
    record.dstAddr = static_cast<uint64_t>(bishengRecord.dstAddr);
    record.srcSpace = bishengRecord.srcSpace;
    record.dstSpace = bishengRecord.dstSpace;
    record.memSize = bishengRecord.type == Sanitizer::MemOpType::MEMCPY_BLOCKS
        ? static_cast<uint64_t>(CalcMemcpyBlocksSize(bishengRecord))
        : static_cast<uint64_t>(bishengRecord.memSize);
    record.lineNo = bishengRecord.lineNo;
    if (memcpy_s(record.fileName, sizeof(record.fileName),
                 bishengRecord.fileName, sizeof(bishengRecord.fileName)) != EOK) {
        return record;
    }

    if (bishengRecord.type == Sanitizer::MemOpType::MEMCPY_BLOCKS) {
        /// check for stride and blockNum
        if (bishengRecord.srcStride < bishengRecord.blockNum) {
            PrintIllegalStrideError(bishengRecord, "srcStride", bishengRecord.srcStride);
        }
        if (bishengRecord.dstStride < bishengRecord.blockNum) {
            PrintIllegalStrideError(bishengRecord, "dstStride", bishengRecord.dstStride);
        }
    }

    return record;
}

inline bool ReadSockPathFromEnv(std::string &socketPath)
{
    char const *env = getenv("MSSANITIZER_PID");
    socketPath = (env == nullptr) ? "" : std::string(env);
    if (socketPath.empty()) {
        return false;
    }
    return true;
}

// 连接服务端，更新使能模式
bool InitClient()
{
    // 返回失败前需把g_client置为空
    if (g_client) {
        return true;
    }

    std::string socketPath;
    if (!ReadSockPathFromEnv(socketPath)) {
        return false;
    }

    g_client = std::make_shared<CommunicationClient>(socketPath);
    Result result = g_client->ConnectToServer();
    if (result.Fail()) {
        g_client = nullptr;
        return false;
    }

    // 接收使能模式信息
    std::string msg;
    constexpr uint32_t maxCount = 10U;
    for (uint32_t i = 0U; i < maxCount; ++i) {
        if (msg.size() >= sizeof(Config)) {
            break;
        }
        std::string temp;
        result = g_client->Read(temp);
        if (!result.Fail()) {
            msg += temp;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100U));
    }

    // 根据协议，从信息头中读取mode
    if (!Deserialize<Config>(msg, g_config)) {
        g_client = nullptr;
        return false;
    }
    return true;
}

bool IsStubEnabled(const StubType &type)
{
    if (!g_client) {
        if (!InitClient()) {
            return false;
        }
    }
    auto it = STUB_ENABLE_MAP.find(type);
    if (it == STUB_ENABLE_MAP.end()) {
        return false;
    }
    return g_config.defaultCheck;
}

DeviceType ConvertStringToDeviceType(const std::string &chipInfo)
{
    auto it = SOC_VERSION_MAP.find(chipInfo);
    if (it == SOC_VERSION_MAP.end()) {
        return DeviceType::INVALID;
    }
    return it->second;
}

MemOpRecord CreateRecord(uint64_t addr, uint32_t memSpace, const char *fileName, uint64_t lineNo)
{
    if (fileName == nullptr) {
        return MemOpRecord {};
    }
    std::string path(fileName);
    std::string baseFilename = path.substr(path.find_last_of("/\\") + 1);
    auto record = MemOpRecord {};
    record.coreId = -1; // do not care
    record.srcAddr = 0;
    record.srcSpace = AddressSpace::INVALID;
    record.dstAddr = addr;
    record.dstSpace = ConvertAddrSpaceHostToKernel(memSpace);
    record.type = MemOpType::INVALID;
    record.memSize = 0;
    record.moduleId = -1;
    record.lineNo = static_cast<int32_t>(lineNo);
    if (memset_s(&record.fileName[0], sizeof(record.fileName), 0, sizeof(record.fileName)) != EOK) {
        return record;
    }
    if (memcpy_s(&record.fileName[0], sizeof(record.fileName) - 1U,
                 baseFilename.c_str(), strlen(baseFilename.c_str())) != EOK) {
        return record;
    }
    std::size_t length = std::min(strlen(baseFilename.c_str()), sizeof(record.fileName) - 1);
    record.fileName[length] = '\0';
    return record;
}

void SendPacketType(const PacketType &type)
{
    PacketHead head;
    head.type = type;
    g_client->Write(Serialize<PacketHead>(head));
}

Result SendHeaderInfo(uint8_t *addr)
{
    std::string deviceSummaryMsg(reinterpret_cast<const char*>(addr), sizeof(DeviceInfoSummary));
    DeviceInfoSummary summary{};
    (void)Deserialize<DeviceInfoSummary>(deviceSummaryMsg, summary);
    summary.device = g_deviceType;
    summary.blockNum += 1U; // 增加host block
    SendPacketType(PacketType::DEVICE_SUMMARY);
    Result result = g_client->Write(Serialize<DeviceInfoSummary>(summary));
    return result;
}

Result SendHostMalloc()
{
    Result result {};
    std::unique_lock<std::mutex> lk(g_mtx);
    for (auto &record : g_hostRecords) {
        SendPacketType(PacketType::SANITIZER_RECORD);
        Sanitizer::SanitizerRecord sanitizerRecord {};
        sanitizerRecord.version = Sanitizer::RecordVersion::MEMORY_RECORD;
        sanitizerRecord.payload.memoryRecord = record;
        result = g_client->Write(Serialize<Sanitizer::SanitizerRecord>(sanitizerRecord));
        if (result.Fail()) {
            return result;
        }
    }
    return result;
}

}

// 进入核函数前触发，获取device型号
void AscendCheckInit(const char *chipInfo)
{
    if (!IsStubEnabled(StubType::ASCEND_CHECK_INIT)) {
        return;
    }
    g_deviceType = ConvertStringToDeviceType(std::string(chipInfo));
    return;
}

namespace {
bool IsSendInfoFail(uint8_t *addr)
{
    if (addr == nullptr) {
        std::cout << "addr is nullptr" << std::endl;
        return true;
    }
    // 1. 发送信息头
    Result result = SendHeaderInfo(addr);
    if (result.Fail()) {
        std::cout << "SendHeaderInfo result failed" << std::endl;
        return true;
    }

    // 2. 发送host侧malloc记录信息
    result = SendHostMalloc();
    if (result.Fail()) {
        std::cout << "SendHostMalloc result failed" << std::endl;
        return true;
    }
    return false;
}
}  // namespace

// 离开核函数后触发，获取device侧记录的信息，发送device侧与host侧记录信息回工具server端
void AscendCheckExit(uint8_t *addr, uint64_t size)
{
    if (addr == nullptr || !IsStubEnabled(StubType::ASCEND_CHECK_EXIT)) {
        return;
    }

    if (IsSendInfoFail(addr)) {
        return;
    }

    auto deviceInfoSummary = DeviceInfoSummary {};
    if (memcpy_s(&deviceInfoSummary, sizeof(DeviceInfoSummary), addr, sizeof(DeviceInfoSummary)) != EOK) {
        return;
    }
    uint64_t curSize = 0;
    curSize += sizeof(DeviceInfoSummary);
    addr = addr + sizeof(DeviceInfoSummary);
    for (uint32_t i = 0U; i < deviceInfoSummary.blockNum; ++i) {
        auto tempAddr = addr;
        auto blockSummary = BlockSummary {};
        if (memcpy_s(&blockSummary, sizeof(BlockSummary), tempAddr, sizeof(BlockSummary)) != EOK) {
            return;
        }
        tempAddr = tempAddr + sizeof(BlockSummary);
        for (uint32_t j = 0U; j < blockSummary.validSize / sizeof(MemOpRecordBisheng); ++j) {
            auto record = MemOpRecordBisheng {};
            if (memcpy_s(&record, sizeof(MemOpRecordBisheng), tempAddr, sizeof(MemOpRecordBisheng)) != EOK) {
                return;
            }
            SendPacketType(PacketType::SANITIZER_RECORD);
            auto sanitizerRecord = Sanitizer::SanitizerRecord {};
            sanitizerRecord.version = Sanitizer::RecordVersion::MEMORY_RECORD;
            // 转换bishengRecord为通用record后发送
            sanitizerRecord.payload.memoryRecord = ConvertToNormalRecord(record);
            g_client->Write(Serialize<Sanitizer::SanitizerRecord>(sanitizerRecord));
            tempAddr = tempAddr + sizeof(MemOpRecordBisheng);
        }
        uint64_t offset = deviceInfoSummary.blockSize + sizeof(BlockSummary);
        addr = addr + offset;
        curSize += offset;
        if (curSize > size) {
            std::cout << " the addr current offset is " << curSize
                << ", but the addr size is " <<  size
                << " which beyond the maximum size" << std::endl;
            return;
        }
    }
}

// malloc_device/malloc_host/malloc_shared/get_access触发，记录malloc信息
void AscendCheckMalloc(uint64_t addr, uint32_t memSpace, uint64_t len, const char *fileName, uint64_t lineNo)
{
    if (!IsStubEnabled(StubType::ASCEND_CHECK_MALLOC)) {
        return;
    }
    MemOpRecord record = CreateRecord(addr, memSpace, fileName, lineNo);
    record.type = MemOpType::MALLOC;
    record.memSize = len;
    {
        std::unique_lock<std::mutex> lk(g_mtx);
        g_hostRecords.emplace_back(record);
    }
}

// free触发，记录并发送free信息
void AscendCheckFree(uint64_t addr, uint32_t memSpace, const char *fileName, uint64_t lineNo)
{
    if (!IsStubEnabled(StubType::ASCEND_CHECK_FREE)) {
        return;
    }
    if (g_deviceType == DeviceType::INVALID) {
        return;
    }
    if (g_client) {
        MemOpRecord record = CreateRecord(addr, memSpace, fileName, lineNo);
        record.type = MemOpType::FREE;
        SendPacketType(PacketType::SANITIZER_RECORD);
        auto sanitizerRecord = Sanitizer::SanitizerRecord {};
        sanitizerRecord.version = Sanitizer::RecordVersion::MEMORY_RECORD;
        sanitizerRecord.payload.memoryRecord = record;
        g_client->Write(Serialize<Sanitizer::SanitizerRecord>(sanitizerRecord));
        std::unique_lock<std::mutex> lk(g_mtx);
        g_hostRecords.emplace_back(record);
    }
}
