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


#include <string>
#include <vector>
#include <iterator>
#include <fstream>
#include <gtest/gtest.h>
#include <fcntl.h>

#include "utility/path.h"
#include "utility/file_system.h"
#include "temp_file_guard.h"

using namespace Sanitizer;

TEST(FileSystem, list_directory_of_not_exist_path_expect_return_false)
{
    Path path("./not_exist");
    std::vector<Path> childPaths;
    ASSERT_FALSE(ListDir(path, std::back_inserter(childPaths)));
}

TEST(FileSystem, list_directory_of_not_directory_path_expect_return_false)
{
    Path path("/bin/ls");
    std::vector<Path> childPaths;
    ASSERT_FALSE(ListDir(path, std::back_inserter(childPaths)));
}

TEST(FileSystem, list_directory_of_directory_path_expect_return_true)
{
    Path path("/bin/");
    std::vector<Path> childPaths;
    ASSERT_TRUE(ListDir(path, std::back_inserter(childPaths)));
    ASSERT_FALSE(childPaths.empty());
}

TEST(FileSystem, write_binary_expect_success_and_read_correct_data)
{
    std::string filename = "./test.bin";
    char wbuf[] = "1234567890";
    ASSERT_TRUE(WriteBinary(filename, wbuf, sizeof(wbuf)));

    std::ifstream ifs;
    ifs.open(filename, std::ios::binary);
    ASSERT_TRUE(ifs.is_open());
    char rbuf[sizeof(wbuf)];
    ifs.read(rbuf, sizeof(rbuf));
    ifs.close();
    ASSERT_EQ(std::string(wbuf), std::string(rbuf));

    std::remove(filename.c_str());
}

TEST(FileSystem, file_owner_checking)
{
    std::string fileName = "./test_file";
    remove(fileName.c_str());
    std::ofstream file(fileName, std::iostream::out);
    file.close();

    ASSERT_TRUE(IsOwnerOf(fileName));
}

void CompareFileMode(mode_t fileMode, bool less, mode_t testMode)
{
    mode_t umaskValue = ~fileMode & ACCESSPERMS;
    std::string fileName{"./test_file"};
    UmaskGuard umaskGuard(umaskValue);
    remove(fileName.c_str());
    int fd = open(fileName.c_str(), O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    ASSERT_NE(fd, -1);
    close(fd);
    ASSERT_TRUE(Path(fileName).Exists());
    struct stat st;
    Path pathObj(fileName);
    pathObj.GetStat(st);
    bool fileModeFitExpect = !((st.st_mode & ACCESSPERMS) ^ fileMode);
    if (!fileModeFitExpect) {
        std::cout << "actual fileMode:" << std::oct << st.st_mode << ", setMode:" << fileMode << std::endl;
    }
    ASSERT_TRUE(fileModeFitExpect);
    bool fitExpect = IsModeSaferThan(fileName, testMode) == less;
    if (!fitExpect) {
        std::cout << "fileMode:" << std::oct << fileMode << ", testMode:" << testMode << std::endl;
    }
    ASSERT_TRUE(fitExpect);
    remove(fileName.c_str());
}

TEST(FileSystem, file_mode_checking)
{
    CompareFileMode(0777, false, 0640);
    CompareFileMode(0711, false, 0640);
    CompareFileMode(0700, false, 0640);
    CompareFileMode(0420, false, 0640);
    CompareFileMode(0210, false, 0640);
    CompareFileMode(0600, true, 0640);
    CompareFileMode(0640, true, 0640);
    CompareFileMode(0240, true, 0640);
    CompareFileMode(0000, true, 0640);
}
