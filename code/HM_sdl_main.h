#pragma once

#include "SDL_video.h"
#include <SDL.h>

struct HM_Sdl {
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* bitmapTexture;
  void* bitmapMemory;
  int bitmapWidth;
  int bitmapHeight;
  int pitch;
  int textureSize;
};

extern struct HM_Sdl hm_sdl;

typedef struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
} Color;

// Predefined color list
const Color RED     = {255, 0, 0};
const Color GREEN   = {0, 255, 0};
const Color BLUE    = {0, 0, 255};
const Color WHITE   = {255, 255, 255};
const Color BLACK   = {0, 0, 0};
const Color YELLOW  = {255, 255, 0};
const Color CYAN    = {0, 255, 255};
const Color MAGENTA = {255, 0, 255};
const Color GRAY    = {128, 128, 128};
const Color ORANGE  = {255, 165, 0};

bool HM_SDLSetup();
bool HM_SdlSetupVideo();
void HM_SdlOpenControllers();
void HM_SdlCloseControllers();
void HM_SdlCloseController();
void HM_RenderTexture();
void HM_SDLSetupTexture();
void HM_RenderOffsetGradient (int offX, int offY);

