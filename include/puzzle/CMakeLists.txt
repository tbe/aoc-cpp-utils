add_library(aoc_puzzle INTERFACE)
target_sources(aoc_puzzle INTERFACE puzzle.h)
target_link_libraries(aoc_puzzle INTERFACE cxxopts aoc_util)
target_include_directories(aoc_puzzle INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})