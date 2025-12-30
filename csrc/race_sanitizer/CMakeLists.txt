add_subdirectory(alg_framework)
file(GLOB SOURCE "*.cpp")
list(SORT SOURCE)
target_sources(ascend_san PRIVATE ${SOURCE})
