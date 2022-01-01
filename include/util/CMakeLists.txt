add_library(aoc_util INTERFACE)
target_sources(aoc_util INTERFACE numericGrid.h)
target_include_directories(aoc_util INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})