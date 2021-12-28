function(add_aoc_executable HEADER CLASS DAY YEAR)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/puzzle-${year}-${day}.cpp             "#include <puzzle.h>
#include \"${CMAKE_CURRENT_SOURCE_DIR}/${HEADER}\"

 PUZZLE_MAIN(${YEAR}, ${DAY}, ${CLASS})")
    add_executable(aoc-${year}-${day} ${CMAKE_CURRENT_BINARY_DIR}/puzzle.cpp)
    target_link_libraries(aoc-${year}-${day} aoc_puzzle)
endfunction()