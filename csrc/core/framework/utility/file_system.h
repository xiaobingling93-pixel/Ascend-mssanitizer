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


#ifndef CORE_FRAMEWORK_UTILITY_FILE_SYSTEM_H
#define CORE_FRAMEWORK_UTILITY_FILE_SYSTEM_H

#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdlib>
#include <functional>

#include "securec.h"
#include "umask_guard.h"
#include "path.h"
#include "ustring.h"

namespace Sanitizer {

constexpr char const *PATH_SEP = "/";
constexpr mode_t  DIR_FILE_MODE = 0750;
constexpr mode_t  LEAST_OUTPUT_FILE_MODE = 0640;


inline bool IsSoftLink(const std::string &path)
{
    struct stat buf{};
    return lstat(path.c_str(), &buf) == 0 && (S_IFMT & buf.st_mode) == S_IFLNK;
}

inline bool IsSoftLinkRecur(const std::string &path)
{
    Path pathObj(path);
    if (!pathObj.Exists()) {
        return false;
    }
    while (pathObj.ToString() != "/" && pathObj.ToString() != ".") {
        if (IsSoftLink(pathObj.ToString())) {
            return true;
        }
        pathObj = pathObj.Parent();
    }
    return false;
}

inline bool IsDir(const std::string &path)
{
    struct stat buf{};
    return stat(path.c_str(), &buf) == 0 && (buf.st_mode & S_IFMT) == S_IFDIR;
}

inline bool IsOwnerOf(const std::string &path)
{
    struct stat buf {};
    return lstat(path.c_str(), &buf) == 0 && getuid() == buf.st_uid;
}

inline bool IsRootUser()
{
    return getuid() == 0;
}

// get file or dir mode by input path
inline bool GetPathMode(const std::string &path, mode_t &mode)
{
    struct stat buf{};
    if (stat(path.c_str(), &buf) != 0) {
        return false;
    }
    mode = buf.st_mode;
    return true;
}


// 按位考虑，对于hazardMode为0的位，path指定文件的mode也都为0则安全
inline bool IsModeSaferThan(const std::string &path, mode_t hazardMode)
{
    mode_t mode;
    if (!GetPathMode(path, mode)) {
        return false;
    }
    mode_t hazard = ~(hazardMode & ACCESSPERMS) & ACCESSPERMS;
    mode_t pathMode = mode & ACCESSPERMS;
    return !(hazard & pathMode);
}

inline std::string GetAbsolutePath(const std::string &path)
{
    auto *temp = realpath(path.c_str(), nullptr);
    if (temp == nullptr) {
        return "";
    }
    std::string absolutePath(temp);
    free(temp);
    return absolutePath;
}

inline bool IsPathExists(const std::string &path)
{
    if (path.empty()) {
        return false;
    }
    struct stat buf{};
    return stat(path.c_str(), &buf) == 0;
}

inline FILE* OpenFileWithUmask(const std::string &path, const std::string &mode, mode_t mask)
{
    if (path.empty()) {
        return nullptr;
    }
    UmaskGuard guard{mask};
    FILE *fp = fopen(path.c_str(), mode.c_str());
    return fp;
}

inline bool IsFileExecutable(struct stat fileStat)
{
    if (getuid() == fileStat.st_uid) {
        if ((fileStat.st_mode & S_IXUSR) != 0) {
            return true;
        }
    } else if (getgid() == fileStat.st_gid) {
        if ((fileStat.st_mode & S_IXGRP) != 0) {
            return true;
        }
    } else if ((fileStat.st_mode & S_IXOTH) != 0) {
        return true;
    } else if (IsRootUser()) {
        return true;
    }
    return false;
}

template<typename Iterator>
bool ListDir(Path const &path, Iterator it)
{
    struct stat st{};
    if (!path.GetStat(st) || (st.st_mode & S_IFDIR) == 0) {
        return false;
    }

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.ToString().c_str())) == nullptr) {
        return false;
    }

    for (; (ent = readdir(dir)) != nullptr; ++it) {
        *it = path / Path(ent->d_name);
    }
    closedir(dir);
    return true;
}

inline bool WriteBinary(std::string const &filename, char const *data, uint64_t length)
{
    if (!data) {
        return false;
    }
    std::ofstream ofs(filename, std::ios::out | std::ios::binary);
    ofs.write(data, length);
    return ofs.good();
}

inline bool ChangeMode(std::string const &path, mode_t mode)
{
    return chmod(path.c_str(), mode) == 0;
}

using LoggerType = std::function<void(const std::string &)>;

bool IsFilePermSafe(std::string const &dir, mode_t permission, LoggerType logger = nullptr);

bool IsSafeLogFile(std::string const & logFile, LoggerType logger = nullptr);

/// 获取当前执行程序的路径
bool GetSelfExePath(Path &path);

}
#endif // !CORE_FRAMEWORK_UTILITY_FILE_SYSTEM_H
