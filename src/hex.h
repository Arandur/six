#pragma once

#include <array>
#include <cstdint>
#include <optional>

namespace hex
{
  struct coord
  {
    constexpr coord(uint8_t x, uint8_t y) noexcept :
      x(x), y(y)
    {}

    constexpr auto operator = (const coord& rhs) noexcept
    {
      x = rhs.x;
      y = rhs.y;
    }

    /**
     * We take advantage of unsigned integer overflow, which is well-defined
     * in C++. This turns hex-space into a toroidal lattice. The size of the
     * lattice doesn't really matter for the purposes of our game, as long
     * as it's at least 43 cells wide — otherwise, if someone placed 42
     * pieces in a row, the two pieces at the end would register as neighbors.
     * Given that we don't care exactly how large the lattice is, for the
     * sake of computational efficiency we make it 256 cells wide.
     */
    uint8_t x, y;

    constexpr auto ne() const noexcept -> coord 
    { 
      return { static_cast<uint8_t>(x + 1), y }; 
    }

    constexpr auto  e() const noexcept -> coord 
    { 
      return { static_cast<uint8_t>(x + 1), static_cast<uint8_t>(y - 1) }; 
    }

    constexpr auto se() const noexcept -> coord 
    { 
      return { x, static_cast<uint8_t>(y - 1) }; 
    }

    constexpr auto sw() const noexcept -> coord 
    { 
      return { static_cast<uint8_t>(x - 1), y }; 
    }

    constexpr auto  w() const noexcept -> coord 
    { 
      return { static_cast<uint8_t>(x - 1), static_cast<uint8_t>(y + 1) }; 
    }

    constexpr auto nw() const noexcept -> coord 
    { 
      return { x, static_cast<uint8_t>(y + 1) }; 
    }

    constexpr void neighbors(std::array<coord, 6>& arr) const noexcept
    {
      arr[0] = ne();
      arr[1] =  e();
      arr[2] = se();
      arr[3] = sw();
      arr[4] =  w();
      arr[5] = nw();
    }
  };

  enum class color : uint8_t
  {
    RED = 1,
    BLACK = 2
  };

  inline void swap(color& lhs, color& rhs)
  {
    using std::swap;
    swap(lhs, rhs);
  }
}

namespace std
{
  template <>
  class optional<hex::color>
  {
  public:
    constexpr optional() noexcept : _color(static_cast<hex::color>(0)) {}
    constexpr optional(nullopt_t) noexcept : _color(static_cast<hex::color>(0)) {}
    constexpr optional(const optional<hex::color>& rhs) noexcept : 
      _color(rhs._color) 
    {}
    constexpr optional(optional<hex::color>&& rhs) noexcept :
      _color(move(rhs._color))
    {}
    template <class... Args>
    constexpr optional(in_place_t, Args&&... args) noexcept :
      _color(forward<Args&&>(args)...)
    {}

    auto operator = (nullopt_t) noexcept -> optional<hex::color>&
    {
      _color = static_cast<hex::color>(0);
      return *this;
    }

    auto operator = (const optional<hex::color>& rhs) noexcept -> optional<hex::color>&
    {
      _color = rhs._color;
      return *this;
    }

    auto operator = (optional<hex::color>&& rhs) noexcept -> optional<hex::color>&
    {
      _color = move(rhs._color);
      return *this;
    }

    auto operator = (const hex::color& value) noexcept -> optional<hex::color>&
    {
      _color = value;
      return *this;
    }

    auto operator = (hex::color&& value) noexcept -> optional<hex::color>&
    {
      _color = move(value);
      return *this;
    }

    constexpr auto operator -> () const noexcept -> const hex::color*
    {
      return &_color;
    }

    constexpr auto operator -> () noexcept -> hex::color*
    {
      return &_color;
    }

    constexpr auto operator * () const & noexcept -> const hex::color&
    {
      return _color;
    }

    constexpr auto operator * () & noexcept -> hex::color&
    {
      return _color;
    }

    constexpr auto operator * () const && noexcept -> const hex::color&&
    {
      return move(_color);
    }

    constexpr auto operator * () && noexcept -> hex::color&&
    {
      return move(_color);
    }

    constexpr explicit operator bool() const noexcept
    {
      return static_cast<uint8_t>(_color) != 0;
    }

    constexpr auto has_value() const noexcept -> bool
    {
      return static_cast<bool>(*this);
    }

    constexpr auto value() const & -> const hex::color&
    {
      if (*this)
      {
        return _color;
      }
      else
      {
        throw bad_optional_access{};
      }
    }

    constexpr auto value() & -> hex::color&
    {
      if (*this)
      {
        return _color;
      }
      else
      {
        throw bad_optional_access{};
      }
    }

    constexpr auto value() const && -> const hex::color&&
    {
      if (*this)
      {
        return move(_color);
      }
      else
      {
        throw bad_optional_access{};
      }
    }

    constexpr auto value() && -> hex::color&&
    {
      if (*this)
      {
        return move(_color);
      }
      else
      {
        throw bad_optional_access{};
      }
    }

    constexpr auto value_or(hex::color&& default_value) const & -> hex::color
    {
      if (*this)
      {
        return _color;
      }
      else
      {
        return default_value;
      }
    }

    constexpr auto value_or(hex::color&& default_value) && -> hex::color
    {
      if (*this)
      {
        return move(_color);
      }
      else
      {
        return default_value;
      }
    }

    void swap(optional<hex::color>& rhs) noexcept
    {
      using hex::swap;

      swap(_color, rhs._color);
    }

    void reset() noexcept
    {
      _color = static_cast<hex::color>(0);
    }

    template <class... Args>
    auto emplace(Args&&... args) -> hex::color&
    {
      return (_color = hex::color(forward<Args&&>(args)...));
    }

  private:
    hex::color _color;
  };
}
