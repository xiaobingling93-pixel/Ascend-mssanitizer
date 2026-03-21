add_subdirectory(dbi)

find_program(BISHENG_COMPILER bisheng)
if(NOT BISHENG_COMPILER)
    message(FATAL_ERROR "Compiler bisheng is not installed or not in PATH")
endif()

# setup toolchain for ccec_stub compiling
set(CMAKE_CXX_COMPILER bisheng)
set(CMAKE_LINKER llvm-ar)
set(CMAKE_CXX_LINK_FLAGS "")
set(CMAKE_CXX_FLAGS "")
if(${BUILD_TRACEKIT})
  # make intrinsics stub export tracekit prefix
  set(CMAKE_CXX_FLAGS "-DBUILD_TRACEKIT")
endif()

# set aicore arch list for sanitizer stub
set(CCE_AICORE_ARCH_LIST
  "dav-c220-vec"
  "dav-c220-cube"
  "dav-c220"
  "dav-m200"
  "dav-m200-vec"
)

message(STATUS "Sanitizer stub built for: ${CCE_AICORE_ARCH_LIST}")

file(GLOB_RECURSE SANITIZER_STUB_FILES *.cpp)
file(GLOB_RECURSE DBI_SOURCES dbi/*.cpp)

list(REMOVE_ITEM SANITIZER_STUB_FILES ${DBI_SOURCES})

# @description compile sanitizer stub files to virtual object
function(compile_cce_object CCE_AICORE_ARCH SANITIZER_STUB_OBJECT SANITIZER_STUB_FILES)
  set(SANITIZER_COMPILE_FLAGS
    "-xcce -std=c++17 -O2 --cce-aicore-only --cce-aicore-arch=${CCE_AICORE_ARCH} \
    -mllvm -cce-aicore-record-overflow=false \
    -mllvm -cce-aicore-record-stack-size=false \
    --cce-use-legacy-mixkernel-mangling \
    -mllvm -cce-aicore-addr-transform -mllvm -cce-aicore-long-call -mllvm -cce-aicore-relax -fPIC -pthread"
  )
  add_library(${SANITIZER_STUB_OBJECT} OBJECT ${SANITIZER_STUB_FILES})
  target_include_directories(${SANITIZER_STUB_OBJECT} PUBLIC
    ${CSRC_INC_DIR}
    ${SANITIZER_THIRDPARTY_INCLUDE}
    ${SANITIZER_INC_DIR}
  )
  # remove secure flags from parent project
  set_target_properties(${SANITIZER_STUB_OBJECT} PROPERTIES COMPILE_OPTIONS "")
  set_target_properties(${SANITIZER_STUB_OBJECT} PROPERTIES COMPILE_FLAGS ${SANITIZER_COMPILE_FLAGS})
endfunction()

foreach(CCE_AICORE_ARCH ${CCE_AICORE_ARCH_LIST})
  set(SANITIZER_STUB_LIB sanitizer_stub_${CCE_AICORE_ARCH})
  if(${CCE_AICORE_ARCH} STREQUAL "dav-c220")
    set(LOAD_STORE_STUB_FILE "./move_instructions/load_store.cpp")
    set(SANITIZER_STUB_OBJECT sanitizer_stub_object_${CCE_AICORE_ARCH})
    set(SANITIZER_STUB_LOAD_STORE_VEC sanitizer_stub_load_store_dav-c220-vec)
    set(SANITIZER_STUB_LOAD_STORE_CUBE sanitizer_stub_load_store_dav-c220-cube)
    compile_cce_object(${CCE_AICORE_ARCH} ${SANITIZER_STUB_OBJECT} "${SANITIZER_STUB_FILES}")
    compile_cce_object("dav-c220-vec" ${SANITIZER_STUB_LOAD_STORE_VEC} ${LOAD_STORE_STUB_FILE})
    compile_cce_object("dav-c220-cube" ${SANITIZER_STUB_LOAD_STORE_CUBE} ${LOAD_STORE_STUB_FILE})
    add_library(${SANITIZER_STUB_LIB} STATIC
      $<TARGET_OBJECTS:${SANITIZER_STUB_OBJECT}>
      $<TARGET_OBJECTS:${SANITIZER_STUB_LOAD_STORE_VEC}>
      $<TARGET_OBJECTS:${SANITIZER_STUB_LOAD_STORE_CUBE}>
    )
  else()
    set(SANITIZER_STUB_OBJECT sanitizer_stub_object_${CCE_AICORE_ARCH})
    compile_cce_object(${CCE_AICORE_ARCH} ${SANITIZER_STUB_OBJECT} "${SANITIZER_STUB_FILES}")
    add_library(${SANITIZER_STUB_LIB} STATIC $<TARGET_OBJECTS:${SANITIZER_STUB_OBJECT}>)
  endif()
  install(TARGETS ${SANITIZER_STUB_LIB} ARCHIVE DESTINATION lib64)
endforeach()
