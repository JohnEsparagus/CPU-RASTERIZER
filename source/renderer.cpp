#include <rasterizer/renderer.hpp>

#include <algorithm>

namespace rasterizer
{

  void clear(image_view const& color_buffer, vector4f const& color)
  {
    auto ptr = color_buffer.pixels;
    auto size = color_buffer.width * color_buffer.height;
    std::fill(ptr, ptr + size, to_color4ub(color));
  }

}
