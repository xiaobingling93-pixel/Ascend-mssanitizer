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

#include "file_system.h"
#include "log.h"
#include <sstream>

using namespace std;
namespace Sanitizer {

bool IsFilePermSafe(const string &dir, mode_t permission, LoggerType logger)
{
    bool needLog = logger != nullptr;
    // 返回predicate，如果predicate==false且logger非空则打印falseMsg
    auto isTrueOrLog = [needLog, logger](bool predicate, const char *falseMsg) -> bool {
        if (!predicate && needLog) {
            logger(falseMsg);
        }
        return predicate;
    };
    Path curPath = Path(dir).Absolute();
    string curPathStr = Utility::ReplaceInvalidChar(curPath.ToString());
    mode_t logFileDirMode = permission;
    mode_t currentMode;
    if (!GetPathMode(curPathStr, currentMode)) {
        printf("[mssanitizer] ERROR: Get (%s) permission failed.\n", curPathStr.c_str());
        return false;
    }
    currentMode = currentMode & 07777;
    std::stringstream permissionStr;
    std::stringstream currentPermission;
    permissionStr << "0" << std::oct << permission;
    currentPermission << "0" << std::oct << currentMode;

    std::stringstream msg;
    msg << "Current file or dir permission is " << currentPermission.str() << ", which poses a security risk. " <<
        "It's recommended to chmod " << curPathStr << " permission to the " << permissionStr.str() << ".";
    // root用户不强制要求权限，仅对风险权限进行告警
    isTrueOrLog((!IsRootUser() || !curPath.Exists() || IsModeSaferThan(dir, logFileDirMode)),
        (string("WARNING: ") + msg.str()).c_str());
    return isTrueOrLog(curPath.Exists(), (string("ERROR: Directory (") + curPathStr + ") does not exist.").c_str()) &&
           isTrueOrLog(IsRootUser() || IsOwnerOf(dir),
               (string("ERROR: The user is not the owner of the directory (") + curPathStr + ")").c_str()) &&
           isTrueOrLog(
               IsRootUser() || IsModeSaferThan(dir, logFileDirMode), (string("ERROR: ") + msg.str()).c_str());
}

bool IsSafeLogFile(string const & logFile, LoggerType logger)
{
    bool needLog = logger != nullptr;
    auto isTrueOrLog = [needLog, logger](bool predicate, const char *falseMsg) -> bool {
        if (!predicate && needLog) {
            logger(falseMsg);
        }
        return predicate;
    };
    std::string parentPath = Path(logFile).Parent().ToString();
    // 检查父目录
    if (!IsFilePermSafe(parentPath, DIR_FILE_MODE, logger)) {
        return false;
    }
    return isTrueOrLog(!IsSoftLink(parentPath),
                       (string("ERROR: The directory (") + parentPath + ") is a soft link.").c_str()) &&
            // 检查log文件
            (!Path(logFile).Exists() ||
            (IsFilePermSafe(logFile, LEAST_OUTPUT_FILE_MODE, logger) &&
            isTrueOrLog(!IsSoftLink(logFile),
                        (string("ERROR: The log file (") + logFile + ") is a soft link.").c_str())));
}

bool GetSelfExePath(Path &path)
{
    char buf[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
    if (len <= 0) {
        SAN_ERROR_LOG("get self exe path failed from /proc/self/exe");
        return false;
    }
    buf[len] = 0;
    path = Path(buf);
    return true;
}

}  // namespace Sanitizer
