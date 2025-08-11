#pragma once
#include <rasterizer/image_view.hpp>
#include <rasterizer/color.hpp>

namespace rasterizer
{
  struct framebuffer
  {
    image_view<color4ub> color;
    image_view<uint32_t> depth;

    uint32_t width() const 
    {
      if (color)
      {
        return color.width;
      }
      return depth.width;
    }
    uint32_t height() const // color buffer may be missing in shadow mapping 
    {
      if (color)
      {
        return color.height;
      }
      return depth.height;
    
    }
  };
}
