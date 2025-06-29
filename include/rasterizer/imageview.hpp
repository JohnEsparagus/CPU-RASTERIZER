#pragma once
#include <cpu_rasterizer/types.hpp>
#include <cstdint>

namespace rasterizer
{
  struct image_view 
  {
    color4ub* pixels = nullptr;
    std::uint32_t width = 0;
    std::uint32_t height = 0;
  }
}
