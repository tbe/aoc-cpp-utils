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

#ifndef NUMERICGRID_H
#define NUMERICGRID_H

#include <cstdint>
#include <istream>
#include <sstream>
#include <vector>

/**
 * operator to load the a row from a std::istream
 */
inline std::istream &operator>>(std::istream &is, std::vector<uint_fast8_t> &vec) {
  while (is && is.peek() != std::istream::traits_type::eof())
    vec.push_back(is.get() - '0');
  return is;
}

namespace AoC {

/**
 * @brief container for AoC typical numeric grids.
 *
 * It stores the input in a two dimensional vector, and provides some basic access functionality, like iterators and column views.
 *
 * @tparam T the storage type for each number. While they are always 8bit (one digit per cell), one may use another storage type in certain cases
 */
template <typename T = uint_fast8_t> class numericGrid {
public:
  template <typename vT> class Iterator;
  class ColumnView;

  /**
   * @brief access a specific row
   * @param idx row to access
   * @return the row vector
   */
  std::vector<T> &operator[](std::size_t idx) { return m_grid[idx]; }
  const std::vector<T> &operator[](std::size_t idx) const { return m_grid[idx]; }

  ///@{
  /**
   * @brief get a value by position
   * @param idx a pair of x,y
   * @return a reference to the value
   */
  T &operator[](const std::pair<size_t, size_t> &idx) {
    // TODO: bounds check?
    //   std::vector does no bounds checking, should we?
    return m_grid[idx.second][idx.first];
  }

  const T &operator[](const std::pair<size_t, size_t> &idx) const {
    // TODO: bounds check?
    //   std::vector does no bounds checking, should we?
    return m_grid[idx.second][idx.first];
  }
  ///@}

  /**
   * @brief get a view for a specific column
   * @param idx the column
   */
  ColumnView column(std::size_t idx) {
    return { this,idx };
  }

  ///@{
  /**
   * @brief get an iterator pointing to the start of the grid
   */
  Iterator<T> begin() { return Iterator<T>{this}; }
  [[nodiscard]] Iterator<const T> begin() const { return Iterator<const T>{this}; }
  [[nodiscard]] Iterator<const T> cbegin() const { return begin(); }
  ///@}

  ///@{
  /**
   * @brief get the end sentinel for the iterator
   */
  static std::default_sentinel_t end() { return {}; }
  static std::default_sentinel_t cend() { return {}; }
  ///@}

  /**
   * @brief get the number of elements.
   *
   * @warning It is assumed, that the each row inside the grid has the same
   * length. This is a very save assumption for AoC, but in the "real world" ...
   *
   * @return the number of elements inside the vector
   */
  std::size_t size() const {
    if (m_grid.empty())
      return 0;
    return m_grid.size() * m_grid[0].size();
  }

  /**
   * @brief get the number of rows (y).
   */
  std::size_t rows() const { return m_grid.size(); }

  /**
   * @brief the the number of columns (x).
   *
   * @see size()
   */
  std::size_t columns() const {
    if (m_grid.empty())
      return 0;
    return m_grid[0].size();
  }

  /** operator to load a whole grid from a std::istream */
  friend std::istream &operator>>(std::istream &is, numericGrid &map) {
    std::string line;
    while (std::getline(is, line)) {
      std::vector<uint_fast8_t> row;
      std::istringstream s{line};
      s >> row;
      map.m_grid.emplace_back(std::move(row));
    }
    return is;
  }

private:
  template <typename vT> friend class Iterator;
  std::vector<std::vector<T>> m_grid;
};

template <typename T> template <typename vT> class numericGrid<T>::Iterator {
public:
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = vT;
  using pointer = vT *;
  using reference = vT &;

  Iterator() = default;

  Iterator(const Iterator &) = default;
  Iterator(Iterator &&) noexcept = default;

  Iterator &operator=(const Iterator &) = default;
  Iterator &operator=(Iterator &&) noexcept = default;

  reference operator*() const { return *m_cIt; }
  pointer operator->() { return &(*m_cIt); }

  bool operator==(const Iterator &o) const { return m_cIt == o.m_cIt; }
  bool operator==(std::default_sentinel_t) const { return m_rowsIt == m_rowsEnd && m_cIt == m_cEnd; }
  bool operator<(const Iterator &o) const { return m_rowsIt == o.m_rowsIt ? m_cIt < o.m_cIt : m_rowsIt < o.m_rowsIt; }

  Iterator &operator++() {
    ++m_cIt;
    if (m_cIt == m_cEnd) {
      ++m_rowsIt;
      if (m_rowsIt != m_rowsEnd) {
        m_cIt = m_rowsIt->begin();
        m_cEnd = m_rowsIt->end();
      }
    }
    return *this;
  }

  Iterator operator++(int) {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }

private:
  friend class numericGrid<T>;
  explicit Iterator(numericGrid<T> *parent)
      : m_parent{parent}, m_rowsIt{parent->m_grid.begin()}, m_rowsEnd{parent->m_grid.end()}, m_cIt{parent->m_grid.begin()->begin()},
        m_cEnd{parent->m_grid.begin()->end()} {}

  numericGrid<T> *m_parent{};

  typename std::vector<std::vector<T>>::iterator m_rowsIt{}, m_rowsEnd{};
  typename std::vector<T>::iterator m_cIt{}, m_cEnd{};
};

template <typename T> class numericGrid<T>::ColumnView {
public:
  template <typename vT> class Iterator {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = vT;
    using pointer = vT *;
    using reference = vT &;

    Iterator() = default;

    Iterator(const Iterator &) = default;
    Iterator(Iterator &&) noexcept = default;

    Iterator &operator=(const Iterator &) = default;
    Iterator &operator=(Iterator &&) noexcept = default;

    bool operator==(const Iterator &o) const { return m_cIt == o.m_cIt; }
    bool operator==(std::default_sentinel_t) const { return m_cIt == m_end; }
    bool operator<(const Iterator &o) const { return m_cIt < o.m_cIt; }

    reference operator*() const { return (*m_cIt)[m_column]; }
    pointer operator->() { return &((*m_cIt)[m_column]); }

    Iterator &operator++() {
      ++m_cIt;
      return *this;
    }

    Iterator operator++(int) {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

  private:
    friend class ColumnView;
    Iterator(const typename std::vector<std::vector<T>>::iterator &begin, const typename std::vector<std::vector<T>>::iterator &end, std::size_t column)
        : m_cIt{begin}, m_end{end},m_column{column} {}

    typename std::vector<std::vector<T>>::iterator m_cIt{}, m_end{};
    size_t m_column;
  };

  ColumnView(const ColumnView &) = default;
  ColumnView(ColumnView &&) noexcept = default;

  ColumnView &operator=(const ColumnView &) = default;
  ColumnView &operator=(ColumnView &&) noexcept = default;

  T &operator[](std::size_t idx) { return m_parent->m_grid[idx][m_column]; }

  const T &operator[](std::size_t idx) const { return m_parent->m_grid[idx][m_column]; }

  Iterator<T> begin() { return Iterator<T>{m_parent->m_grid.begin(), m_parent->m_grid.end(), m_column}; }
  [[nodiscard]] Iterator<const T> begin() const { return {m_parent->m_grid.begin(), m_parent->m_grid.end(), m_column}; }
  [[nodiscard]] Iterator<const T> cbegin() const { return begin(); }

  static std::default_sentinel_t end() { return {}; }

private:
  friend class numericGrid<T>;
  ColumnView(numericGrid<T> *parent, std::size_t column) : m_parent{parent}, m_column{column} {}

  numericGrid<T> *m_parent{};
  std::size_t m_column{};
};

static_assert(std::forward_iterator<numericGrid<uint_fast8_t>::Iterator<uint_fast8_t>>);
static_assert(std::forward_iterator<numericGrid<uint_fast8_t>::Iterator<const uint_fast8_t>>);
static_assert(std::forward_iterator<numericGrid<uint_fast8_t>::ColumnView::Iterator<const uint_fast8_t>>);

static_assert(std::ranges::forward_range<numericGrid<uint_fast8_t>>);
static_assert(std::ranges::forward_range<numericGrid<uint_fast8_t>::ColumnView>);

} // namespace AoC

#endif // NUMERICGRID_H
