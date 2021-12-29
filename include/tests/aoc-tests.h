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

#ifndef AOC_TESTS_H
#define AOC_TESTS_H
#include <gtest/gtest.h>

#define TEST_PART(PART, CLASS, INPUT, OUTPUT) \
{                                             \
  std::istringstream s{INPUT};                \
  CLASS c(s);                                 \
  EXPECT_EQ(c. PART (), OUTPUT);              \
}

/**
 * @macro TEST_PART1
 * @brief Helper macro to test Part 1 of a puzzle
 */
#define TEST_PART1(CLASS, INPUT, OUTPUT) TEST_PART(Part1,CLASS,INPUT,OUTPUT)

/**
 * @macro TEST_PART2
 * @brief Helper macro to test Part 2 of a puzzle
 */
#define TEST_PART2(CLASS, INPUT, OUTPUT) TEST_PART(Part2,CLASS,INPUT,OUTPUT)

#endif // AOC_TESTS_H