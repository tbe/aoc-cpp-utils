/*
    Copyright (c) 2022 Thomas Berger <loki@loki.codes> All rights reserved.

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

#include "numericGrid.h"
#include <gtest/gtest.h>

TEST(numericGrid, default) {
  static const char *test_data = "111\n"
                                 "222\n"
                                 "333\n"
                                 "444";

  AoC::numericGrid<uint_fast8_t > grid;
  std::istringstream s{test_data};
  s >> grid;
  EXPECT_EQ(grid.size(),12);

  std::vector<uint_fast8_t > tmp;
  std::ranges::copy(grid,std::back_inserter(tmp));
  EXPECT_EQ(tmp.size(), grid.size());

  tmp.clear();
  auto v = grid.column(0);
  std::ranges::copy(v,std::back_inserter(tmp));
  EXPECT_EQ(tmp,(std::vector<uint_fast8_t>{1,2,3,4}));
}