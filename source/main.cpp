#include <SDL2/SDL.h>
#include <cmath>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <rasterizer/cube.hpp>
#include <rasterizer/renderer.hpp>
#include <rasterizer/viewport.hpp>
#include <rasterizer/matrix.hpp>
int main()
{
  SDL_Init(SDL_INIT_VIDEO);

  int width = 800;
  int height = 600;

  SDL_Window * window = SDL_CreateWindow("Tiny rasterizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

  SDL_Surface * draw_surface = nullptr;

  int mouse_x = 0;
  int mouse_y = 0;

  using clock = std::chrono::high_resolution_clock;
  static auto start_time = clock::now();

  auto last_frame_start = clock::now();
  bool running = true;
  while (running)
  {
    for (SDL_Event event; SDL_PollEvent(&event);) switch (event.type)
    {
    case SDL_WINDOWEVENT:
      switch (event.window.event)
      {
      case SDL_WINDOWEVENT_RESIZED:
        if (draw_surface)
          SDL_FreeSurface(draw_surface);
        draw_surface = nullptr;
        width = event.window.data1;
        height = event.window.data2;
        break;
      }
      break;
    case SDL_QUIT:
      running = false;
      break;
    case SDL_MOUSEMOTION:
      mouse_x = event.motion.x;
      mouse_y = event.motion.y;
      break;
    }

    if (!running)
      break;

    if (!draw_surface)
    {
      draw_surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
      SDL_SetSurfaceBlendMode(draw_surface, SDL_BLENDMODE_NONE);
    }

    auto now = clock::now();
    float dt = std::chrono::duration_cast<std::chrono::duration<float>>(now - last_frame_start).count();
    last_frame_start = now;
    
    std::chrono::duration<float> elapsed = now - start_time;  
    std::cout << 1/dt << std::endl;

    using namespace rasterizer;

    image_view color_buffer
    {
      .pixels = (color4ub *)draw_surface->pixels,
      .width = (std::uint32_t)width,
      .height = (std::uint32_t)height,
    };

    viewport  viewport 
    {
        .xmin = 0,
    	.xmax = (std::int32_t)color_buffer.width,
    	.ymin = 0,
    	.ymax = (std::int32_t)color_buffer.height,
    };

    clear(color_buffer, {0.8f, 0.9f, 1.f, 1.f});

    float cube_angle = elapsed.count();

		matrix4x4f model = matrix4x4f::scale(0.2f) * matrix4x4f::rotateZX(cube_angle) * matrix4x4f::rotateXY(cube_angle * 1.61f);

		matrix4x4f view = matrix4x4f::translate({0.f, 0.f, -5.f});

		matrix4x4f projection = matrix4x4f::perspective(0.01f, 10.f, M_PI / 3.f, width * 1.f / height);
    
    matrix4x4f transform = projection * view * model;
    draw(color_buffer,
        draw_command {
            .mesh = cube,
	    .cull_mode = cull_mode::cw,
	    .transform = transform,
        },
	viewport
    );

    SDL_Rect rect{.x = 0, .y = 0, .w = width, .h = height};
    SDL_BlitSurface(draw_surface, &rect, SDL_GetWindowSurface(window), &rect);

    SDL_UpdateWindowSurface(window);
  }
}
