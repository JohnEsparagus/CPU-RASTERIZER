#include <SDL2/SDL.h>
#include <iostream>

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

  int mouse_x = 0;
  int mouse_y = 0;
  SDL_UpdateWindowSurface(window);
  bool running = true;
  while (running)
  {
    for (SDL_Event event; SDL_PollEvent(&event);) switch (event.type)
      {
      case SDL_WINDOWEVENT:
        switch (event.window.event)
        {
        case SDL_WINDOWEVENT_RESIZED:
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
  }
}
