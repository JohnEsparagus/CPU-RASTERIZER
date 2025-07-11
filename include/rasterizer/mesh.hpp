#pragma once
#include <rasterizer/vector.hpp>
#include <rasterizer/attribute.hpp>

namespace rasterizer
{
  struct mesh
  {
    attribute<vector3f> positions= {};
    attribute<vector4f> color = {};
    std::uint32_t count = 0;
    std::uint32_t const* indices = nullptr;
    
  };

}
