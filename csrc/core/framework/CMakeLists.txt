# build record_format as dependency of other tools. never install to output
set(SOURCE
  record_format.cpp
  file_mapping.cpp
)
add_library(record_format STATIC)
target_sources(record_format PRIVATE ${SOURCE})
target_include_directories(record_format PUBLIC ${SECUREC_INC_DIR} ${CSRC_INC_DIR})

# check if any resolved symbol in record_format library
add_library(record_format_shared SHARED)
target_sources(record_format_shared PRIVATE ${SOURCE})
target_include_directories(record_format_shared PUBLIC ${SECUREC_INC_DIR} ${CSRC_INC_DIR})
target_link_options(record_format_shared PRIVATE "-Wl,-z,defs")
