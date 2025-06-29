#include <SDL2/SDL.h>
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <chrono>
int main() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "Failed to initialize the SDL2 library\n";
    return -1;
  }

  int width = 680;
  int height = 480;

  SDL_Window *window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

  if (!window) {
    std::cout << "Failed to create the window\n";
    return -1;
  } 
   
  SDL_Surface *window_surface = SDL_GetWindowSurface(window);

  if(!window_surface) {
    std::cout << "Failed to get the windows surface\n";
    return -1;
  }

  SDL_Surface * draw_surface = nullptr;

  int mouse_x = 0;
  int mouse_y = 0;
  SDL_UpdateWindowSurface(window);
  bool running = true;

  using clock = std::chrono::high_resolution_clock;
  using duration = std::chrono::duration<double, std::milli>;

  while (running)
  {
    for (SDL_Event event; SDL_PollEvent(&event);) switch (event.type)
      {
      case SDL_WINDOWEVENT:
        switch (event.window.event)
        {
        case SDL_WINDOWEVENT_RESIZED:
	  if (draw_surface){
	    SDL_FreeSurface(draw_surface);
	  }
	  draw_surface = nullptr;
          width = event.window.data1;
          height = event.window.data2;
          break;
        }
        break;
      case SDL_QUIT:
	SDL_Delay(1000);
        running = false;
        break;
      case SDL_MOUSEMOTION:
        mouse_x = event.motion.x;
        mouse_y = event.motion.y;
        break;
      }
    if (!running)
        break;
    if (!draw_surface) {
      draw_surface = SDL_CreateRGBSurfaceWithFormat(0, width, height,
        32, SDL_PIXELFORMAT_RGBA32);
      SDL_SetSurfaceBlendMode(draw_surface, SDL_BLENDMODE_NONE);    
    }
    auto now = clock::now();
    duration delta = now - last_frame_start;
    last_frame_start = now;
    double frame_time = delta.count();
    double fps = 1000/frame_time;

    std::cout << "FPS: "<< fps << std::endl;
    
    uint32_t color = 0xffffdfd5;
    std::fill_n((uint32_t *)draw_surface->pixels, width * height, color);
    image_view color_buffer 
    {
      .pixels = (color4ub *)draw_surface->pixels,
      .width = (std::uint32_t)width,
      .length = (std::uint32_t)length
    };

    clear(color_buffer, {0.6f, 0.7f, 1.f, 1.f});
    SDL_Rect rect{.x = 0, .y = 0, .w = width, .h = height};
    SDL_BlitSurface(draw_surface, &rect, SDL_GetWindowSurface(window), &rect);
    SDL_UpdateWindowSurface(window);
  }
}
