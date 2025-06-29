#pragma once
#include <cstdint>
#include <algorithm>

namespace rasterizer
{
  struct color4ub
  {
    std::uint8_t r, g, b, a;
  };

  struct vector4
  {
    float x,y,z,w;
  }

  inline  color4ub to_color4ub(vector4f const & c)
  {
    color4ub result;
    result.r  = max(0.f, min(255.f, c.x * 255.f));
    result.g  = max(0.f, min(255.f, c.y * 255.f));
    result.b  = max(0.f, min(255.f, c.z * 255.f));
    result.a  = max(0.f, min(255.f, c.w * 255.f));

    return result;
  }
}

