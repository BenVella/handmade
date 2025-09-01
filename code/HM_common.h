#pragma once

#include "SDL_error.h"
#include "SDL_log.h"

struct HM_App {
  bool IsRunning;
};

// Gobal static app state
extern struct HM_App hm_app;

inline void LogSdlError (const char* message) {
    SDL_Log("%s: %s\n", message, SDL_GetError());
}

inline void StartRunning () {
  hm_app.IsRunning = true;
}

inline void StopRunning () {
  hm_app.IsRunning = false;
}
