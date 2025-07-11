#pragma once
#include <algorithm>
#include  <rasterizer/vector.hpp>
#include <cstdint>
#include  <cmath>
namespace rasterizer
{



  struct color4ub
  {
    std::uint8_t r, g, b, a;
  };

  inline color4ub to_color4ub(vector4f const & c)
  {
    color4ub result;
    result.r  = std::max(0.f, std::min(255.f, c.x * 255.f));
    result.g  = std::max(0.f, std::min(255.f, c.y * 255.f));
    result.b  = std::max(0.f, std::min(255.f, c.z * 255.f));
    result.a  = std::max(0.f, std::min(255.f, c.w * 255.f));

    return result;
  }


}
