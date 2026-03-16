file(GLOB_RECURSE SANITIZER_STUB_FILES ccec/*.cpp)
file(GLOB_RECURSE DBI_SOURCES ccec/dbi/*.cpp)

list(REMOVE_ITEM SANITIZER_STUB_FILES ${DBI_SOURCES})

add_library(sanitizer_api SHARED
  clang_interface.cpp ${SANITIZER_STUB_FILES})

target_include_directories(sanitizer_api PUBLIC ${CSRC_INC_DIR} ${SANITIZER_THIRDPARTY_INCLUDE} ${SANITIZER_INC_DIR})

install(TARGETS sanitizer_api LIBRARY DESTINATION lib64)

add_subdirectory(ccec)
