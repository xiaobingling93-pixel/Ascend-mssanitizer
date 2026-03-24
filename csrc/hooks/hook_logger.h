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


#ifndef __HOOKS_HOOK_LOGGER_H__
#define __HOOKS_HOOK_LOGGER_H__

#include <cstdint>
#include <sstream>
#include <securec.h>

#include "hook_report.h"

namespace Sanitizer {

class HookLogger {
public:
    static constexpr std::size_t MAX_LOG_BUF_SIZE = 2048UL;
    char logBuf[MAX_LOG_BUF_SIZE];

public:
    static inline HookLogger &Instance(void);
    HookLogger(HookLogger const &) = delete;
    HookLogger &operator=(HookLogger const &) = delete;

    template<typename T> inline HookLogger &operator<<(T const &t);
    inline HookLogger &operator<<(HookLogger &(*pf)(HookLogger &));
    static inline HookLogger &Ends(HookLogger &hookLogger);
    inline void Log(std::string const &logString) const;

private:
    HookLogger(void) = default;

private:
    std::ostringstream streamBuffer_;
};

HookLogger &HookLogger::Instance(void)
{
    static HookLogger instance;
    return instance;
}

template <typename T>
inline HookLogger &HookLogger::operator<<(T const &t)
{
    this->streamBuffer_ << t;
    return *this;
}
 
inline HookLogger &HookLogger::operator<<(HookLogger &(*pf)(HookLogger &))
{
    return pf(*this);
}

inline HookLogger &HookLogger::Ends(HookLogger &hookLogger)
{
    std::string const &logString = hookLogger.streamBuffer_.str();
    hookLogger.Log(logString);
    std::ostringstream().swap(hookLogger.streamBuffer_);
    return hookLogger;
}

inline void HookLogger::Log(std::string const &logString) const
{
    if (!HookReport::Instance().ReportLogString(logString)) {
        // report logstring failed, fallback to stdout
        std::cout << logString << std::endl;
    }
}

} // namespace Sanitizer

#define HOOK_LOG(format, ...)                                                 \
    {                                                                         \
        using HookLogger = Sanitizer::HookLogger;                             \
        int printRet = snprintf_truncated_s(HookLogger::Instance().logBuf,    \
                                            HookLogger::MAX_LOG_BUF_SIZE,     \
                                            "%s" format, "", ## __VA_ARGS__); \
        if (printRet >= 0) {                                                  \
            HookLogger::Instance().Log(HookLogger::Instance().logBuf);        \
        }                                                                     \
    }

#endif  // __HOOKS_HOOK_LOGGER_H__
