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

#define Pi32 3.14159265358979f

typedef float real32;
typedef double real64;

typedef struct Color {
  Uint8 r, g, b;
} Color;

// Predefined color list
const Color WHITE = {255, 255, 255};
const Color CYAN = {0, 255, 255};
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

struct hm_audio_ring_buffer {
  uint8_t *Data;
  uint32_t Size;        // total buffer size in bytes
  uint32_t WriteCursor; // write position (game side)
  uint32_t PlayCursor;  // read position (audio callback side)
};

extern struct hm_audio_ring_buffer GlobalAudioBuffer;

inline void LogSdlError(const char *message) {
  SDL_Log("%s: %s\n", message, SDL_GetError());
}

inline void StartRunning() { hm_app.IsRunning = true; }

inline void StopRunning() { hm_app.IsRunning = false; }

// Sdl setup
bool hm_sdl_setup();
bool hm_sdl_setup_video();
void hm_sdl_setup_audio();
void hm_sdl_ctrl_open_all();
void hm_sdl_setup_texture();

// Sdl testing
void hm_sdl_render_gradient_offset(int offX, int offY);

void hm_sdl_audio_test_square();
void hm_sdl_audio_test_sine();
