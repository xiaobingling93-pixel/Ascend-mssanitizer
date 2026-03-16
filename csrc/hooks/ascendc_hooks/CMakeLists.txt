add_library(ascendc_hook SHARED ascendc_hooks.cpp)

target_sources(ascendc_hook PRIVATE
  ${FRAMEWORK_INC_DIR}/communication.cpp
  ${FRAMEWORK_INC_DIR}/utility/domain_socket.cpp
  ${FRAMEWORK_INC_DIR}/utility/result.cpp
  ${FRAMEWORK_INC_DIR}/utility/log.cpp
  ${FRAMEWORK_INC_DIR}/utility/cmd.cpp
  ${FRAMEWORK_INC_DIR}/utility/file_system.cpp
  ${FRAMEWORK_INC_DIR}/utility/ustring.cpp
  ${FRAMEWORK_INC_DIR}/utility/path.cpp
  ${FRAMEWORK_INC_DIR}/record_format.cpp
  ${FRAMEWORK_INC_DIR}/file_mapping.cpp
  ${HOOK_INC_DIR}/hook_report.cpp
  ${HOOK_INC_DIR}/basic_hooks.cpp
  ${HOOK_INC_DIR}/runtime_hooks.cpp
  ${HOOK_INC_DIR}/sanitizer_hooks.cpp
)
target_include_directories(ascendc_hook PUBLIC
  ${FRAMEWORK_INC_DIR}
  ${CSRC_INC_DIR}
  ${SECUREC_INC_DIR}
  ${HOOK_INC_DIR}
  ${SANITIZER_THIRDPARTY_INCLUDE}
  ${SANITIZER_INC_DIR}
)
target_link_libraries(ascendc_hook c_sec dl ascend_hal pthread)
target_link_options(ascendc_hook PRIVATE "-Wl,-z,defs")
if(${BUILD_TRACEKIT})
  target_compile_definitions(ascendc_hook PRIVATE BUILD_TRACEKIT)
endif()
