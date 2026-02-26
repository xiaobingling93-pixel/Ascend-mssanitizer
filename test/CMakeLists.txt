cmake_minimum_required(VERSION 3.13) # to support add_link_options
set(TEST_BIN_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

set(asan_test_header ${CMAKE_SOURCE_DIR}/csrc
                     ${CMAKE_SOURCE_DIR}/csrc/address_sanitizer
                     ${CMAKE_SOURCE_DIR}/csrc/core/framework
                     ${SECUREC_INC_DIR}
                     ${CMAKE_SOURCE_DIR}/csrc/race_sanitizer
                     ${CMAKE_SOURCE_DIR}/csrc/sync_sanitizer
                     ${CMAKE_SOURCE_DIR}/csrc/register_sanitizer
                     plugin)


file(GLOB_RECURSE TEST_ORIGINAL_SOURCE ${CMAKE_SOURCE_DIR}/csrc/core/*.cpp
      ${CMAKE_SOURCE_DIR}/csrc/address_sanitizer/*.cpp
      ${CMAKE_SOURCE_DIR}/csrc/race_sanitizer/*.cpp
      ${CMAKE_SOURCE_DIR}/csrc/race_sanitizer/alg_framework/*.cpp
      ${CMAKE_SOURCE_DIR}/csrc/sync_sanitizer/*.cpp
      ${CMAKE_SOURCE_DIR}/csrc/register_sanitizer/*.cpp
      ${CMAKE_SOURCE_DIR}/csrc/plugin/clang_interface.cpp
      ${CMAKE_SOURCE_DIR}/csrc/hooks/hooks_verify.cpp
      ${CMAKE_SOURCE_DIR}/csrc/hooks/hook_report.cpp
      ${CMAKE_SOURCE_DIR}/csrc/hooks/ascend_hal_stub.cpp)

list(SORT TEST_ORIGINAL_SOURCE)
file(GLOB_RECURSE TEST_SOURCE
  ./address_sanitizer/*.cpp
  ./race_sanitizer/*.cpp
  ./sync_sanitizer/*.cpp
  ./register_sanitizer/*.cpp
  ./core/framework/*.cpp
  ./core/framework/utility/*.cpp
  ./stub_def/*.cpp
  ./hooks/*.cpp
  ./plugin/*.cpp)
list(SORT TEST_SOURCE)

set(gtest_header ${THIRD_PARTY_DIR}/googletest/googletest/include/)
set(hooks_header ${CMAKE_SOURCE_DIR}/csrc/hooks/)

add_subdirectory(ut)
