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

#include <gtest/gtest.h>
#include <dijkstra.h>

#include "sampleGraph.h"

TEST(dijkstra, sample) {
  // create our graph
  Graph g;
  // create our starting node
  Node start{.g = &g};

  // create our Dijkstra container
  AoC::Dijkstra<Node, uint32_t ,30> d(start);

  auto [cost, winner] = d.solve();
  EXPECT_EQ(winner.last,"Bremen");
  EXPECT_EQ(cost, 565);
}