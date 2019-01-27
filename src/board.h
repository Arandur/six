#pragma once

#include "hex.h"

#include <cstdint>
#include <stdexcept>
#include <vector>

namespace hex
{
struct piece_t
{
  coord_t coord;
  color_t color;
};

struct bad_placement : public std::exception
{
  auto what() const noexcept -> const char*
  {
    return "bad placement";
  }
};

struct bad_removal : public std::exception
{
  auto what() const noexcept -> const char*
  {
    return "bad removal";
  }
};

class board_t
{
public:
  class iterator
  {
  public:
    using value_type = piece_t;
    using difference_type = int8_t;
    using reference = const piece_t&;
    using pointer = const piece_t*;
    using iterator_category = std::random_access_iterator_tag;

    iterator (const iterator&) = default;
    auto operator = (const iterator&) -> iterator& = default;

    auto operator * () const noexcept -> reference
    {
      return *_piece;
    }

    auto operator -> () const noexcept -> pointer
    {
      return _piece;
    }

    auto operator [] (const difference_type& n) const noexcept -> reference
    {
      return _piece[n];
    }

    auto operator ++ () noexcept -> iterator&
    {
      ++_piece;
      return *this;
    }

    auto operator ++ (int) noexcept -> iterator
    {
      auto ret = *this;
      operator ++ ();
      return ret;
    }

    auto operator -- () noexcept -> iterator&
    {
      --_piece;
      return *this;
    }

    auto operator -- (int) noexcept -> iterator
    {
      auto ret = *this;
      operator -- ();
      return ret;
    }

    auto operator += (const difference_type& n) noexcept -> iterator&
    {
      _piece += n;
      return *this;
    }

    auto operator -= (const difference_type& n) noexcept -> iterator&
    {
      _piece -= n;
      return *this;
    }

    auto operator -  (const iterator& rhs) const noexcept -> difference_type
    {
      return static_cast<difference_type>(_piece - rhs._piece);
    }

    auto operator == (const iterator& rhs) const noexcept -> bool
    {
      return _piece == rhs._piece;
    }

    auto operator != (const iterator& rhs) const noexcept -> bool
    {
      return _piece != rhs._piece;
    }

    auto operator <  (const iterator& rhs) const noexcept -> bool
    {
      return _piece <  rhs._piece;
    }

    auto operator <= (const iterator& rhs) const noexcept -> bool
    {
      return _piece <= rhs._piece;
    }

    auto operator >  (const iterator& rhs) const noexcept -> bool
    {
      return _piece >  rhs._piece;
    }

    auto operator >= (const iterator& rhs) const noexcept -> bool
    {
      return _piece >= rhs._piece;
    }

    void swap(iterator& rhs)
    {
      using std::swap;
      swap(_piece, rhs._piece);
    }

  private:
    pointer _piece;

    constexpr explicit iterator(pointer piece) noexcept :
      _piece(piece)
    {}

    friend class board_t;
  };

  using const_iterator = iterator;

  static auto init() noexcept -> board_t
  {
    board_t board;

    board._pieces[0] = {{ -2, 0 }, color_t::RED};
    board._pieces[1] = {{  2, 0 }, color_t::BLACK};

    board.n_pieces = 2;

    return board;
  }

  auto at(const coord_t& coord) -> std::optional<color_t>
  {
    for (uint8_t i = 0; i < n_pieces; ++i)
    {
      if (coord == _pieces[i].coord)
      {
        return std::make_optional(_pieces[i].color);
      }
    }

    return {};
  }

  void add(const coord_t& coord, const color_t& color)
  {
    int16_t min_x = coord.x;
    int16_t max_x = coord.x;
    int16_t min_y = coord.y;
    int16_t max_y = coord.y;

    for (uint8_t i = 0; i < n_pieces; ++i)
    {
      if (coord == _pieces[i].coord)
      {
        throw bad_placement{};
      }

      min_x = std::min(min_x, _pieces[i].coord.x);
      max_x = std::max(max_x, _pieces[i].coord.x);
      min_y = std::min(min_y, _pieces[i].coord.y);
      max_y = std::max(max_y, _pieces[i].coord.y);
    }

    _pieces[n_pieces] = { coord, color };
    ++n_pieces;

    for (uint8_t i = 0; i < n_pieces; ++i)
    {
      _pieces[i].coord.x -= (max_x + min_x) / 2;
      _pieces[i].coord.y -= (max_y + min_y) / 2;
    }
  }

  void remove(const coord_t& coord)
  {
    uint8_t i;
    for (i = 0; i < n_pieces; ++i)
    {
      if (coord == _pieces[i].coord)
      {
        for (uint8_t j = i + 1; j < n_pieces; ++j)
        {
          _pieces[j - 1] = _pieces[j];
        }

        --n_pieces;
        break;
      }
    }

    if (i == n_pieces)
    {
      throw bad_removal{};
    }

    int16_t min_x = std::numeric_limits<int16_t>::max();
    int16_t max_x = std::numeric_limits<int16_t>::min();
    int16_t min_y = std::numeric_limits<int16_t>::max();
    int16_t max_y = std::numeric_limits<int16_t>::min();


    for (uint8_t i = 0; i < n_pieces; ++i)
    {
      min_x = std::min(min_x, _pieces[i].coord.x);
      max_x = std::max(max_x, _pieces[i].coord.x);
      min_y = std::min(min_y, _pieces[i].coord.y);
      max_y = std::max(max_y, _pieces[i].coord.y);
    }

    for (uint8_t i = 0; i < n_pieces; ++i)
    {
      _pieces[i].coord.x -= (max_x + min_x) / 2;
      _pieces[i].coord.y -= (max_y + min_y) / 2;
    }
  }

  auto begin() const noexcept -> iterator
  {
    return iterator(_pieces);
  }

  auto cbegin() const noexcept -> iterator
  {
    return begin();
  }

  auto end() const noexcept -> iterator
  {
    return iterator(_pieces + n_pieces);
  }

  auto cend() const noexcept -> iterator
  {
    return end();
  }

private:
  piece_t _pieces[42];
  uint8_t n_pieces;

  board_t() = default;
};

inline auto operator + (board_t::iterator it, board_t::iterator::difference_type n) noexcept -> board_t::iterator
{
  return it += n;
}

inline auto operator + (board_t::iterator::difference_type n, board_t::iterator it) noexcept -> board_t::iterator
{
  return it += n;
}

inline auto operator - (board_t::iterator it, board_t::iterator::difference_type n) noexcept -> board_t::iterator
{
  return it -= n;
}
}
