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


#ifndef CORE_FRAMEWORK_FILE_MAPPING_H
#define CORE_FRAMEWORK_FILE_MAPPING_H

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

#include "core/framework/utility/singleton.h"

namespace Sanitizer {

struct FileInfo {
    int16_t fileIdx;
    std::string fileName;
};

class FileMapping : public ThreadSingleton<FileMapping> {
public:
    using FileMapType = std::unordered_map<uint64_t, FileInfo>;

    /**
     * @brief 查询文件序号/文件路径映射的接口
     *
     * @param 文件序号
     * @return 对应的文件路径，如果对应的路径不存在则返回 <unknown>
     */
    FileInfo Query(uint64_t fileNo);

    /**
     * @brief 清空当前文件映射信息并从一个 StringTable buffer 加载文件映射信息
     *
     * @param StringTable buffer
     */
    void Load(std::vector<char> const& buffer);

    FileMapType const &GetFileMap(void) const { return this->fileMap_; }

    friend class std::pair<const std::thread::id, FileMapping>;

private:
    FileMapType fileMap_{};
};

} // namespace Sanitizer

#endif
