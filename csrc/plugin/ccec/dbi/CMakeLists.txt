add_library(dynamic_bind STATIC dynamic_bind.cpp)
target_include_directories(dynamic_bind PUBLIC ${MSOPSCOMMON_INC})

add_subdirectory(probes)
