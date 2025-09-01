#include "HM_sdl_main.h"

#include "HM_common.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"

#include <cstdlib>

struct HM_Sdl hm_sdl;

void HM_RenderTexture() {
  int pitch = hm_sdl.bitmapWidth * hm_sdl.bytesPerPixel;
  if (SDL_UpdateTexture(
        hm_sdl.bitmapTexture, 0, hm_sdl.bitmapMemory, pitch)
  ) {
    LogSdlError("SDL Update Texture failed");
  }

  SDL_RenderCopy(hm_sdl.renderer, hm_sdl.bitmapTexture, 0, 0);
  SDL_RenderPresent(hm_sdl.renderer);
}

void HM_SDLSetupTexture() {
  int w, h;
  SDL_GetWindowSize(hm_sdl.window, &w, &h);

  if (hm_sdl.bitmapTexture) {
    SDL_DestroyTexture(hm_sdl.bitmapTexture);
  }

  hm_sdl.bitmapTexture = SDL_CreateTexture(
      hm_sdl.renderer, 
      SDL_PIXELFORMAT_ARGB8888,
      SDL_TEXTUREACCESS_STREAMING,
      w, h);
  
  // Make texture width accessible globally
  hm_sdl.bitmapWidth = w;
  hm_sdl.bitmapHeight = h;

  if (hm_sdl.bitmapMemory) {
    free(hm_sdl.bitmapMemory);
  }
  hm_sdl.bitmapMemory = malloc(w * h * hm_sdl.bytesPerPixel);

  HM_RenderTexture();
}

void HM_RenderOffsetGradient (int offX, int offY) {
  int pitch = hm_sdl.bitmapWidth * hm_sdl.bytesPerPixel;
  Uint8* row = (Uint8*) hm_sdl.bitmapMemory;

  for (int y = 0; y < hm_sdl.bitmapHeight; ++y) {
    Uint8* pixel = (Uint8*) row;
    
    for (int x = 0; x < hm_sdl.bitmapWidth; ++x) {
      *pixel = (Uint8)(x + offX);
      ++pixel;

      *pixel = (Uint8)(y + offY);
      ++pixel;

      *pixel = 0;
      ++pixel;

      *pixel = 0;
      ++pixel;
    }
    row += pitch;
  }
  HM_RenderTexture();
}
