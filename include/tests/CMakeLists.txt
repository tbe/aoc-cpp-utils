add_library(aoc_test INTERFACE)
target_sources(aoc_test INTERFACE aoc-tests.h)
target_link_libraries(aoc_test INTERFACE gtest_main)
target_include_directories(aoc_test INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})