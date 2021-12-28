function(add_aoc_executable HEADER CLASS DAY YEAR)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/puzzle-${YEAR}-${DAY}.cpp             "#include <puzzle.h>
#include \"${CMAKE_CURRENT_SOURCE_DIR}/${HEADER}\"

PUZZLE_MAIN(${YEAR}, ${DAY}, ${CLASS})")
    add_executable(aoc-${YEAR}-${DAY} ${CMAKE_CURRENT_BINARY_DIR}/puzzle-${YEAR}-${DAY}.cpp)
    target_link_libraries(aoc-${YEAR}-${DAY} aoc_puzzle)
endfunction()