# build Secodefuzz
include(ExternalProject)
set(SECODEFUZZ_PATH ${THIRD_PARTY_DIR}/Secodefuzz)
set(SECODE_FUZZ_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/secode_fuzz)

ExternalProject_Add(secode_fuzz
                    SOURCE_DIR  ${SECODEFUZZ_PATH}
                    BUILD_COMMAND C_INCLUDE_PATH=${SECODEFUZZ_PATH}/examples/xml-lib/libxml2-2.6.26/include:$ENV{C_INCLUDE_PATH} $(MAKE) CC=gcc
                    BINARY_DIR ${SECODE_FUZZ_BINARY_DIR}
                    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy ${SECODE_FUZZ_BINARY_DIR}/libSecodefuzz.so ${TEST_BIN_OUT_DIR}/fuzz/
                    EXCLUDE_FROM_ALL FALSE
)

