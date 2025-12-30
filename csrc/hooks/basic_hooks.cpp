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


#include <iostream>

#include "runtime.h"
#include "vallina_symbol.h"
#include "hook_report.h"
#include "hook_logger.h"

using namespace Sanitizer;

namespace {

struct RuntimeLibLoader {
    static void *Load(void)
    {
        return RuntimeLibLoad("libruntime.so");
    }
};

using RuntimeSymbol = VallinaSymbol<RuntimeLibLoader>;

}  // namepasce Dummy

RTS_API rtError_t rtSetDevice(int32_t devId)
{
    /// 根据该日志信息判断算子是否上板
    HOOK_LOG("rtSetDevice enable success device %d", devId);
    using RtSetDevice = decltype(&rtSetDevice);
    auto vallina = RuntimeSymbol::Instance().Get<RtSetDevice>(__func__);
    if (vallina == nullptr) {
        std::cout << "[" << __func__ << "] vallina func get FAILED" << std::endl;
        return RT_ERROR_RESERVED;
    }

    using RtGetSocVersion = decltype(&rtGetSocVersion);
    auto vallinaRtGetSocVersion = RuntimeSymbol::Instance().Get<RtGetSocVersion>("rtGetSocVersion");
    if (vallinaRtGetSocVersion == nullptr) {
        std::cout << "[" << __func__ << "] get vallina func rtGetSocVersion FAILED" << std::endl;
        return RT_ERROR_RESERVED;
    }

    rtError_t ret = vallina(devId);
    if (ret != RT_ERROR_NONE) {
        return ret;
    }

    /// 必须在设置了当前的 deviceId 之后才能获取芯片型号
    constexpr uint64_t socVersionBufLen = 64UL;
    char socVersion[socVersionBufLen] = "";
    if (vallinaRtGetSocVersion(socVersion, sizeof(socVersion)) == RT_ERROR_NONE) {
        typename decltype(SOC_VERSION_MAP)::const_iterator it = SOC_VERSION_MAP.find(socVersion);
        DeviceType deviceType = it == SOC_VERSION_MAP.cend() ? DeviceType::INVALID : it->second;
        HookReport::Instance().ReportDeviceType(deviceType);
        if (it == SOC_VERSION_MAP.cend()) {
            std::cout << "[mssanitizer] unsupported soc version " << socVersion << std::endl;
        }
    } else {
        std::cout << "[" << __func__ << "] get soc version FAILED" << std::endl;
    }

    return ret;
}
