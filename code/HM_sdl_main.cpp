#include "HM_sdl_main.h"

#include "HM_common.h"
#include "SDL_render.h"

#include <cstdlib>

struct HM_Sdl {
  int bytesPerPixel = 4;
  int bitmapWidth;
  void* bitmapMemory;
};

static struct HM_Sdl hm_sdl;

void HM_SDLResizeTexture(SDL_Window* window) {
  SDL_Renderer* renderer = SDL_GetRenderer(window);
  int w, h;
  SDL_GetWindowSize(window, &w, &h);

  SDL_Texture *texture = SDL_CreateTexture(
      renderer, 
      SDL_PIXELFORMAT_ARGB8888,
      SDL_TEXTUREACCESS_STREAMING,
      w, h);
  
  if (texture) {
    SDL_DestroyTexture(texture);
  }

  // Make texture width accessible globally
  hm_sdl.bitmapWidth = w;

  // Get bytes for argb with 8 bits each
  hm_sdl.bitmapMemory = malloc(w * h * hm_sdl.bytesPerPixel);

  if (hm_sdl.bitmapMemory) {
    free(hm_sdl.bitmapMemory);
  }

  int pitch = hm_sdl.bitmapWidth * hm_sdl.bytesPerPixel;
  if (SDL_UpdateTexture(
        texture, 0, hm_sdl.bitmapMemory, pitch)
  ) {
    LogSdlError("SDL Updat Texture failed");
  }

  SDL_RenderCopy(renderer, texture, 0, 0);
  SDL_RenderPresent(renderer);
}

