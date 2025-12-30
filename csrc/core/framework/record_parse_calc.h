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


#ifndef CORE_FRAMEWORK_RECORD_PARSE_CALC_H
#define CORE_FRAMEWORK_RECORD_PARSE_CALC_H

#include <vector>

#include "record_defs.h"
#include "event_def.h"

namespace Sanitizer {

extern void ParseRecordScatterVnchwconv(const KernelRecord &record, std::vector<SanEvent> &events);
extern void ParseVRegPropCoorOPRecord(const KernelRecord &record, std::vector<SanEvent> &events);
} // namespace Sanitizer

#endif