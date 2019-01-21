#pragma once

#include "hex.h"

#include <array>
#include <stdexcept>

namespace hex
{
class bad_piece_placement : public std::exception
{
public:
  auto what() const noexcept -> const char*
  {
    return "bad piece placement";
  }
};

class board
{
public:
  constexpr board() {}

  constexpr auto get_color(const hex::coord& c) const noexcept -> std::optional<hex::color>
  {
    return get(c.x, c.y);
  }

  void set_color(const hex::coord& c, const hex::color& color)
  {
    auto& space = get(c.x, c.y);

    if (space)
    {
      throw bad_piece_placement{};
    }

    space = color;
  }

private:
  std::array<std::optional<hex::color>, 4096> _colors;

  constexpr auto get(uint8_t x, uint8_t y) noexcept -> std::optional<hex::color>&
  {
    return _colors[(x % 64) * 64 + (y % 64)];
  }

  constexpr auto get(uint8_t x, uint8_t y) const noexcept -> const std::optional<hex::color>&
  {
    return _colors[(x % 64) * 64 + (y % 64)];
  }
};
}
