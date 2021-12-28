add_library(aoc_dijkstra INTERFACE)
target_sources(aoc_dijkstra INTERFACE dijkstra.h)
target_include_directories(aoc_dijkstra INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})