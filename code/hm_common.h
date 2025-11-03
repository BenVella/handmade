#pragma once

#include "SDL_log.h"
#include "SDL_render.h"
#include "SDL_video.h"
struct HM_App {
  bool IsRunning;
};

// Gobal static app state
extern struct HM_App hm_app;

struct HM_Sdl {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *bitmapTexture;
  void *bitmapMemory;
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
const Color RED = {255, 0, 0};
const Color GREEN = {0, 255, 0};
const Color BLUE = {0, 0, 255};
const Color WHITE = {255, 255, 255};
const Color BLACK = {0, 0, 0};
const Color YELLOW = {255, 255, 0};
const Color CYAN = {0, 255, 255};
const Color MAGENTA = {255, 0, 255};
const Color GRAY = {128, 128, 128};
const Color ORANGE = {255, 165, 0};

struct HM_Controls {
  bool Up;
  bool Down;
  bool Left;
  bool Right;
  bool Start;
  bool Back;
  bool LeftShoulder;
  bool RightShoulder;
  bool AButton;
  bool BButton;
  bool XButton;
  bool YButton;
  int16_t StickX;
  int16_t StickY;
};

const int maxControllers = 4;
extern struct HM_Controls Ctrlers[maxControllers];

// Maps extern
extern struct HM_Sdl hm_sdl;

// Maps extern
extern struct HM_Controls Ctrlers[maxControllers];

inline void LogSdlError(const char *message) {
  SDL_Log("%s: %s\n", message, SDL_GetError());
}

inline void StartRunning() { hm_app.IsRunning = true; }

inline void StopRunning() { hm_app.IsRunning = false; }

bool HM_SDLSetup();
void HM_SdlCtrlrsOpenAll();
void HM_SDLSetupTexture();
void HM_RenderOffsetGradient(int offX, int offY);
void HM_AudioTest();
