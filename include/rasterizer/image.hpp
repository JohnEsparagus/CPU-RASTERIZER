#pragma once
#include <rasterizer/image_view.hpp>

#include <cstdint>
#include <memory>

namespace rasterizer
{
  template <typename Pixel>
  struct image 
  {
    std::unique_ptr<Pixel[]> pixels;
    uint32_t width = 0;
    uint32_t height = 0;

    explicit operator bool() const
    {
      return pixels != nullptr;
    }
    image_view<Pixel> view()
    {	    
      return image_view<Pixel>
        {
	  .pixels = pixels.get(),
	  .width = width,
          .height = height,
	};
    }
    static image allocate(uint32_t width, uint32_t height) {
      return image {
        .pixels = std::make_unique<Pixel[]>(new Pixel[width*height]),
        .width = width,
        .height = height,
      };
    }
  };
}
