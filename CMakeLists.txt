cmake_minimum_required(VERSION 3.21)
project(aoc-cpp-utils VERSION 1.0.0
        DESCRIPTION "Some AoC C++ Helper Classes")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include(FetchContent)

# configuration
IF(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(BUILD_TEST TRUE)
ELSE()
    set(BUILD_TEST FALSE)
ENDIF()

#### Doxygen configuration
find_package(Doxygen REQUIRED dot)
FetchContent_Declare(
        cppreference-book
        URL https://github.com/PeterFeicht/cppreference-doc/releases/download/v20211127/html-book-20211127.tar.xz
)

FetchContent_GetProperties(cppreference-book)
if(NOT cppreference-book_POPULATED)
    FetchContent_Populate(cppreference-book)
endif()
SET(DOXYGEN_TAGFILES ${cppreference-book_SOURCE_DIR}/cppreference-doxygen-web.tag.xml=http://en.cppreference.com/w/
        ${CMAKE_CURRENT_LIST_DIR}/doc/extra.tag.xml=http://en.cppreference.com/w/)
SET(DOXYGEN_GENERATE_TAGFILE ${CMAKE_CURRENT_BINARY_DIR}/aoc-cpp.tag.xml)
SET(DOXYGEN_HIDE_SCOPE_NAMES YES)
SET(DOXYGEN_SKIP_FUNCTION_MACROS NO)
doxygen_add_docs(doc include doc)
#### end of doxygen

#### cxxopts
FetchContent_Declare(
        cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
        GIT_TAG v3.0.0
)
FetchContent_MakeAvailable(cxxopts)

add_subdirectory(include)

include(${PROJECT_SOURCE_DIR}/cmake/AoC-Helpers.cmake)

if(BUILD_TEST)
    enable_testing()
    add_subdirectory(test)
endif()

include_directories(${CMAKE_CURRENT_LIST_DIR})