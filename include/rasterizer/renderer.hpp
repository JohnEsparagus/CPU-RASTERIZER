#pragma once
#include <rasterizer/vector.hpp>
#include <rasterizer/image_view.hpp>
#include <rasterizer/draw_command.hpp>
#include <rasterizer/viewport.hpp>
#include <rasterizer/framebuffer.hpp>
namespace rasterizer
{
  void clear(image_view<color4ub> const& color_buffer, vector4f const& color);
  void clear(image_view<uint32_t> const& depth_buffer, uint32_t value);
  void draw(framebuffer const& framebuffer, draw_command const& command, viewport  const& viewport);
  bool point_in_triangle(std::int32_t x, std::int32_t y, vector4f v0, vector4f v1, vector4f v2);
  
}
