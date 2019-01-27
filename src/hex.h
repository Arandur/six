#pragma once

#include <array>
#include <cstdint>
#include <optional>

namespace hex
{
  struct coord_t
  {
    int16_t x, y;

    constexpr coord_t() noexcept :
      x(0), y(0)
    {}

    constexpr coord_t(int16_t x, int16_t y) noexcept :
      x(x), y(y)
    {}

    constexpr auto operator = (const coord_t& rhs) noexcept -> coord_t&
    {
      x = rhs.x;
      y = rhs.y;

      return *this;
    }

    constexpr auto operator == (const coord_t& rhs) const noexcept -> bool
    {
      return x == rhs.x && y == rhs.y;
    }

    constexpr auto operator != (const coord_t& rhs) const noexcept -> bool
    {
      return x != rhs.x || y != rhs.y;
    }

    constexpr auto ne() const noexcept -> coord_t 
    { 
      return { static_cast<int16_t>(x + 2), static_cast<int16_t>(y + 3) }; 
    }

    constexpr auto  e() const noexcept -> coord_t 
    { 
      return { static_cast<int16_t>(x + 4), static_cast<int16_t>(y) }; 
    }

    constexpr auto se() const noexcept -> coord_t 
    { 
      return { static_cast<int16_t>(x + 2), static_cast<int16_t>(y - 3) }; 
    }

    constexpr auto sw() const noexcept -> coord_t 
    { 
      return { static_cast<int16_t>(x - 2), static_cast<int16_t>(y - 3) }; 
    }

    constexpr auto  w() const noexcept -> coord_t 
    { 
      return { static_cast<int16_t>(x - 4), static_cast<int16_t>(y) }; 
    }

    constexpr auto nw() const noexcept -> coord_t 
    { 
      return { static_cast<int16_t>(x - 2), static_cast<int16_t>(y + 3) }; 
    }

    constexpr void neighbors(std::array<coord_t, 6>& arr) const noexcept
    {
      arr[0] = ne();
      arr[1] =  e();
      arr[2] = se();
      arr[3] = sw();
      arr[4] =  w();
      arr[5] = nw();
    }
  };

  enum class color_t : uint8_t
  {
    RED = 1,
    BLACK = 2
  };

  inline void swap(color_t& lhs, color_t& rhs)
  {
    using std::swap;
    swap(lhs, rhs);
  }
}

namespace std
{
  template <>
  class optional<hex::color_t>
  {
  public:
    constexpr optional() noexcept : _color(static_cast<hex::color_t>(0)) {}

    constexpr optional(nullopt_t) noexcept : _color(static_cast<hex::color_t>(0)) {}

    constexpr optional(const optional<hex::color_t>& rhs) noexcept : 
      _color(rhs._color) 
    {}

    constexpr optional(optional<hex::color_t>&& rhs) noexcept :
      _color(move(rhs._color))
    {}

    template <class... Args>
    constexpr optional(in_place_t, Args&&... args) noexcept :
      _color(forward<Args&&>(args)...)
    {}

    constexpr explicit optional(hex::color_t color) noexcept : _color(color) {}

    auto operator = (nullopt_t) noexcept -> optional<hex::color_t>&
    {
      _color = static_cast<hex::color_t>(0);
      return *this;
    }

    auto operator = (const optional<hex::color_t>& rhs) noexcept -> optional<hex::color_t>&
    {
      _color = rhs._color;
      return *this;
    }

    auto operator = (optional<hex::color_t>&& rhs) noexcept -> optional<hex::color_t>&
    {
      _color = move(rhs._color);
      return *this;
    }

    auto operator = (const hex::color_t& value) noexcept -> optional<hex::color_t>&
    {
      _color = value;
      return *this;
    }

    auto operator = (hex::color_t&& value) noexcept -> optional<hex::color_t>&
    {
      _color = move(value);
      return *this;
    }

    constexpr auto operator -> () const noexcept -> const hex::color_t*
    {
      return &_color;
    }

    constexpr auto operator -> () noexcept -> hex::color_t*
    {
      return &_color;
    }

    constexpr auto operator * () const & noexcept -> const hex::color_t&
    {
      return _color;
    }

    constexpr auto operator * () & noexcept -> hex::color_t&
    {
      return _color;
    }

    constexpr auto operator * () const && noexcept -> const hex::color_t&&
    {
      return move(_color);
    }

    constexpr auto operator * () && noexcept -> hex::color_t&&
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

    constexpr auto value() const & -> const hex::color_t&
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

    constexpr auto value() & -> hex::color_t&
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

    constexpr auto value() const && -> const hex::color_t&&
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

    constexpr auto value() && -> hex::color_t&&
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

    constexpr auto value_or(hex::color_t&& default_value) const & -> hex::color_t
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

    constexpr auto value_or(hex::color_t&& default_value) && -> hex::color_t
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

    void swap(optional<hex::color_t>& rhs) noexcept
    {
      using hex::swap;

      swap(_color, rhs._color);
    }

    void reset() noexcept
    {
      _color = static_cast<hex::color_t>(0);
    }

    template <class... Args>
    auto emplace(Args&&... args) -> hex::color_t&
    {
      return (_color = hex::color_t(forward<Args&&>(args)...));
    }

  private:
    hex::color_t _color;
  };
}
