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

#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <array>
#include <concepts>
#include <memory_resource>
#include <queue>
#include <utility>
#include <vector>

namespace AoC {

/**
 * @concept SortablePair
 *
 * @brief A sortable pair, requires the first parameter to fulfill
 * std::totally_ordered.
 */
template <typename T>
concept SortablePair = requires(T t) {
  t.first;
  { t.first } -> std::totally_ordered;
};

/**
 * @concept HasNext
 *
 * @brief Fulfilled if the objects provides a `T.next()` function that returns
 * a SortablePair
 */
template <typename T>
concept HasNext = requires(T t) {
  t.next();
  { t.next() } -> std::ranges::range;
  { *t.next().begin() } -> SortablePair;
};

/**
 * @concept HasPMRNext
 *
 * @brief Fulfilled if the objects provides a `T.next()` function that accepts a
 *  std::pmr::memory_resource and returns a SortablePair
 */
template <typename T>
concept HasPMRNext = requires(T t) {
  t.next(std::declval<std::pmr::memory_resource *>());
  { t.next(std::declval<std::pmr::memory_resource *>()) } -> std::ranges::range;
  {
    *t.next(std::declval<std::pmr::memory_resource *>()).begin()
    } -> SortablePair;
};

/**
 * @concept HasCompleted
 *
 * @brief Fulfilled if the object has a `T.completed()` function that returns a
 * `bool`
 */

template <typename T>
concept HasCompleted = requires(T t) {
  t.completed();
  { t.completed() } -> std::convertible_to<bool>;
};

/**
 * @concept DijkstraNode
 *
 * @brief Requirements for a Dijkstra node object
 *
 * To fulfill this requirements, an object has to implement:
 * - HasNext OR HasPMRNext
 * - HasCompleted
 * - std::is_default_constructible_v
 * - std::movable
 */
template <typename T>
concept DijkstraNode = std::is_default_constructible_v<T> && std::movable<T> &&
    (HasNext<T> || HasPMRNext<T>)&&HasCompleted<T>;

/**
 * @class Dijkstra
 * @brief Simple Dijkstra container with optimized memory management
 *
 * The main goal of this container is, to provide a simple class, that handles
 * the recurring
 * [Dijkstra](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm) patterns
 * found in some AoC Puzzles.
 *
 * It utilizes std::pmr resources and std::array for memory management.
 *
 * @warning Be careful with defining the Complexity parameter. An std::array
 *   with  `(sizeof(Key) + sizeof(T*)) Complexity` will be created. This may
 * smash your heap!
 *
 * @tparam T Node type
 * @tparam Key Key type
 * @tparam Complexity estimated complexity.
 * @tparam ExtraMem additional memory required for a PMR aware `next()`
 */

template <DijkstraNode T, class Key, size_t Complexity, size_t ExtraMem = 0>
class Dijkstra {
  struct heap_t {
    Key first;
    T *second;
    bool operator<(const heap_t &o) const { return o.first < first; }
  };

public:
  /**
   * @brief Constructor.
   *
   * @param node the starting node
   * @param k the costs of the starting node
   */
  explicit Dijkstra(T node, Key k = {})
      : m_heap{std::less<heap_t>(), std::pmr::vector<heap_t>{&m_stack_pool}} {
    push(k, node);
  }

  /**
   * @brief solves the provided issue.
   *
   * Solving is done by:
   * - get the currently cheapest object from the storage
   * - invoke `next()`
   * - check the cost for each returned node
   * - keep nodes that are cheaper then the last "winner"
   * - repeat
   *
   * @return a pair of the costs and a copy of the winning node
   */
  std::pair<const Key, T> solve() {
    while (!m_heap.empty()) {
      auto &ne = m_heap.top();
      auto next = getNext(*(ne.second));
      pop();
      for (auto &n : next) {
        auto &[cost, node] = n;
        if (cost > m_lowest)
          continue;
        if (node.completed()) {
          if (cost < m_lowest) {
            m_lowest = cost;
            std::swap(m_winner, node);
          }
          continue;
        }
        push(cost, node);
      }
    }
    return {m_lowest, m_winner};
  }

private:
  void push(Key cost, T &n) {
    // move our object to the target storage
    auto ptr = m_alloc.template new_object<T>(std::move(n));
    // push it to the heap
    m_heap.emplace(heap_t{.first = cost, .second = ptr});
  }

  void pop() {
    auto obj = m_heap.top();
    m_alloc.delete_object(obj.second);
    m_heap.pop();
  }

  auto getNext(const T &n) requires HasNext<T> { return n.next(); }

  auto getNext(const T &n) requires HasPMRNext<T> {
    return n.next(&m_dyn_pool);
  }

  Key m_lowest{std::numeric_limits<Key>::max()};
  T m_winner{};

  // stack memory for the priority queue
  // We use the stack here, at it is continuous area, that should fit in the CPU
  // cache as a whole This makes the heap operations a lot faster
  std::array<std::byte, Complexity * sizeof(heap_t)> m_stack_arena{};
  std::pmr::monotonic_buffer_resource m_stack_buff{m_stack_arena.data(),
                                                   m_stack_arena.size()};
  std::pmr::unsynchronized_pool_resource m_stack_pool{&m_stack_buff};
  std::priority_queue<heap_t, std::pmr::vector<heap_t>> m_heap;

  // dynamic memory pools are used for the node objects and for the next
  // function if they support it. our operations on this memory area are much
  // less optimized, but there is no need to, as it is limited to creation and
  // deletion of objects (at least for the code within the Dijkstra
  // implementation)
  std::pmr::monotonic_buffer_resource m_dyn_buff{(sizeof(T) * Complexity) +
                                                 ExtraMem};
  std::pmr::unsynchronized_pool_resource m_dyn_pool{&m_dyn_buff};
  std::pmr::polymorphic_allocator<T> m_alloc{&m_dyn_pool};
};

} // namespace AoC

#endif // DIJKSTRA_H
