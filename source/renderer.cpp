#include <rasterizer/renderer.hpp>
#include <rasterizer/framebuffer.hpp>
#include <rasterizer/viewport.hpp>
#include <algorithm>
#include <cmath>
#include <cstdio>

namespace rasterizer
{
  namespace
  {
    struct vertex {
      vector4f position;
      vector4f color;
    };

    vertex clip_intersect_edge(vertex const & v0, vertex const & v1, float val0, float val1)
    {
      float t = val0/(val0-val1);
      // interporlation

      vertex a;
      a.position = (1.f - t) * v0.position + t * v1.position;
      a.color = (1.f - t) * v0.color + t * v1.color;

      return a;
    }

    bool depth_test_passed(depth_test_mode mode, std::uint32_t value, std::uint32_t reference)
    {
      switch (mode)
      {
	case depth_test_mode::always: return true;
	case depth_test_mode::never: return false;
	case depth_test_mode::less: return value < reference;
	case depth_test_mode::less_equal: return value <= reference;
	case depth_test_mode::greater: return value > reference;
	case depth_test_mode::greater_equal: return value >= reference;
	case depth_test_mode::equal: return value == reference;
	case depth_test_mode::not_equal: return value != reference;
      }
      return true;
    }

    vertex * clip_triangle( vertex *triangle, vector4f equation, vertex * result)
    {
      float values[3] = 
      {
        dot(triangle[0].position, equation),
        dot(triangle[1].position, equation),
        dot(triangle[2].position, equation),
      };

      std::uint8_t mask = (values[0] < 0.f ? 1 : 0) | (values[1] < 0.f ? 2 : 0) | (values[2] < 0.f ? 4 : 0);

      switch(mask)
      {
        case 0b000:
	  *result++ = triangle[0];
	  *result++ = triangle[1];
	  *result++ = triangle[2];
	  break;
	case 0b001:
          {
            auto v01 = clip_intersect_edge(triangle[0], triangle[1], values[0], values[1]);
            auto v02 = clip_intersect_edge(triangle[0], triangle[2], values[0], values[2]);
            *result++ = v01;
            *result++ = triangle[1];
            *result++ = triangle[2];
            *result++ = v01;
            *result++ = triangle[2];
            *result++ = v02;
          }
          break;
        case 0b010:
          {
            auto v10 = clip_intersect_edge(triangle[1], triangle[0], values[1], values[0]);
            auto v12 = clip_intersect_edge(triangle[1], triangle[2], values[1], values[2]);
            *result++ = triangle[0];
            *result++ = v10;
            *result++ = triangle[2];
            *result++ = triangle[2];
            *result++ = v10;
            *result++ = v12;
          }
          break;
        case 0b011:
          *result++ = clip_intersect_edge(triangle[0], triangle[2], values[0], values[2]);
          *result++ = clip_intersect_edge(triangle[1], triangle[2], values[1], values[2]);
          *result++ = triangle[2];
          break;
        case 0b100:
          {
            auto v20 = clip_intersect_edge(triangle[2], triangle[0], values[2], values[0]);
            auto v21 = clip_intersect_edge(triangle[2], triangle[1], values[2], values[1]);
            *result++ = triangle[0];
            *result++ = triangle[1];
            *result++ = v20;
            *result++ = v20;
            *result++ = triangle[1];
            *result++ = v21;
          }
          break;
        case 0b101:
          *result++ = clip_intersect_edge(triangle[0], triangle[1], values[0], values[1]);
          *result++ = triangle[1];
          *result++ = clip_intersect_edge(triangle[2], triangle[1], values[2], values[1]);
          break;
        case 0b110:
          *result++ = triangle[0];
          *result++ = clip_intersect_edge(triangle[1], triangle[0], values[1], values[0]);
          *result++ = clip_intersect_edge(triangle[2], triangle[0], values[2], values[0]);
          break;
        case 0b111:
          break;
      }

      return result;

    }

    vertex * clip_triangle(vertex *begin, vertex* end) {
      static vector4f const equations[2] = 
      {
        {0.f, 0.f, 1.f, 1.f},
	{0.f, 0.f, -1.f, 1.f}
      };

      vertex result[12];
      
      for (auto equation : equations) {
        auto result_end = result;
	for (vertex * triangle = begin; triangle != end; triangle += 3) 
	{
		// no recurison
	  result_end = clip_triangle(triangle, equation, result_end);
	}
	end = std::copy(result, result_end,begin);
      }
      return end;
    }

  }

  void clear(image_view<color4ub> const& color_buffer, vector4f const& color)
  {
    auto ptr = color_buffer.pixels;
    auto size = color_buffer.width * color_buffer.height;
    std::fill(ptr, ptr + size, to_color4ub(color));
  }
  void clear(image_view<std::uint32_t> const& depth_buffer, std::uint32_t value)
  {
    auto ptr = depth_buffer.pixels;
    auto size = depth_buffer.width * depth_buffer.height;
    std::fill(ptr, ptr + size, value);
  }
  void draw(framebuffer const& framebuffer, draw_command const& command, viewport const& viewport)
  {
    for (std::uint32_t vertex_index = 0; vertex_index + 2 < command.mesh.count; vertex_index += 3)
    {

      std::uint32_t i0 = vertex_index + 0;
      std::uint32_t i1 = vertex_index + 1;
      std::uint32_t i2 = vertex_index + 2;
// i refers to indices if there are indices 
      if (command.mesh.indices)
      {
        i0 = command.mesh.indices[i0];	
        i1 = command.mesh.indices[i1];	
        i2 = command.mesh.indices[i2];	
      }

      vertex clipped_vertices[12];

      clipped_vertices[0].position =  command.transform * as_point(command.mesh.positions[i0]);
      clipped_vertices[1].position = command.transform * as_point(command.mesh.positions[i1]);
      clipped_vertices[2].position = command.transform * as_point(command.mesh.positions[i2]);

      clipped_vertices[0].color = command.mesh.color[i0];
      clipped_vertices[1].color = command.mesh.color[i1];
      clipped_vertices[2].color = command.mesh.color[i2];

      auto clipped_vertices_end = clip_triangle(clipped_vertices, clipped_vertices + 3);

      for (auto triangle_begin = clipped_vertices; triangle_begin!= clipped_vertices_end; triangle_begin +=3)
      {

	auto v0 = triangle_begin[0];
	auto v1 = triangle_begin[1];
	auto v2 = triangle_begin[2];

	vector3f p0{v0.position.x, v0.position.y, v0.position.z};
	vector3f p1{v1.position.x, v1.position.y, v1.position.z};
	vector3f p2{v2.position.x, v2.position.y, v2.position.z};


	vector3f edge1 = p1-p0;
	vector3f edge2 = p2-p0;

	vector3f faceNormal = cross_product(edge1 , edge2);
	faceNormal = normalize(faceNormal);

        v0.position = perspective_divide(v0.position);
        v1.position = perspective_divide(v1.position);
        v2.position = perspective_divide(v2.position);
  
        v0.position = to_pixel(viewport, v0.position);
        v1.position = to_pixel(viewport, v1.position);
        v2.position = to_pixel(viewport, v2.position);
  
        float det012 = det2D(v1.position - v0.position, v2.position - v0.position);
        bool const ccw = det012 < 0.f;
  
        switch (command.cull_mode)
        {
        case cull_mode::none:
          if (ccw)
          {
            std::swap(v1, v2);
            det012 = -det012;
          }
          break;
        case cull_mode::cw:
          if (!ccw)
            continue;
          std::swap(v1, v2);
          det012 = -det012;
          break;
        case cull_mode::ccw:
          if (ccw)
            continue;
          break;
        }
  
        std::int32_t xmin = std::max<std::int32_t>(viewport.xmin,0);
        std::int32_t xmax = std::min<std::int32_t>(viewport.xmax, framebuffer.color.width) - 1;
        std::int32_t ymin = std::max<std::int32_t>(viewport.ymin, 0);
        std::int32_t ymax = std::min<std::int32_t>(viewport.ymax, framebuffer.color.height) - 1;
        //-1 to avoid bounds
  
        xmin = std::max<float>(0, std::min({std::floor(v0.position.x), std::floor(v1.position.x), std::floor(v2.position.x)}));
        xmax = std::min<float>(framebuffer.color.width - 1, std::max({std::floor(v0.position.x), std::floor(v1.position.x), std::floor(v2.position.x)}));
        ymin = std::max<float>(0, std::min({std::floor(v0.position.y), std::floor(v1.position.y), std::floor(v2.position.y)}));
        ymax = std::min<float>(framebuffer.color.height - 1, std::max({std::floor(v0.position.y), std::floor(v1.position.y), std::floor(v2.position.y)}));
  
        for (std::int32_t y = ymin; y <= ymax; ++y)
        {
          for (std::int32_t x = xmin; x <= xmax; ++x)
          {
            vector4f p{x + 0.5f, y + 0.5f, 0.f, 0.f};
  
            float det01p = det2D(v1.position - v0.position, p - v0.position);
            float det12p = det2D(v2.position - v1.position, p - v1.position);
            float det20p = det2D(v0.position - v2.position, p - v2.position);

            if (det01p >= 0.f && det12p >= 0.f && det20p >= 0.f)
  	    { 
  	      float delta1 = det12p / det012 * v0.position.w; //start from here
  	      float delta2 = det20p / det012 * v1.position.w;
  	      float delta3 = det01p / det012 * v2.position.w;

	      float delta_sum = delta1 + delta2 + delta3;

	      delta1 /= delta_sum;
	      delta2 /= delta_sum;
	      delta3 /= delta_sum;

	      if (framebuffer.depth)
	      {
		float z = delta1 * v0.position.z + delta2 * v1.position.z + delta3 * v2.position.z;
		std::uint32_t depth = (0.5f + 0.5f * z) * std::uint32_t(-1);
		auto &old_depth = framebuffer.depth.at(x,y);
		
		if (!depth_test_passed(command.depth.mode, depth, old_depth))
		  continue;
		if (command.depth.write)
		  old_depth = depth;

	      }

	      //interpolated surface ( scratch a pixel shading effect)
	      vector3f P = delta1 * p0 + delta2 * p1 + delta3 * p2;

	      vector3f lightPos{-10.f,-10.f,10.f};
	      vector3f lightDir = normalize(lightPos - P);

	      float ambient = 0.05f;
	      float diffuse = std::max(0.0f, dot(faceNormal,lightDir)); 

	      float  facingRatio = ambient + diffuse;

	      if (framebuffer.color)
	      {
		vector4f interpolatedColor = delta1 * v0.color + delta2 * v1.color + delta3 * v2.color;
	        framebuffer.color.at(x, y) = to_color4ub(interpolatedColor * facingRatio);
	      
	      }
  	    }
  	  }
        }
      }
    }
  }
}
