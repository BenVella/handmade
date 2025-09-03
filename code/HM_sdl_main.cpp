#include "HM_sdl_main.h"

#include "HM_common.h"
#include "SDL_gamecontroller.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include <sys/mman.h>

#include <cstdlib>

struct HM_Sdl hm_sdl;
const int maxControllers = 4;
int numControllers;
SDL_GameController* controllerHandles[maxControllers];

bool HM_SDLSetup() {
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
  SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1");
  SDL_Log("Starting application with verbose logging...");

  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, 
      "Hello", "Hello, World!", 0);

  int sdlInit = SDL_Init(
        SDL_INIT_VIDEO |
        SDL_INIT_GAMECONTROLLER);
  if (sdlInit != 0) {
    LogSdlError("Failed SDL Init");
    SDL_Quit();
    return false;
  }

  bool vidOk = HM_SdlSetupVideo();
  if (!vidOk)
    return false;

  HM_SdlOpenControllers();
  return true;
}

bool HM_SdlSetupVideo() {
  Uint64 window_flags = SDL_WINDOW_OPENGL | 
                        SDL_WINDOW_RESIZABLE;
  hm_sdl.window = SDL_CreateWindow("Handmade App",
                  SDL_WINDOWPOS_CENTERED,
                  SDL_WINDOWPOS_CENTERED,
                  800, 800,
                  window_flags);

  if (!hm_sdl.window) {
    LogSdlError("Post Window SDL Error");
    SDL_Quit();
    return false;
  }

  hm_sdl.renderer = SDL_CreateRenderer(hm_sdl.window, 
                      -1, SDL_RENDERER_SOFTWARE);

  if (!hm_sdl.renderer) {
    LogSdlError("Post Renderer SDL Error");
    SDL_Quit();
    return false;
  }
  return true;
}

void HM_SdlOpenControllers() {
  int joyMax = SDL_NumJoysticks();
  numControllers = 0;
  for(int joyIdx=0; joyIdx < joyMax; ++joyIdx)
  {
      if (!SDL_IsGameController(joyIdx))
      {
          continue;
      }
      if (numControllers >= maxControllers)
      {
          break;
      }
      controllerHandles[numControllers] = SDL_GameControllerOpen(joyIdx);
      numControllers++;
  }
}

void HM_SdlCloseController(int joyIdx) {
  if (controllerHandles[joyIdx]) {
    SDL_GameControllerClose(controllerHandles[joyIdx]);
   }
}

void HM_SdlCloseControllers() {
  for (int joyIdx = 0; joyIdx < numControllers; ++joyIdx) {
    HM_SdlCloseController(joyIdx);
  }
}

void HM_RenderTexture() {
  if (SDL_UpdateTexture(
        hm_sdl.bitmapTexture, 0, 
        hm_sdl.bitmapMemory, hm_sdl.pitch)
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
  short bytesPerPixel = 4;
  hm_sdl.pitch = hm_sdl.bitmapWidth * bytesPerPixel;
  hm_sdl.textureSize = hm_sdl.pitch * hm_sdl.bitmapHeight;

  if (hm_sdl.bitmapMemory) {
    munmap(hm_sdl.bitmapMemory, hm_sdl.textureSize);
  }

  hm_sdl.bitmapMemory = mmap(0,
      hm_sdl.textureSize,
    PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_PRIVATE,
    -1, 0);

  // malloc and free posix compliant option:
  // if (hm_sdl.bitmapMemory) {
  //   free(hm_sdl.bitmapMemory);
  // }
  // hm_sdl.bitmapMemory = malloc(hm_sdl.textureSize);

  HM_RenderTexture();
}

void HM_RenderOffsetGradient (int offX, int offY) {
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
    row += hm_sdl.pitch;
  }
  HM_RenderTexture();
}
