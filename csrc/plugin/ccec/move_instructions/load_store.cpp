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


#include "plugin/record_move_instructions.h"
#include "plugin/utils.h"

using namespace Sanitizer;

/// load/store 接口没有对应的 CCE 指令，编译器在插桩时无法生成 mangling 之后的类型，
/// 因此将 load/store 的桩接口声明为 extern "C" 的类型
extern "C" {
SANITIZER_REPORT(load, AddressSpace space, uint64_t addr, uint64_t size)
{
    RecordLoadStoreEvent<RecordType::LOAD>(EXTRA_PARAMS, space, addr, size, size);
}

SANITIZER_REPORT(store, AddressSpace space, uint64_t addr, uint64_t size)
{
    RecordLoadStoreEvent<RecordType::STORE>(EXTRA_PARAMS, space, addr, size, size);
}
}  // extern "C"
