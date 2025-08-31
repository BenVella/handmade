#pragma once

#include "SDL_log.h"
#include "SDL_render.h"
#include "SDL_video.h"

struct HM_App {
  bool IsRunning;
  SDL_Window* Window;
  SDL_Renderer* Renderer;
};

// Gobal static app state
static struct HM_App hm_app;

inline void LogSdlError (const char* message) {
    SDL_Log("%s: %s\n", message, SDL_GetError());
}

inline void StartRunning () {
  hm_app.IsRunning = true;
}

inline void StopRunning () {
  hm_app.IsRunning = false;
}
