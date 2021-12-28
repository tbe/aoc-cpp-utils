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

#ifndef SAMPLEGRAPH_H
#define SAMPLEGRAPH_H

#include <map>
#include <vector>

struct Graph {
  Graph() {
    // fill our graph with some distances
    distances["Berlin"] = {
        {"Essen", 530}, {"Stuttgart", 630}, {"Dortmund", 491}};
    distances["Essen"] = {
        {"Stuttgart", 430}, {"Dortmund", 38}, {"Bremen", 255}};
    distances["Stuttgart"] = {
        {"Dortmund", 417}, {"Bremen", 638}, {"Mannheim", 133}};
    distances["Dortmund"] = {
        {"Bremen", 234}, {"Mannheim", 294}, {"Berlin", 492}};
    distances["Bremen"] = {{"Mannheim", 512}, {"Berlin", 390}, {"Essen", 248}};
    distances["Mannheim"] = {
        {"Berlin", 624}, {"Essen", 310}, {"Stuttgart", 140}};
  }
  std::map<std::string, std::map<std::string, uint32_t>> distances;
};

struct Node {
  Graph *g;

  std::string last{"Mannheim"};
  std::vector<std::string> route;
  uint32_t costs{0};

  std::vector<std::pair<uint32_t, Node>> next() const {
    if (last == "Bremen") {
      return {};
    }
    std::vector<std::pair<uint32_t, Node>> out;
    for (auto const &n : g->distances[last]) {
      Node node(*this);
      node.last = n.first;
      node.costs += n.second;
      node.route.push_back(last);
      auto const c = node.costs;
      out.emplace_back(c, std::move(node));
    }
    return std::move(out);
  };

  bool completed() const { return last == "Bremen"; }
};

#endif // SAMPLEGRAPH_H
