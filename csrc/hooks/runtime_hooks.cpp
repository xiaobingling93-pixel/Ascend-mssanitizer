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


#include "hook_logger.h"
#include "runtime.h"

#include <cstdint>
#include <elf.h>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <iterator>
#include <unordered_map>

#include "record_defs.h"
#include "hook_report.h"
#include "handle_mapping.h"
#include "constant.h"
#include "utility/elf_loader.h"
#include "utility/umask_guard.h"
#include "utility/ustring.h"
#include "vallina_symbol.h"
#include "utility/serializer.h"
#include "utility/file_system.h"
#include "utility/cmd.h"
#include "utility/type_traits.h"
#include "utility/types.h"

using namespace Sanitizer;

#if defined(BUILD_TRACEKIT)
#define KERNEL_LAUNCH_INIT __mstrace_init
#define KERNEL_LAUNCH_FINALIZE __mstrace_finalize
#else
#define KERNEL_LAUNCH_INIT __sanitizer_init
#define KERNEL_LAUNCH_FINALIZE __sanitizer_finalize
#endif

extern "C" {
uint8_t* KERNEL_LAUNCH_INIT(uint64_t blockDim);
void KERNEL_LAUNCH_FINALIZE(uint8_t *memInfo, uint64_t blockDim);
}

namespace {

constexpr uint64_t MAX_BINARY_SIZE = 32ULL * 1024 * 1024 * 1024; // 32GB
constexpr uint32_t MAX_FILE_MAPPING_BUFF_SIZE = 1024U * 1024 * 1024; // 1GB

struct RuntimeLibLoader {
    static void *Load(void)
    {
        return RuntimeLibLoad("libruntime.so");
    }
};

struct OpMemInfo {
    // 内存信息<地址,长度>
    std::vector<std::pair<uint64_t, uint64_t>> memInfos;
    // 二级指针<index, <index, length>>
    std::map<uint64_t, std::vector<std::pair<uint64_t, uint64_t>>> secondPtrInfoMap;
    // 存储按照地址去重后的<addr,length>，防止double malloc 和 double free
    std::vector<std::pair<uint64_t, uint64_t>> uniqueMemInfos;
    uint32_t inputNum;
    uint32_t skipNum;
    bool hasShapeInfo = true;
};

enum class PtrType : uint64_t {
    PRIMARY = 0U,
    SECONDARY,
    SECONDARY_SHAPE
};
// 约束: 依赖流同步来保证rtSetExceptionExtInfo和rtKernelLaunchxxx函数串行调用
static OpMemInfo g_opMemInfo;

std::string &GetRtKernelLogBuff()
{
    static std::string buff;
    return buff;
}

using RuntimeSymbol = VallinaSymbol<RuntimeLibLoader>;

using namespace Sanitizer;

void ReportKernelBinaryByHandle(const void *hdl)
{
    auto it = as_const(HandleMapping::GetInstance().handleBinKernelMap_).find(hdl);
    if (it == HandleMapping::GetInstance().handleBinKernelMap_.cend()) {
        HOOK_LOG("kernel binary NOT registered");
        return;
    }

    HookReport::Instance().ReportKernelBinary(it->second.bin);
    HookReport::Instance().SetKernelType(it->second.kernelType);
}

void ReportKernelBinaryByStub(const void *stub)
{
    auto it = as_const(HandleMapping::GetInstance().stubHandleMap_).find(stub);
    if (it == HandleMapping::GetInstance().stubHandleMap_.cend()) {
        HOOK_LOG("stub function NOT registered");
        return;
    }

    ReportKernelBinaryByHandle(it->second);
}

using HandlePtr = TaggedType<void const *, class HandleTag>;
using StubPtr = TaggedType<void const *, class StubTag>;

bool GetSectionHeaders(HandlePtr hdl, std::map<std::string, Elf64_Shdr> &headers)
{
    auto it = as_const(HandleMapping::GetInstance().handleBinKernelMap_).find(hdl.value);
    if (it == HandleMapping::GetInstance().handleBinKernelMap_.cend()) {
        HOOK_LOG("kernel binary NOT registered");
        return false;
    }

    ElfLoader loader{};
    if (!loader.FromBuffer(it->second.bin)) {
        HOOK_LOG("parse elf section from kernel binary FAILED");
        return false;
    }
    Elf elf = loader.Load();
    headers = elf.GetSectionHeaders();
    return true;
}

bool GetSectionHeaders(StubPtr stub, std::map<std::string, Elf64_Shdr> &headers)
{
    auto it = as_const(HandleMapping::GetInstance().stubHandleMap_).find(stub.value);
    if (it == HandleMapping::GetInstance().stubHandleMap_.cend()) {
        HOOK_LOG("stub function NOT registered");
        return false;
    }

    return GetSectionHeaders(HandlePtr{it->second}, headers);
}

template <typename Ptr>
std::vector<Elf64_Shdr> GetAllocSectionHeaders(Ptr ptr)
{
    std::vector<Elf64_Shdr> allocHeaders;
    std::map<std::string, Elf64_Shdr> headers;
    if (!GetSectionHeaders(ptr, headers)) {
        return allocHeaders;
    }

    for (auto const &headerPair : headers) {
        // SHF_ALLOC 标志位为 1 说明该 section 在运行时的 GM 上需要分配对应的内存，
        // 则工具将对应的内存也设置为已分配状态
        Elf64_Shdr const &header = headerPair.second;
        if (header.sh_flags & SHF_ALLOC) {
            HOOK_LOG("need alloc section %s, addr: %llu, size: %llu",
                     headerPair.first.c_str(), header.sh_addr, header.sh_size);
            allocHeaders.emplace_back(header);
        }
    }
    return allocHeaders;
}

void ReportSectionsMalloc(uint64_t pcStartAddr, std::vector<Elf64_Shdr> const &headers)
{
    HOOK_LOG("%s", GetRtKernelLogBuff().c_str());
    GetRtKernelLogBuff().clear();

    if (pcStartAddr == 0x00) {
        return;
    }

    HookReport &report = HookReport::Instance();
    for (auto const & header : headers) {
        // section 的运行时地址可以由 pcStartAddr + sectionAddr 计算得到
        ReportAddrInfo addrInfo{pcStartAddr + header.sh_addr, header.sh_size, MemInfoSrc::RT};
        report.ReportMalloc(addrInfo);
        // 需要通过 memset 将这片内存标记为已初始化的状态，防止初始化检测产生误报
        report.ReportMemset(addrInfo);
    }
}

void ReportSectionsFree(uint64_t pcStartAddr, std::vector<Elf64_Shdr> const &headers)
{
    if (pcStartAddr == 0x00) {
        return;
    }

    HookReport &report = HookReport::Instance();
    for (auto const & header : headers) {
        ReportAddrInfo addrInfo{pcStartAddr + header.sh_addr, MemInfoSrc::RT};
        report.ReportFree(addrInfo);
    }
}

KernelType MagicToKernelType(uint32_t magic)
{
    KernelType kernelType {};
    if (magic == RT_DEV_BINARY_MAGIC_ELF_AIVEC) {
        kernelType = KernelType::AIVEC;
    } else if (magic == RT_DEV_BINARY_MAGIC_ELF_AICUBE) {
        kernelType = KernelType::AICUBE;
    } else if (magic == RT_DEV_BINARY_MAGIC_ELF) {
        kernelType = KernelType::MIX;
    } else if (magic == RT_DEV_BINARY_MAGIC_ELF_AICPU) {
        kernelType = KernelType::AICPU;
    } else {
        HOOK_LOG("INVALID kernel binary magic number");
    }
    return kernelType;
}

// return true if argument is valid, false otherwise.
bool VerifyArginfo(rtArgsEx_t const &argsInfo)
{
    /// isNoNeedH2DCopy == 1表示走GE整图下发，args的填充和拼接由用户指定，不由rts拼接
    if (argsInfo.isNoNeedH2DCopy == 1) {
        std::cout << "[mssanitizer] isNoNeedH2DCopy = 1 means GE is used to deliver the graph,"
                  << " whose detection is not supported yet" << std::endl;
        return false;
    }
    auto args = argsInfo.args;
    if (args == nullptr) {
        return false;
    }

    if (argsInfo.hostInputInfoPtr != nullptr) {
        auto headSize = argsInfo.hostInputInfoPtr->dataOffset;
        if (headSize > argsInfo.argsSize) {
            HOOK_LOG("Illegal input argument: HeadSize[%u] exceeds argsSize[%u].", headSize, argsInfo.argsSize);
            return false;
        }
    }
    return true;
}

/// argsInfo中args中包含kernel函数入参和tilingData具体的值，argsSize为args长度。hasTiling标识了算子是否包含tilingPtr
/// 算子包含tilingPtr时，args的排列顺序为：input + output + workSpace + tilingPtr + OverFlow addr + tilingData
/// kernel函数入参为：input + output + workSpace + tilingPtr
/// tilingAddrOffset代表tilingPtr的偏移
/// tilingDataOffset代表tilingData的偏移；编译器将记录信息对应的GM地址memInfo插入到了kernel函数入参的末尾
/// 修改后的args排列顺序为：input + output + workSpace + tilingPtr + memInfo + OverFlow addr + tilingData
/// 算子不包含tilingPtr时，args的排列顺序为：input + output + workSpace
/// 修改后的args排列顺序为：input + output + workSpace + memInfo
rtArgsEx_t ProduceRtArgsEx(rtArgsEx_t const &argsInfo, std::vector<uint8_t> &argsWithMemInfo,
    const std::string &memInfoS)
{
    std::ostringstream oss;
    oss << "argsSize:" << argsInfo.argsSize << ", tilingAddrOffset:" << argsInfo.tilingAddrOffset <<
    ", tilingDataOffset:" << argsInfo.tilingDataOffset << ", hostInputInfoNum:" << argsInfo.hostInputInfoNum <<
    ", hasTiling:" << static_cast<uint16_t>(argsInfo.hasTiling) <<
    ", isNoNeedH2DCopy:" << static_cast<uint16_t>(argsInfo.isNoNeedH2DCopy);
    if (!VerifyArginfo(argsInfo)) {
        HOOK_LOG("%s", oss.str());
        return argsInfo;
    }
    rtArgsEx_t newArginfo = argsInfo;
    uint32_t argsSize = argsInfo.argsSize;
    auto args = argsInfo.args;

    argsWithMemInfo = std::vector<uint8_t>(argsInfo.argsSize + memInfoS.size());
    if (argsInfo.hasTiling == 0) {
       /// 没有tiling时，如果算子有host侧输入，则第一个host输入中的dataOffset为kernel函数入参末尾；如果没有host输入，则argSize为kernel函数入参末尾；
        if (newArginfo.hostInputInfoPtr != nullptr) {
            auto headSize = newArginfo.hostInputInfoPtr->dataOffset;
            std::copy_n(static_cast<uint8_t const*>(args), headSize, argsWithMemInfo.begin());
            oss << ", memInfo:" << headSize;
            std::copy_n(memInfoS.cbegin(), memInfoS.size(), argsWithMemInfo.begin() + headSize);
            uint32_t tail = argsSize - headSize;
            std::copy_n(static_cast<uint8_t const*>(args) + headSize, tail,
                        argsWithMemInfo.begin() + headSize + memInfoS.size());
        } else {
            std::copy_n(static_cast<uint8_t const*>(args), argsSize, argsWithMemInfo.begin());
            oss << ", memInfo:" << argsSize;
            std::copy_n(memInfoS.cbegin(), memInfoS.size(), argsWithMemInfo.begin() + argsSize);
        }
    } else {
        auto tilingAddrOffset = argsInfo.tilingAddrOffset;
        constexpr uint8_t tilingPtrSize = 8U;
        auto headSize = tilingAddrOffset + tilingPtrSize;
        std::copy_n(static_cast<uint8_t const*>(args), headSize, argsWithMemInfo.begin());
        oss << ", memInfo:" << headSize;
        std::copy_n(memInfoS.cbegin(), memInfoS.size(), argsWithMemInfo.begin() + headSize);
        uint32_t tail = argsSize - headSize;
        std::copy_n(static_cast<uint8_t const*>(args) + headSize, tail,
                    argsWithMemInfo.begin() + headSize + memInfoS.size());
        newArginfo.tilingDataOffset += memInfoS.size();
    }
    if (newArginfo.hostInputInfoPtr != nullptr) {
        /// 偏移多个host侧输入的dataoffset
        for (uint16_t i = 0; i < newArginfo.hostInputInfoNum; ++i) {
            (newArginfo.hostInputInfoPtr + i)->dataOffset += memInfoS.size();
        }
    }
    newArginfo.args = argsWithMemInfo.data();
    newArginfo.argsSize = argsWithMemInfo.size();
    GetRtKernelLogBuff() += oss.str();
    return newArginfo;
}

// |<-atomic_index(u64)->||<-inputnum(u32) skipnum(u32)->||<-input(u64)->||<-output(u64)->||<-workspace(u64)->|...
void ExtractOpMemInfo(const rtArgsSizeInfo_t * const sizeInfo, OpMemInfo &opMemInfo)
{
    if (sizeInfo == nullptr || sizeInfo->infoAddr == nullptr) {
        return;
    }
    uint64_t *buff = static_cast<uint64_t *>(sizeInfo->infoAddr);

    // input&skip信息偏移为1, 低32bit为input，高32bit为skip
    opMemInfo.inputNum = *(buff + 1U) & 0xffffffff;
    opMemInfo.skipNum = (*(buff + 1U) >> 32U) & 0xffffffff;

    GetRtKernelLogBuff() += "inputNum:" + std::to_string(opMemInfo.inputNum) +
                     ", skipNum:" + std::to_string(opMemInfo.skipNum) + ", ";

    // 二级指针场景解析
    uint64_t rightOperand = 56U;
    uint64_t inputOffset = 0U;
    for (uint64_t i = 0U; i < opMemInfo.inputNum; ++i) {
        // 高8位存放指针类型信息，0表示1级指针，1表示无shape信息的二级指针，2表示带shape信息的二级指针
        uint64_t ptrType = (*(buff + 2U + i + inputOffset) >> rightOperand) & 0xff;
        // 低56位表示一级指针的个数
        uint64_t primaryPtrNum = *(buff + 2U + i + inputOffset) & 0xffffffffffffff;
        if (ptrType == static_cast<uint64_t>(PtrType::SECONDARY_SHAPE)) {
            opMemInfo.memInfos.emplace_back(std::make_pair(i, 0));
            for (uint64_t j = 0U; j < primaryPtrNum; j++) {
                // size信息偏移为2, length信息为当前参数index+1
                uint64_t inputSize = *(buff + 3U + i + j + inputOffset);
                opMemInfo.secondPtrInfoMap[i].emplace_back(std::make_pair(i + j + inputOffset + 1U, inputSize));
            }
            inputOffset += primaryPtrNum;
        } else if (ptrType == static_cast<uint64_t>(PtrType::SECONDARY)) {
            HOOK_LOG("Kernel shape is null, will be set 0");
            opMemInfo.hasShapeInfo = false;
            opMemInfo.secondPtrInfoMap[i].emplace_back(std::make_pair(i, 0U));
        } else if (ptrType == static_cast<uint64_t>(PtrType::PRIMARY)) {
            // size信息偏移为2
            opMemInfo.memInfos.emplace_back(std::make_pair(0U, *(buff + 2 + i + inputOffset)));
        } else {
            HOOK_LOG("Ptr type value is invalid, should be 0,1,2");
            opMemInfo.memInfos.emplace_back(std::make_pair(0U, 0U));
        }
    }
}

void GetSecondPtrInfo(const rtArgsEx_t * const argsInfo, OpMemInfo &opMemInfo,
                      std::unordered_map<uint64_t, uint64_t> &addrMap, const uint32_t index,
                      std::vector<uint64_t> &order)
{
    uint64_t *buff = static_cast<uint64_t *>(argsInfo->args);
    for (auto i = 0; i < argsInfo->hostInputInfoNum; i++) {
        uint64_t hostIndex = (argsInfo->hostInputInfoPtr + i)->addrOffset / 8U - 1U;
        if (hostIndex != uint64_t(index)) {continue;}
        if (opMemInfo.secondPtrInfoMap.find(hostIndex) != opMemInfo.secondPtrInfoMap.end()) {
            uint64_t *dynamicInputsPtr = buff + (argsInfo->hostInputInfoPtr + i)->dataOffset / 8U;
            uint64_t ptrOffset = *dynamicInputsPtr;
            // no shape info
            if (!opMemInfo.hasShapeInfo) {
                uint64_t dynamicInputsAddr = reinterpret_cast<uint64_t>(dynamicInputsPtr);
                addrMap[dynamicInputsAddr] = 0;
                order.push_back(dynamicInputsAddr);
                continue;
            }
            auto const firstInfoVec = opMemInfo.secondPtrInfoMap.at(hostIndex);
            uint64_t ptrIndex = 0U;
            for (auto it : firstInfoVec) {
                uint64_t* inputPtr = dynamicInputsPtr + ptrOffset + ptrIndex;
                uint64_t inputAddr = reinterpret_cast<uint64_t>(inputPtr);
                addrMap[inputAddr] = std::max(addrMap[inputAddr], it.second);
                order.push_back(inputAddr);
                ptrIndex++;
            }
        }
    }
}

void ReportOpMallocInfo(const rtArgsEx_t * const argsInfo, OpMemInfo &opMemInfo)
{
    // 设置临时数组记录顺序，临时map用于去重，目的是保持去重后顺序一致
    std::vector<uint64_t> order;
    std::unordered_map<uint64_t, uint64_t> addrMap;
    if (argsInfo == nullptr || argsInfo->args == nullptr) {
        return;
    }
    uint64_t *buff = static_cast<uint64_t *>(argsInfo->args);
    uint32_t index = 0U;
    for (auto &it : opMemInfo.memInfos) {
        if (opMemInfo.secondPtrInfoMap.find(index) != opMemInfo.secondPtrInfoMap.end()) {
            if (argsInfo->hostInputInfoPtr == nullptr) {
                HOOK_LOG("Host input ptr is null, the length will be set 0");
                it.first = *(buff + opMemInfo.skipNum + index);
                order.push_back(it.first);
                addrMap[it.first] = 0U;
            } else {
                GetSecondPtrInfo(argsInfo, opMemInfo, addrMap, index, order);
            }
            index++;
            continue;
        }
        it.first = *(buff + opMemInfo.skipNum + index);
        if (addrMap.find(it.first) == addrMap.end()) {
            order.push_back(it.first);
        }
        // 部分算子存在内存复用情况，输入和输出为相同地址，这里计算相同地址的最大长度作为malloc上报时的长度
        addrMap[it.first] = std::max(addrMap[it.first], it.second);
        index++;
    }
    for (uint64_t address : order) {
        opMemInfo.uniqueMemInfos.push_back(std::make_pair(address, addrMap[address]));
    }

    for (const auto &it : opMemInfo.uniqueMemInfos) {
        HookReport::Instance().ReportMalloc(it.first, it.second, MemInfoSrc::EXTRA);
        // 内存分配是通过获取tensor信息模拟的，无实际的桩函数记录，因此需要设为STORE，防止未初始化读误报
        HookReport::Instance().ReportStore(it.first, it.second, MemInfoSrc::EXTRA);
    }

    // 上报tiling地址信息
    if (argsInfo->hasTiling == 0U) {
        return;
    }
    uint64_t tilingAddr = *(buff + argsInfo->tilingAddrOffset / 8);
    uint32_t tilingSize = argsInfo->argsSize - argsInfo->tilingDataOffset;

    // 输入输出tensor有两种上报路径，如果不通过setExeptionExtInfo上报，那么通过rtMalloc拿到tensor信息
    // tilling信息总是通过RtKernelLaunchWithHandleV2获取，此时需要把tillin信息上报来源设置为RTDEVICE
    // 和tensor信息保持一致
    MemInfoSrc memInfoSrc = opMemInfo.uniqueMemInfos.empty() ? MemInfoSrc::RT : MemInfoSrc::EXTRA;
    HOOK_LOG("tilingAddr:0x%llx, size:%lu", tilingAddr, tilingSize);
    HookReport::Instance().ReportMalloc(tilingAddr, tilingSize, memInfoSrc);
    // 内存分配是通过获取tilling信息模拟的，无实际的桩函数记录，因此需要设为STORE，防止未初始化读误报
    HookReport::Instance().ReportStore(tilingAddr, tilingSize, memInfoSrc);
    opMemInfo.uniqueMemInfos.push_back(std::make_pair(tilingAddr, tilingSize));
}

void ReportOpFreeInfo(OpMemInfo &opMemInfo)
{
    for (const auto &it : opMemInfo.uniqueMemInfos) {
        HookReport::Instance().ReportFree(it.first, MemInfoSrc::EXTRA);
    }

    opMemInfo.memInfos.clear();
    opMemInfo.uniqueMemInfos.clear();
    return;
}

void ReportOperatorMemInfo(const rtArgsEx_t * const arginfo, uint8_t *memInfo, uint32_t blockDim)
{
    ReportOpMallocInfo(arginfo, g_opMemInfo);
    KERNEL_LAUNCH_FINALIZE(memInfo, blockDim);
    ReportOpFreeInfo(g_opMemInfo);
}

bool ParseTilingKeyLine(std::string const &line, uint64_t &tilingKey)
{
    std::vector<std::string> items;
    Utility::Split(line, std::back_inserter(items), " ");
    if (items.size() < 5UL) {
        return false;
    }
    constexpr std::size_t scopeIdx = 1UL;
    constexpr std::size_t symbolKindIdx = 2UL;
    if (items[scopeIdx] != "g" || items[symbolKindIdx] != "F") {
        return false;
    }

    constexpr std::size_t kernelNameIdx = 4UL;
    std::string kernelName = items[kernelNameIdx];
    if (Utility::EndWith(kernelName, "_mix_aic") ||
        Utility::EndWith(kernelName, "_mix_aiv")) {
        kernelName = kernelName.substr(0UL, kernelName.length() - 8UL);
    }

    items.clear();
    Utility::Split(kernelName, std::back_inserter(items), "_");
    if (items.size() < 2UL) {
        return false;
    }
    std::string kernelNamePrefix = items[0] + "_" + items[1];
    GetRtKernelLogBuff() = "kernelName:" + kernelNamePrefix + " " + GetRtKernelLogBuff();
    std::stringstream ss(items[items.size() - 1UL]);
    ss >> tilingKey;
    return true;
}

bool ParseFirstTilingKey(std::string const &output, uint64_t &tilingKey)
{
    std::vector<std::string> lines;
    Utility::Split(output, std::back_inserter(lines), "\n");

    // skip headers
    auto it = lines.cbegin();
    for (; it != lines.cend(); ++it) {
        if (it->find("SYMBOL TABLE:") != std::string::npos) {
            break;
        }
    }

    if (it == lines.cend()) {
        return false;
    }

    ++it;
    for (; it != lines.cend(); ++it) {
        if (ParseTilingKeyLine(*it, tilingKey)) {
            return true;
        }
    }
    return false;
}

bool GetKernelFirstTilingKey(std::vector<char> const &binary, uint64_t &tilingKey)
{
    std::string kernelPath = "./kernel.o." + std::to_string(getpid());
    {
        UmaskGuard umaskGuard(REGULAR_MODE_MASK);
        if (!WriteBinary(kernelPath, binary.data(), binary.size())) {
            return false;
        }
    }

    std::vector<std::string> cmd = {
        "llvm-objdump",
        "-t",
        kernelPath
    };
    std::string output;
    bool ret = PipeCall(cmd, output) && ParseFirstTilingKey(output, tilingKey);
    remove(kernelPath.c_str());
    return ret;
}

bool GetPcStartAddrStatic(void const *stubFunc, uint64_t &pcStartAddr)
{
    void *pcStart = nullptr;
    uint32_t prefetchCnt{};

    rtError_t ret = rtKernelGetAddrAndPrefCnt(nullptr, 0UL, stubFunc, 0U, &pcStart, &prefetchCnt);
    if (ret != RT_ERROR_NONE) {
        // reset pcStartAddr to 0x00 if rtKernelGetAddrAndPrefCnt query failed
        pcStartAddr = 0x00;
        return false;
    }
    pcStartAddr = reinterpret_cast<uint64_t>(pcStart);
    return true;
}

bool GetPcStartAddrDynamic(void *hdl, uint64_t &pcStartAddr)
{
    auto it = as_const(HandleMapping::GetInstance().handleBinKernelMap_).find(hdl);
    if (it == HandleMapping::GetInstance().handleBinKernelMap_.cend()) {
        HOOK_LOG("kernel handle NOT registered in map");
        return false;
    }

    uint64_t tilingKey = 0UL;
    if (!GetKernelFirstTilingKey(it->second.bin, tilingKey)) {
        HOOK_LOG("get first tiling key from kernel FAILED");
        return false;
    }
    HOOK_LOG("first tiling key in kernel binary: %lu", tilingKey);

    void *pcStart = nullptr;
    uint32_t prefetchCnt{};
    rtError_t ret = rtKernelGetAddrAndPrefCnt(hdl, tilingKey, nullptr, 1U, &pcStart, &prefetchCnt);
    if (ret != RT_ERROR_NONE) {
        // reset pcStartAddr to 0x00 if rtKernelGetAddrAndPrefCnt query failed
        pcStartAddr = 0x00;
        return false;
    }
    pcStartAddr = reinterpret_cast<uint64_t>(pcStart);
    return true;
}

}  // namespace Dummy

RTS_API rtError_t rtMalloc(void **devPtr, uint64_t size, rtMemType_t type,
                           const uint16_t moduleId)
{
    using RtMalloc = decltype(&rtMalloc);
    auto vallina = RuntimeSymbol::Instance().Get<RtMalloc>(__func__);
    if (vallina == nullptr) {
        HOOK_LOG("vallina func get FAILED");
        return RT_ERROR_RESERVED;
    }

    rtError_t ret = vallina(devPtr, size, type, moduleId);
    if (ret != RT_ERROR_NONE) {
        return ret;
    }

    uint64_t addr = reinterpret_cast<uint64_t>(*devPtr);
    HookReport::Instance().ReportMalloc(addr, size, MemInfoSrc::RT);

    return ret;
}

RTS_API rtError_t rtFree(void *devPtr)
{
    uint64_t addr = reinterpret_cast<uint64_t>(devPtr);
    HookReport::Instance().ReportFree(addr, MemInfoSrc::RT);

    using RtFree = decltype(&rtFree);
    auto vallina = RuntimeSymbol::Instance().Get<RtFree>(__func__);
    if (vallina == nullptr) {
        HOOK_LOG("vallina func get FAILED");
        return RT_ERROR_RESERVED;
    }

    return vallina(devPtr);
}

RTS_API rtError_t rtMemset(void *devPtr, uint64_t destMax, uint32_t val, uint64_t cnt)
{
    using RtMemset = decltype(&rtMemset);
    auto vallina = RuntimeSymbol::Instance().Get<RtMemset>(__func__);
    if (vallina == nullptr) {
        HOOK_LOG("vallina func get FAILED");
        return RT_ERROR_RESERVED;
    }

    rtError_t ret = vallina(devPtr, destMax, val, cnt);
    // 失败不做返回，仍上报，用于检测问题

    uint64_t addr = reinterpret_cast<uint64_t>(devPtr);
    HookReport::Instance().ReportStore(addr, cnt, MemInfoSrc::RT);

    return ret;
}

RTS_API rtError_t rtMemcpy(void *dst, uint64_t destMax, const void *src, uint64_t cnt, rtMemcpyKind_t kind)
{
    using RtMemcpy = decltype(&rtMemcpy);
    auto vallina = RuntimeSymbol::Instance().Get<RtMemcpy>(__func__);
    if (vallina == nullptr) {
        HOOK_LOG("vallina func get FAILED");
        return RT_ERROR_RESERVED;
    }

    rtError_t ret = vallina(dst, destMax, src, cnt, kind);
    // 失败不做返回，仍上报，用于检测问题

    uint64_t dstAddr = reinterpret_cast<uint64_t>(dst);
    uint64_t srcAddr = reinterpret_cast<uint64_t>(src);
    if (kind == RT_MEMCPY_HOST_TO_DEVICE || kind == RT_MEMCPY_DEVICE_TO_DEVICE) {
        HookReport::Instance().ReportStore(dstAddr, cnt, MemInfoSrc::RT);
    }
    if (kind == RT_MEMCPY_DEVICE_TO_HOST || kind == RT_MEMCPY_DEVICE_TO_DEVICE) {
        HookReport::Instance().ReportLoad(srcAddr, cnt, MemInfoSrc::RT);
    }

    return ret;
}

/// 物理内存和虚拟内存映射函数，后续程序使用的地址为映射之后的虚拟地址，故劫持映射函数得到的虚拟地址作为malloc上报的地址
RTS_API rtError_t rtMapMem(void *devPtr, size_t size, size_t offset, rtDrvMemHandle_t *handle, uint64_t flags)
{
    using RtMapMem = decltype(&rtMapMem);
    auto vallina = RuntimeSymbol::Instance().Get<RtMapMem>(__func__);
    if (vallina == nullptr) {
        HOOK_LOG("vallina func get FAILED");
        return RT_ERROR_RESERVED;
    }

    rtError_t ret = vallina(devPtr, size, offset, handle, flags);
    // 失败不做返回，仍上报，用于检测问题

    uint64_t addr = reinterpret_cast<uint64_t>(devPtr);
    HookReport::Instance().ReportMalloc(addr, size, MemInfoSrc::RT);

    return ret;
}

RTS_API rtError_t rtUnmapMem(void *devPtr)
{
    uint64_t addr = reinterpret_cast<uint64_t>(devPtr);
    HookReport::Instance().ReportFree(addr, MemInfoSrc::RT);

    using RtUnmapMem = decltype(&rtUnmapMem);
    auto vallina = RuntimeSymbol::Instance().Get<RtUnmapMem>(__func__);
    if (vallina == nullptr) {
        HOOK_LOG("vallina func get FAILED");
        return RT_ERROR_RESERVED;
    }

    return vallina(devPtr);
}

RTS_API rtError_t rtDevBinaryRegister(const rtDevBinary_t *bin, void **hdl)
{
    using RtDevBinaryRegister = decltype(&rtDevBinaryRegister);
    auto vallina = RuntimeSymbol::Instance().Get<RtDevBinaryRegister>(__func__);
    if (vallina == nullptr) {
        HOOK_LOG("vallina func get FAILED");
        return RT_ERROR_RESERVED;
    }

    rtError_t result = vallina(bin, hdl);
    if (result == RT_ERROR_NONE && bin != nullptr && bin->data != nullptr && hdl != nullptr) {
        // register handle bin map
        if (bin->length > MAX_BINARY_SIZE) {
            HOOK_LOG("Illegal binary size: binary size[%u] exceeds max binary size[%u].",
                bin->length, MAX_BINARY_SIZE);
            return RT_ERROR_MEMORY_ALLOCATION ;
        }
        auto binData = static_cast<char const *>(bin->data);
        BinKernel binKernel {};
        binKernel.bin = std::vector<char>(binData, binData + bin->length);
        binKernel.kernelType = MagicToKernelType(bin->magic);
        HandleMapping::GetInstance().handleBinKernelMap_[*hdl] = std::move(binKernel);
    }
    return result;
}

RTS_API rtError_t rtRegisterAllKernel(const rtDevBinary_t *bin, void **hdl)
{
    using RtRegisterAllKernel = decltype(&rtRegisterAllKernel);
    auto vallina = RuntimeSymbol::Instance().Get<RtRegisterAllKernel>(__func__);
    if (vallina == nullptr) {
        HOOK_LOG("vallina func get FAILED");
        return RT_ERROR_RESERVED;
    }

    rtError_t result = vallina(bin, hdl);
    if (result == RT_ERROR_NONE && bin != nullptr && bin->data != nullptr && hdl != nullptr) {
        // register handle bin map
        if (bin->length > MAX_BINARY_SIZE) {
            HOOK_LOG("Illegal binary size: binary size[%u] exceeds max binary size[%u].",
                bin->length, MAX_BINARY_SIZE);
            return RT_ERROR_MEMORY_ALLOCATION ;
        }
        auto binData = static_cast<char const *>(bin->data);
        BinKernel binKernel {};
        binKernel.bin = std::vector<char>(binData, binData + bin->length);
        binKernel.kernelType = MagicToKernelType(bin->magic);
        HandleMapping::GetInstance().handleBinKernelMap_[*hdl] = std::move(binKernel);
    }
    return result;
}

RTS_API rtError_t rtDevBinaryUnRegister(void *hdl)
{
    using RtDevBinaryUnRegister = decltype(&rtDevBinaryUnRegister);
    auto vallina = RuntimeSymbol::Instance().Get<RtDevBinaryUnRegister>(__func__);
    if (vallina == nullptr) {
        HOOK_LOG("vallina func get FAILED");
        return RT_ERROR_RESERVED;
    }

    rtError_t result = vallina(hdl);
    if (result == RT_ERROR_NONE) {
        // unregister handle bin map
        auto handleBinIt = as_const(HandleMapping::GetInstance().handleBinKernelMap_).find(hdl);
        if (handleBinIt != HandleMapping::GetInstance().handleBinKernelMap_.cend()) {
            HandleMapping::GetInstance().handleBinKernelMap_.erase(handleBinIt);
        }
        // unregister stub handle map
        for (auto it = as_const(HandleMapping::GetInstance().stubHandleMap_).cbegin();
             it != HandleMapping::GetInstance().stubHandleMap_.cend();) {
            if (it->second == hdl) {
                it = HandleMapping::GetInstance().stubHandleMap_.erase(it);
            } else {
                ++it;
            }
        }
    }
    return result;
}

RTS_API rtError_t rtFunctionRegister(void *binHandle, const void *stubFunc,
                                     const char_t *stubName,
                                     const void *kernelInfoExt,
                                     uint32_t funcMode)
{
    using RtFunctionRegister = decltype(&rtFunctionRegister);
    auto vallina = RuntimeSymbol::Instance().Get<RtFunctionRegister>(__func__);
    if (vallina == nullptr) {
        HOOK_LOG("vallina func get FAILED");
        return RT_ERROR_RESERVED;
    }

    rtError_t result = vallina(binHandle, stubFunc, stubName, kernelInfoExt, funcMode);
    if (result == RT_ERROR_NONE) {
        // record stub handle map
        HandleMapping::GetInstance().stubHandleMap_[stubFunc] = binHandle;
    }
    return result;
}

RTS_API rtError_t rtKernelLaunch(const void *stubFunc, uint32_t blockDim, void *args,
                                 uint32_t argsSize, rtSmDesc_t *smDesc, rtStream_t stm)
{
    HOOK_LOG("rtKernelLaunch enable success");
    using RtKernelLaunch = decltype(&rtKernelLaunch);
    auto vallina = RuntimeSymbol::Instance().Get<RtKernelLaunch>(__func__);
    if (vallina == nullptr) {
        HOOK_LOG("vallina func get FAILED");
        return RT_ERROR_RESERVED;
    }

    /// get pc start addr and save to hook report
    auto &pcStartAddr = HookReport::Instance().pcStartAddr;
    if (!GetPcStartAddrStatic(stubFunc, pcStartAddr)) {
        HOOK_LOG("get pc start addr FAILED");
    }
    ReportKernelBinaryByStub(stubFunc);

    uint8_t *memInfo = KERNEL_LAUNCH_INIT(blockDim);
    HookReport::Instance().SetBlockDim(blockDim);
    HookReport::Instance().ReportKernelInfo();
    /// join args with sanitizer memInfo
    std::string memInfoS = Sanitizer::Serialize(memInfo);
    std::vector<uint8_t> argsWithMemInfo(argsSize + memInfoS.size());
    if (args != nullptr) {
        std::copy_n(static_cast<uint8_t const*>(args), argsSize, argsWithMemInfo.begin());
    }
    GetRtKernelLogBuff() += "argsSize:" + std::to_string(argsSize);
    std::copy_n(memInfoS.cbegin(), memInfoS.size(), argsWithMemInfo.begin() + argsSize);

    rtError_t ret = vallina(stubFunc, blockDim, argsWithMemInfo.data(),
                            argsWithMemInfo.size(), smDesc, stm);
    rtStreamSynchronize(stm);

    std::vector<Elf64_Shdr> headers = GetAllocSectionHeaders(StubPtr{stubFunc});
    ReportSectionsMalloc(pcStartAddr, headers);
    KERNEL_LAUNCH_FINALIZE(memInfo, blockDim);
    ReportSectionsFree(pcStartAddr, headers);

    return ret;
}

RTS_API rtError_t rtKernelLaunchWithHandleV2(void *hdl, const uint64_t tilingKey, uint32_t blockDim,
    rtArgsEx_t *argsInfo, rtSmDesc_t *smDesc, rtStream_t stm, const rtTaskCfgInfo_t *cfgInfo)
{
    HOOK_LOG("rtKernelLaunchWithHandleV2 enable success");
    using RtKernelLaunchWithHandleV2 = decltype(&rtKernelLaunchWithHandleV2);
    auto vallina = RuntimeSymbol::Instance().Get<RtKernelLaunchWithHandleV2>(__func__);
    if (vallina == nullptr) {
        HOOK_LOG("vallina func get FAILED");
        return RT_ERROR_RESERVED;
    }

    /// get pc start addr and save to hook report
    auto &pcStartAddr = HookReport::Instance().pcStartAddr;
    if (!GetPcStartAddrDynamic(hdl, pcStartAddr)) {
        HOOK_LOG("get pc start addr FAILED");
    }
    ReportKernelBinaryByHandle(hdl);

    uint8_t *memInfo = KERNEL_LAUNCH_INIT(blockDim);
    HookReport::Instance().SetBlockDim(blockDim);
    HookReport::Instance().ReportKernelInfo();
    /// join args with sanitizer memInfo
    rtArgsEx_t newArginfo;
    std::vector<uint8_t> argsWithMemInfo;
    if (argsInfo != nullptr) {
        std::string memInfoS = Sanitizer::Serialize(memInfo);
        newArginfo = ProduceRtArgsEx(*argsInfo, argsWithMemInfo, memInfoS);
        argsInfo = &newArginfo;
    }

    rtError_t ret = vallina(hdl, tilingKey, blockDim, argsInfo, smDesc, stm, cfgInfo);
    rtStreamSynchronize(stm);

    std::vector<Elf64_Shdr> headers = GetAllocSectionHeaders(HandlePtr{hdl});
    ReportSectionsMalloc(pcStartAddr, headers);
    ReportOperatorMemInfo(argsInfo, memInfo, blockDim);
    ReportSectionsFree(pcStartAddr, headers);
    return ret;
}

RTS_API rtError_t rtKernelLaunchWithFlagV2(const void *stubFunc, uint32_t blockDim, rtArgsEx_t *argsInfo,
    rtSmDesc_t *smDesc, rtStream_t stm, uint32_t flags, const rtTaskCfgInfo_t *cfgInfo)
{
    HOOK_LOG("rtKernelLaunchWithFlagV2 enable success");
    using RtKernelLaunchWithFlagV2 = decltype(&rtKernelLaunchWithFlagV2);
    auto vallina = RuntimeSymbol::Instance().Get<RtKernelLaunchWithFlagV2>(__func__);
    if (vallina == nullptr) {
        HOOK_LOG("vallina func get FAILED");
        return RT_ERROR_RESERVED;
    }

    /// get pc start addr and save to hook report
    auto &pcStartAddr = HookReport::Instance().pcStartAddr;
    if (!GetPcStartAddrStatic(stubFunc, pcStartAddr)) {
        HOOK_LOG("get pc start addr FAILED");
    }
    ReportKernelBinaryByStub(stubFunc);

    uint8_t *memInfo = KERNEL_LAUNCH_INIT(blockDim);
    HookReport::Instance().SetBlockDim(blockDim);
    HookReport::Instance().ReportKernelInfo();
    rtArgsEx_t newArginfo;
    std::vector<uint8_t> argsWithMemInfo;
    if (argsInfo != nullptr) {
        std::string memInfoS = Sanitizer::Serialize(memInfo);
        newArginfo = ProduceRtArgsEx(*argsInfo, argsWithMemInfo, memInfoS);
        argsInfo = &newArginfo;
    }
    rtError_t ret = vallina(stubFunc, blockDim, argsInfo, smDesc, stm, flags, cfgInfo);
    rtStreamSynchronize(stm);

    std::vector<Elf64_Shdr> headers = GetAllocSectionHeaders(StubPtr{stubFunc});
    ReportSectionsMalloc(pcStartAddr, headers);
    ReportOperatorMemInfo(argsInfo, memInfo, blockDim);
    ReportSectionsFree(pcStartAddr, headers);
    return ret;
}

RTS_API rtError_t rtSetExceptionExtInfo(const rtArgsSizeInfo_t * const sizeInfo)
{
    HOOK_LOG("rtSetExceptionExtInfo enable success");
    using RtSetExceptionExtInfo = decltype(&rtSetExceptionExtInfo);
    auto vallina = RuntimeSymbol::Instance().Get<RtSetExceptionExtInfo>(__func__);
    if (vallina == nullptr) {
        HOOK_LOG("vallina func get FAILED");
        return RT_ERROR_RESERVED;
    }
 
    rtError_t ret = vallina(sizeInfo);
    if (ret != RT_ERROR_NONE) {
        return ret;
    }
 
    // 提取算子内存空间信息
    ExtractOpMemInfo(sizeInfo, g_opMemInfo);
    return ret;
}

RTS_API rtError_t rtStreamSynchronize(rtStream_t stm)
{
    using RtStreamSynchronize = decltype(&rtStreamSynchronize);
    auto vallina = RuntimeSymbol::Instance().Get<RtStreamSynchronize>(__func__);
    if (vallina == nullptr) {
        HOOK_LOG("vallina func get FAILED");
        return RT_ERROR_RESERVED;
    }

    rtError_t ret = vallina(stm);
    return ret;
}

RTS_API rtError_t rtKernelGetAddrAndPrefCnt(void *hdl, const uint64_t tilingKey,
                                            const void *const stubFunc,
                                            const uint32_t flag, void **addr,
                                            uint32_t *prefetchCnt)
{
    using RtKernelGetAddrAndPrefCnt = decltype(&rtKernelGetAddrAndPrefCnt);
    auto vallina = RuntimeSymbol::Instance().Get<RtKernelGetAddrAndPrefCnt>(__func__);
    if (vallina == nullptr) {
        HOOK_LOG("vallina func get FAILED");
        return RT_ERROR_RESERVED;
    }

    return vallina(hdl, tilingKey, stubFunc, flag, addr, prefetchCnt);
}
