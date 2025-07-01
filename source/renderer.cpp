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

  bool point_in_triangle(std::int32_t x, std::int32_t y, vector4f v0, vector4f v1, vector4f v2)
  {
    vector4f p{x + 0.5f, y + 0.5f, 0.f, 0.f};
    float det1 = det2D(v1-v0, p-v0);
    float det2 = det2D(v2-v1, p-v1);
    float det3 = det2D(v0-v2, p-v2);
    if (det1 >=0.f && det2 >=0.f && det3>= 0.f) 
    {
      return true;
    }
    return false;
  }

  void draw(image_view const& color_buffer, draw_command const& command)
  {
    for(std::uint32_t vertex_index=0;vertex_index+2<command.mesh.vertex_count;vertex_index+=3)
    {
      auto v0 = as_point(command.mesh.positions[vertex_index+0]);
      auto v1 = as_point(command.mesh.positions[vertex_index+1]);
      auto v2 = as_point(command.mesh.positions[vertex_index+2]);
    
      for  (std::int32_t y=0; y < color_buffer.height; ++y)
      {
        for (std::int32_t x=0; x < color_buffer.width; ++x)
	{
	  if (point_in_triangle(x,y,v0,v1,v2))
	  {
	    color_buffer.at(x,y) = to_color4ub(command.mesh.color);
	  }
	}
      }
    }
  }

}
