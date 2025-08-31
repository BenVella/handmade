#include "HM_sdl_main.h"

void HM_SDLResizeTexture(SDL_Window* window) {
  SDL_Renderer* renderer = SDL_GetRenderer(window);
  int w, h;
  SDL_GetWindowSize(window, &w, &h);
  SDL_Texture *Texture = SDL_CreateTexture(
      renderer, 
      SDL_PIXELFORMAT_ARGB8888,
      SDL_TEXTUREACCESS_STREAMING,
      w, h);
}


