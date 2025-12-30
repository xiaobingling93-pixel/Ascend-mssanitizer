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


#ifndef __TEST_UTILITY_TEMP_FILE_GUARD_H__
#define __TEST_UTILITY_TEMP_FILE_GUARD_H__

#include <string>
#include <fstream>

#include "utility/file_system.h"

class TempFileGuard {
public:
    explicit TempFileGuard(std::string const &fileName) : fileName_{fileName}, alreadyExist_{false}
    {
        if (Sanitizer::IsPathExists(fileName)) {
            alreadyExist_ = true;
            return;
        }
        std::ofstream ofs(fileName);
        ofs << "some content" << std::endl;
    }
    ~TempFileGuard(void)
    {
        if (!alreadyExist_) {
            remove(fileName_.c_str());
        }
    }

private:
    std::string fileName_;
    bool alreadyExist_;
};

#endif // __TEST_UTILITY_TEMP_FILE_GUARD_H__
