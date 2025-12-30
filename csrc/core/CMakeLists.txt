add_subdirectory(framework)

file(GLOB_RECURSE SOURCE "*.cpp")
list(SORT SOURCE)
target_sources(ascend_san PRIVATE ${SOURCE})
