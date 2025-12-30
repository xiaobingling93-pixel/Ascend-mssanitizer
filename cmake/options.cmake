# 安全规范编译命令
add_compile_options("-fPIE")
add_compile_options("-fPIC")
add_compile_options("-fstack-protector-all")
add_compile_options("-Wall")
add_compile_options("-D_FORTIFY_SOURCE=2")
add_compile_options("-fno-strict-aliasing")
add_compile_options("-O2")

add_link_options("-Wl,-z,now")
add_link_options("-Wl,-z,relro")
add_link_options("-Wl,-z,noexecstack")
add_link_options("-pie")
add_link_options("-s") #strip
set(CMAKE_SKIP_RPATH TRUE)
set(CMAKE_CXX_FLAGS "-std=c++11")

# 设置工具提交 ID
find_package(Git REQUIRED)
execute_process(COMMAND "${GIT_EXECUTABLE}" rev-parse HEAD
  OUTPUT_VARIABLE MSSANITIZER_COMMIT_REVISION OUTPUT_STRIP_TRAILING_WHITESPACE)
add_definitions(-D__MSSANITIZER_COMMIT_REVISION__="${MSSANITIZER_COMMIT_REVISION}")