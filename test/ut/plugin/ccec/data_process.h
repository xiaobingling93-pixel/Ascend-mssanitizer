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

#ifndef TEST_PLUGIN_CCEC_CCEC_DATA_PROCESS_H
#define TEST_PLUGIN_CCEC_CCEC_DATA_PROCESS_H

#include <array>
#include <random>
#include <vector>
#include <algorithm>
#include "core/framework/record_defs.h"
#include "ccec_defs.h"
#include "plugin/record_type_map.h"

namespace Sanitizer {

int RandInt(int min, int max);

// 使用CheckBinaryOpBlockInfo和CheckUnaryOpBlockInfo作为校验桩函数入参的大小
struct CheckBinaryOpBlockInfo {
    uint8_t dstBlockNum;
    uint8_t src0BlockNum;
    uint8_t src1BlockNum;
    uint16_t dstBlockSize;
    uint16_t src0BlockSize;
    uint16_t src1BlockSize;
};
struct CheckUnaryOpBlockInfo {
    uint8_t dstBlockNum;
    uint8_t srcBlockNum;
    uint16_t dstBlockSize;
    uint16_t srcBlockSize;
};
struct CheckReduceOpBlockInfo {
    uint8_t dstBlockNum;
    uint8_t srcBlockNum;
    uint16_t dstBlockSize;
    uint16_t srcBlockSize;
};

std::vector<uint8_t> CreateMemInfo();

UnaryOpRecord CreateRandomUnaryOpRecord();
VgatherRecord CreateRandomVgatherRecord();
BinaryOpRecord CreateRandomBinaryOpRecord();
ReduceOpRecord CreateRandomReduceOpRecord();
ReduceV2Record CreateRandomReduceV2Record();
VecDupRecord CreateRandomVecDupRecord();
VecRegPropCoordOpRecord CreateRandomVecRegPropCoordOpRecord();
CmpMaskRecord CreateRandomCmpMaskRecord();

std::vector<UnaryOpRecord> CreateRandomUnaryOpRecords(uint8_t randNum);
std::vector<BinaryOpRecord> CreateRandomBinaryOpRecords(uint8_t randNum);
std::vector<ReduceOpRecord> CreateRandomReduceOpRecords(uint8_t randNum);

bool IsEqual(UnaryOpRecord const &lhs, UnaryOpRecord const &rhs);
bool IsEqual(BinaryOpRecord const &lhs, BinaryOpRecord const &rhs);

// 比对单目和双目的原因是部分单目指令存在对dst的读和写操作，代码内部对这类指令是用双目结构体去存储，此时双目中的src1代表dst
bool IsEqual(LoadB2Record const &lhs, LoadB2Record const &rhs);
bool IsEqual(Load2DRecord const &lhs, Load2DRecord const &rhs);
bool IsEqual(Load2DSparseRecord const &lhs, Load2DSparseRecord const &rhs);
bool IsEqual(Load2DTransposeRecord const &lhs, Load2DTransposeRecord const &rhs);
bool IsEqual(DecompressHeaderRecord const &lhs, DecompressHeaderRecord const &rhs);
bool IsEqual(DcPreloadRecord const &lhs, DcPreloadRecord const &rhs);
bool IsEqual(BroadcastRecord const &lhs, BroadcastRecord const &rhs);
bool IsEqual(ScatterVnchwconvRecord const &lhs, ScatterVnchwconvRecord const &rhs);
bool IsEqual(LoadAWinogradRecord const &lhs, LoadAWinogradRecord const &rhs);
bool IsEqual(LoadBWinogradRecord const &lhs, LoadBWinogradRecord const &rhs);
bool IsEqual(Load3DRecord const &lhs, Load3DRecord const &rhs);
bool IsEqual(Set2DRecord const &lhs, Set2DRecord const &rhs);
bool IsEqual(LoadImageRecord const &lhs, LoadImageRecord const &rhs);
bool IsEqual(LoadSmaskRecord const &lhs, LoadSmaskRecord const &rhs);
bool IsEqual(UnaryOpRecord const &lhs, BinaryOpRecord const &rhs);
bool IsEqual(ReduceOpRecord const &lhs, ReduceOpRecord const &rhs);
bool IsEqual(DmaMovRecord const &lhs, DmaMovRecord const &rhs);
bool IsEqual(DmaMovConvReluRecord const &lhs, DmaMovConvReluRecord const &rhs);
bool IsEqual(DmaMovNd2nzRecord const &lhs, DmaMovNd2nzRecord const &rhs);
bool IsEqual(MovAlignRecord const &lhs, MovAlignRecord const &rhs);
bool IsEqual(MovAlignRecordV2 const &lhs, MovAlignRecordV2 const &rhs);
bool IsEqual(MovBtRecord const &lhs, MovBtRecord const &rhs);
bool IsEqual(MovFpRecord const &lhs, MovFpRecord const &rhs);
bool IsEqual(VecDupRecord const &lhs, VecDupRecord const &rhs, uint8_t bitsSize);
bool IsEqual(HardSyncRecord const &lhs, HardSyncRecord const &rhs);
bool IsEqual(SyncRecord const &lhs, SyncRecord const &rhs);
bool IsEqual(PipeBarrierRecord const &lhs, PipeBarrierRecord const &rhs);
bool IsEqual(LoadStoreRecord const &lhs, LoadStoreRecord const &rhs);
bool IsEqual(FftsSyncRecord const &lhs, FftsSyncRecord const &rhs);
bool IsEqual(WaitFlagDevRecord const &lhs, WaitFlagDevRecord const &rhs);
bool IsEqual(WaitFlagDevPipeRecord const &lhs, WaitFlagDevPipeRecord const &rhs);
bool IsEqual(IntraBlockSyncRecord const& lhs, IntraBlockSyncRecord const& rhs);
bool IsEqual(VecRegPropCoordOpRecord const &lhs, VecRegPropCoordOpRecord const &rhs);
bool IsEqual(SoftSyncRecord const& lhs, SoftSyncRecord const& rhs);
bool IsEqual(MstxCrossRecord const& lhs, MstxCrossRecord const& rhs);
bool IsEqual(MstxRecord const& lhs, MstxRecord const& rhs);
bool IsEqual(NdDMAOut2UbRecord const& lhs, NdDMAOut2UbRecord const& rhs);
bool IsEqual(DmaMovNd2nzDavRecord const& lhs, DmaMovNd2nzDavRecord const& rhs);
bool IsEqual(MovL1UBRecord const& lhs, MovL1UBRecord const& rhs);
bool IsEqual(MovL1BtRecord const& lhs, MovL1BtRecord const& rhs);
bool IsEqual(MovL1FbRecord const& lhs, MovL1FbRecord const& rhs);
bool BlockSizeNumIsEqual(CheckBinaryOpBlockInfo &blockInfo, BinaryOpRecord const &rhs);
bool BlockSizeNumIsEqual(CheckUnaryOpBlockInfo &blockInfo, UnaryOpRecord const &rhs);
bool BlockSizeNumIsEqual(CheckReduceOpBlockInfo &blockInfo, ReduceOpRecord const &rhs);

template <RecordType recordType>
bool CheckRecordEqual(uint8_t const *memInfo, RecordMapping<recordType> const &record)
{
    using Record = RecordMapping<recordType>;
    /// 检查 RecordType 是否匹配
    RecordType actualType = *reinterpret_cast<RecordType const *>(memInfo);
    if (actualType != recordType) {
        return false;
    }

    /// 检查 Record 结构体是否一致
    Record const &actualRecord = *reinterpret_cast<Record const *>(memInfo + sizeof(RecordType));
    return IsEqual(actualRecord, record);
}

inline bool CheckRecordEqual(uint8_t const *memInfo, VecDupRecord const &record, uint8_t bitsSize)
{
    VecDupRecord const &actualRecord = *reinterpret_cast<VecDupRecord const *>(memInfo + sizeof(RecordType));
    return IsEqual(actualRecord, record, bitsSize);
}

uint64_t ExtractConfigFromUnaryOpRecord(const UnaryOpRecord &record);
uint64_t ExtractConfigFromUnaryOpRecordWithOffset(const UnaryOpRecord &record, uint32_t offset);
uint64_t ExtractConfigFromBinaryOpRecord(const BinaryOpRecord &record);
uint64_t ExtractConfigFromReduceOpRecord(const ReduceOpRecord &record);

template<typename MemRecord, typename CheckBlockInfoType, typename VecRecord = MemRecord>
inline bool MultiIsEqual(uint8_t const *memInfo, std::vector<VecRecord> const &records,
    std::vector<CheckBlockInfoType> &blockInfoVec)
{
    memInfo += sizeof(RecordGlobalHead);
    RecordBlockHead const &recordBlockHead = *reinterpret_cast<RecordBlockHead const*>(memInfo);
    memInfo += sizeof(RecordBlockHead);
    if (blockInfoVec.size() < recordBlockHead.recordWriteCount) {
        return false;
    }
    for (uint64_t i = 0; i < recordBlockHead.recordWriteCount; ++i) {
        MemRecord const gmRecord = *reinterpret_cast<MemRecord const*>(memInfo + sizeof(RecordType));
        memInfo += sizeof(RecordType) + sizeof(MemRecord);
        VecRecord const record = records[i];
        CheckBlockInfoType blockInfo = blockInfoVec[i];
        if (!IsEqual(record, gmRecord)) {
            return false;
        }
        if (!BlockSizeNumIsEqual(blockInfo, gmRecord)) {
            return false;
        }
    }
    return true;
}

template<typename DstType, typename Src0Type = DstType, typename Src1Type = Src0Type, typename... Args>
void CallUnaryScalarConfigFunc(uint8_t *memInfo, std::vector<UnaryOpRecord> &records, uint8_t idx,
                               UnaryScalarConfigFunc<DstType, Src0Type, Src1Type, Args...> func, Args... args)
{
    uint64_t config = ExtractConfigFromUnaryOpRecord(records[idx]);
    func(memInfo, records[idx].location.fileNo, records[idx].location.lineNo, records[idx].location.pc,
        reinterpret_cast<DstType*>(records[idx].dst),
        reinterpret_cast<Src0Type*>(records[idx].src), Src1Type{}, config, args...);
}

template<typename DstType, typename Src0Type = DstType, typename Src1Type = Src0Type, typename... Args>
void CallUnaryScalarU16Func(uint8_t *memInfo, std::vector<UnaryOpRecord> &records, uint8_t idx,
                            UnaryScalarU16Func<DstType, Src0Type, Src1Type, Args...> func, Args... args)
{
    func(memInfo, records[idx].location.fileNo, records[idx].location.lineNo, records[idx].location.pc,
        reinterpret_cast<DstType*>(records[idx].dst),
        reinterpret_cast<Src0Type*>(records[idx].src),
        Src1Type{}, records[idx].repeat, records[idx].dstBlockStride, records[idx].srcBlockStride,
        records[idx].dstRepeatStride, records[idx].srcRepeatStride, args...);
}
 
template<typename DstType, typename Src0Type = DstType, typename Src1Type = Src0Type, typename... Args>
void CallUnaryScalarU8Func(uint8_t *memInfo, std::vector<UnaryOpRecord> &records, uint8_t idx,
                           UnaryScalarU8Func<DstType, Src0Type, Src1Type, Args...> func, Args... args)
{
    func(memInfo, records[idx].location.fileNo, records[idx].location.lineNo, records[idx].location.pc,
        reinterpret_cast<DstType*>(records[idx].dst),
        reinterpret_cast<Src0Type*>(records[idx].src),
        Src1Type{}, records[idx].repeat, records[idx].dstBlockStride, records[idx].srcBlockStride,
        records[idx].dstRepeatStride, records[idx].srcRepeatStride, args...);
}
 
template<typename DstType, typename SrcType = DstType, typename... Args>
void CallUnaryConfigFunc(uint8_t *memInfo, std::vector<UnaryOpRecord> &records, uint8_t idx,
                         UnaryConfigFunc<DstType, SrcType, Args...> func, Args... args)
{
    uint64_t config = ExtractConfigFromUnaryOpRecord(records[idx]);
    func(memInfo, records[idx].location.fileNo, records[idx].location.lineNo, records[idx].location.pc,
        reinterpret_cast<DstType*>(records[idx].dst),
        reinterpret_cast<SrcType*>(records[idx].src), config, args...);
}
 
template<typename DstType, typename SrcType = DstType, typename... Args>
void CallUnaryU16Func(uint8_t *memInfo, std::vector<UnaryOpRecord> &records, uint8_t idx,
                      UnaryU16Func<DstType, SrcType, Args...> func, Args... args)
{
    func(memInfo, records[idx].location.fileNo, records[idx].location.lineNo, records[idx].location.pc,
        reinterpret_cast<DstType*>(records[idx].dst),
        reinterpret_cast<SrcType*>(records[idx].src),
        records[idx].repeat, records[idx].dstBlockStride, records[idx].srcBlockStride,
        records[idx].dstRepeatStride, records[idx].srcRepeatStride, args...);
}
 
template<typename DstType, typename SrcType = DstType, typename... Args>
void CallUnaryU8Func(uint8_t *memInfo, std::vector<UnaryOpRecord> &records, uint8_t idx,
                     UnaryU8Func<DstType, SrcType, Args...> func, Args... args)
{
    func(memInfo, records[idx].location.fileNo, records[idx].location.lineNo, records[idx].location.pc,
        reinterpret_cast<DstType*>(records[idx].dst),
        reinterpret_cast<SrcType*>(records[idx].src),
        records[idx].repeat, records[idx].dstBlockStride, records[idx].srcBlockStride,
        records[idx].dstRepeatStride, records[idx].srcRepeatStride, args...);
}
 
template<typename DstType, typename Src0Type = DstType, typename Src1Type = Src0Type, typename... Args>
void CallBinaryConfigFunc(uint8_t *memInfo, std::vector<BinaryOpRecord> &records, uint8_t idx,
                          BinaryConfigFunc<DstType, Src0Type, Src1Type, Args...> func, Args... args)
{
    uint64_t config = ExtractConfigFromBinaryOpRecord(records[idx]);
    func(memInfo, records[idx].location.fileNo, records[idx].location.lineNo, records[idx].location.pc,
        reinterpret_cast<DstType*>(records[idx].dst),
        reinterpret_cast<Src0Type*>(records[idx].src0),
        reinterpret_cast<Src1Type*>(records[idx].src1), config, args...);
}
 
template<typename DstType, typename Src0Type = DstType, typename Src1Type = Src0Type, typename... Args>
void CallBinaryFunc(uint8_t *memInfo, std::vector<BinaryOpRecord> &records, uint8_t idx,
                    BinaryFunc<DstType, Src0Type, Src1Type, Args...> func, Args... args)
{
    func(memInfo, records[idx].location.fileNo, records[idx].location.lineNo, records[idx].location.pc,
        reinterpret_cast<DstType*>(records[idx].dst),
        reinterpret_cast<Src0Type*>(records[idx].src0),
        reinterpret_cast<Src1Type*>(records[idx].src1),
        records[idx].repeat, records[idx].dstBlockStride,
        records[idx].src0BlockStride, records[idx].src1BlockStride, records[idx].dstRepeatStride,
        records[idx].src0RepeatStride, records[idx].src1RepeatStride, args...);
}
 
template<typename DstType, typename SrcType = DstType, typename... Args>
void CallReduceConfigFunc(uint8_t *memInfo, std::vector<ReduceOpRecord> &records, uint8_t idx,
                          ReduceConfigFunc<DstType, SrcType, Args...> func, Args... args)
{
    uint64_t config = ExtractConfigFromReduceOpRecord(records[idx]);
    func(memInfo, records[idx].location.fileNo, records[idx].location.lineNo, records[idx].location.pc,
        reinterpret_cast<DstType*>(records[idx].dst),
        reinterpret_cast<SrcType*>(records[idx].src), config, args...);
}
 
template<typename DstType, typename SrcType = DstType, typename... Args>
void CallReduceFunc(uint8_t *memInfo, std::vector<ReduceOpRecord> &records, uint8_t idx,
                    ReduceFunc<DstType, SrcType, Args...> func, Args... args)
{
    func(memInfo, records[idx].location.fileNo, records[idx].location.lineNo, records[idx].location.pc,
        reinterpret_cast<DstType*>(records[idx].dst),
        reinterpret_cast<SrcType*>(records[idx].src),
        records[idx].repeat, records[idx].dstRepeatStride, records[idx].srcBlockStride,
        records[idx].srcRepeatStride, args...);
}

}  // namespace Sanitizer

#endif  // TEST_PLUGIN_CCEC_CCEC_DATA_PROCESS_H
