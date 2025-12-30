# 如果上层工程已添加了googletest，则这里就不再需要添加
if (NOT DEFINED GOOGLETEST)
    # EXCLUDE_FROM_ALL 禁用 gtest 的 install 功能，保持 output 目录干净
    add_subdirectory(googletest EXCLUDE_FROM_ALL)
    set(GOOGLETEST TRUE)
endif()
