include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_subdirectory(puzzle)
add_subdirectory(dijkstra)
