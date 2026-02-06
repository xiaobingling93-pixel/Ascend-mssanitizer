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


#ifndef SYNC_SANITIZER_SYNC_SANITIZER_H
#define SYNC_SANITIZER_SYNC_SANITIZER_H

#include <map>
#include <vector>
#include "core/framework/sanitizer_base.h"

namespace Sanitizer {

// 该类实现synccheck检测，当前功能是检测并上报多余的set_flag同步指令
class SyncSanitizer : public SanitizerBase {
public:
    bool SetDeviceInfo(DeviceInfoSummary const &deviceInfo, Config const &config) override;
    bool SetKernelInfo(KernelSummary const &kernelInfo) override;
    void Do(const SanitizerRecord &record, const std::vector<SanEvent> &events) override;
    void ParseOnlineError(const KernelErrorRecord &record, BlockType blockType, uint64_t serialNo) override;
    bool CheckRecordBeforeProcess(const SanitizerRecord &record) override;
    void RegisterNotifyFunc(const MSG_FUNC &func) override;
    void Exit() override;

private:
    inline bool IsTargetBlockId(uint32_t blockId);
    uint64_t CalcSetFlagSyncID(SanEvent const &event);
    uint64_t CalcWaitFlagSyncID(SanEvent const &event);
    void ReportUnpairedInfo();

    int16_t checkBlockId_ = CHECK_ALL_BLOCK;
    MSG_FUNC msgFunc_;
    std::map<uint64_t, std::vector<SyncDispInfo>> syncEvents_;
    bool isFinished_{false};
};
}

#endif  // SYNC_SANITIZER_SYNC_SANITIZER_H
