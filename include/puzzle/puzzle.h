/*
    Copyright (c) 2021 Thomas Berger <loki@loki.codes> All rights reserved.

    This file is part of Lokis AoC C++ Utilities.

    `AoC C++ Utilities` is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PUZZLE_H
#define PUZZLE_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

#include <cxxopts.hpp>

/**
 * @macro PUZZLE_MAIN
 * @brief Helper macro for a default AoC Puzzle main.
 *
 * This macro creates a `main()` function that will handle all input and
 * run the puzzle.
 *
 * @see runPuzzle
 */
#define PUZZLE_MAIN(year, day, puzzle)                                         \
  int main(int argc, char *argv[]) {                                           \
    return AoC::runPuzzle<puzzle>(year, day, argc, argv);                      \
  };

namespace AoC {

/**
 * @concept Puzzle
 *
 * @brief Requirements for a AoC puzzle.
 *
 * A Puzzle requires the following functions:
 * - Constructor that takes and std::istream reference
 * - a `Part1()` function
 * - a `Part2()` function
 */
template <class T>
concept Puzzle = requires(T t) {
  std::constructible_from<T,std::istream&>;
  t.Part1();
  t.Part2();
};


/**
 * @brief runs a Puzzle.
 *
 * This function is called from main, and does all the handling of input
 * parameters, class instantiation and actually running the Puzzle
 *
 * @tparam P the Puzzle object type
 * @param year the AoC year
 * @param day the AoC day
 * @param argc argc from main
 * @param argv argv from main
 * @return return value for main
 */
template <Puzzle P>
int runPuzzle(int year, int day, int argc, char *argv[]) {
  // prepare our options parser
  std::ostringstream s;
  s << "aoc-" << year << "-" << day;
  const std::string shortDesc{s.str()};
  s.str("");
  s << "AoC puzzle " << day << " (" << year << ")";
  cxxopts::Options options(shortDesc, s.str());

  // clang-format off
  options.add_options()
      ("f,file",  "File name",  cxxopts::value<std::string>()->default_value("input.txt"))
      ("1,part1", "Run Part 1")
      ("2,part2", "Run Part 2")
      ("h,help",  "Print usage")
  ;
  // clang-format on
  auto opts = options.parse(argc, argv);
  if (opts.count("help")) {
    std::cout << options.help() << std::endl;
    std::exit(0);
  }

  auto const filename = opts["file"].as<std::string>();
  std::ifstream input{filename};
  if (!input) {
    std::cerr << "failed to open input file " << filename << std::endl;
    std::exit(1);
  }

  P puzzle(input);
  if (opts.count("part1")) {
    std::cout << "running part 1 ..." << std::endl;
    auto const res = puzzle.Part1();
    std::cout << "part 1 result: " << res << std::endl;
  }

  if (opts.count("part2")) {
    std::cout << "running part 2 ..." << std::endl;
    auto const res = puzzle.Part2();
    std::cout << "part 2 result: " << res << std::endl;
  }
  return 0;
}

} // namespace AoC

#endif // PUZZLE_H
