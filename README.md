# AoC CPP Utilities

A (growing) collection of my helper libraries, used and developed during AoC.

## Usage

To use this utility repo, i recommend including it via CMake like this:

```cmake
include(FetchContent)
FetchContent_Declare(
        aoc-cpp-utils
        GIT_REPOSITORY git@github.com:tbe/aoc-cpp-utils.git
        GIT_TAG main
)
FetchContent_MakeAvailable(aoc-cpp-utils)
include(${aoc-cpp-utils_SOURCE_DIR}/cmake/AoC-Helpers.cmake)
```

### Write some puzzle code

I didn't like copying main over and over again and duplicate cmake definitions, there are some helpers.

The puzzle solver has to provide the following interface:

```c++
struct example {
    example(std::istream &);
    somethingNotVoid Part1();
    somethingNotVoid Part2();
};
```

Create such a `struct` or `class`, and use the following macro in your CMakeLists.txt:

```cmake
# add_aoc_executable(HEADER CLASS DAY YEAR)
add_aoc_executable(example.h example 01 1999)
```

The resulting target name will be in the form of `aoc-<year>-<day>`. You can use the usual CMake commands to add other
dependencies for linkage, and all the stuff you are expecting to work.

## Utility classes

### Dijkstra

As many puzzles are basically shorted path issues, and i did the same copy'n'paste all over the place, there is a small
class to help with it.

The implementation is just the core-loop and some optimized (but far from perfect) storage for this kind of problems.

To use the class, your node objects have to provide the following interface:

```c++
struct exampleNode {
    std::vector<std::pair<YourCostValueType,exampleNode>> next() const;
    // OR
    std::vector<std::pair<YourCostValueType,exampleNode>> next(std::pmr::memory_resource *) const;
    
    bool completed() const;
};
```

To be precise, any forward iterable container of std::pair will do. You could even have an own type, as long as it has
`first` and `second` member variables that return costs and node objects.

I have designed this interface, because it does not make any assumption on how the core problem is solved. It could be either
self containing states, or a node object could be just a wrapper, that gets it's state from some other object. It doesn't matter.

Have a look in `tests/dijkstra` for such a wrapper example.

The `AoC::Dijkstra` class is a template class, here is an example how to instantiate it:

```c++
AoC::Dijkstra dijkstra<Node,uint32_t,10000>(initalNode, 0);
auto [costs, winner] = dijkstra.solve();
```

### More to come

I have a bunch more helper classes for repeating objects, but i wan't to clean them up, bring them to C++20, and write
some useful tests first before I publish them.