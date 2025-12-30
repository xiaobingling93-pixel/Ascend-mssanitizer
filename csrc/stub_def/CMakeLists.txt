file(GLOB STUB_SOURCE "*.cpp" ${FRAMEWORK_INC_DIR}/record_format.cpp)
list(SORT STUB_SOURCE)
target_sources(ascend_san_stub PRIVATE ${STUB_SOURCE})
