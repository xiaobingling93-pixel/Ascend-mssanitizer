// Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.

#include "core/framework/record_defs.h"

#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3101
#define SIMT_MODE  // 开启simt模式
#include "plugin/record_simt_instructions.h"
#include "plugin/addr_process.h"
using namespace Sanitizer;

SANITIZER_REPORT(simt_stk_b8, uint64_t addr, int64_t offset)
{
    SimtRecordLoadStoreEvent<RecordType::SIMT_STK, DetailedDataType::B8>(EXTRA_PARAMS, AddressSpace::PRIVATE,
                                                                           addr, offset);
}

#undef SIMT_MODE
#else  // NOT C310
#include "plugin/utils.h"
using namespace Sanitizer;

SANITIZER_REPORT(simt_stk_b8, uint64_t addr, int64_t offset)
{
    (void)addr;
    (void)offset;
}

#endif