#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_log.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <SDL_video.h>

#include "HM_sdl_main.h"
#include "HM_common.h"

void DrawColor(const Color& color) {
  SDL_Renderer* renderer = hm_sdl.renderer;
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}

void HandleSdlWindowEvent(const SDL_WindowEvent& winEvent) {
  SDL_Log("Handling WinEvent: %d\n", winEvent.event);

  switch (winEvent.event) {
    case SDL_WINDOWEVENT_SIZE_CHANGED: {
      SDL_Log("SDL_WINDOWEVENT_SIZE_CHANGED (%d, %d)\n", 
          winEvent.data1, winEvent.data2);
      HM_SDLSetupTexture();
    } break;
    case SDL_WINDOWEVENT_EXPOSED:
    {
      static bool IsWhite = true;
      DrawColor(IsWhite ? WHITE : GRAY);
      IsWhite = !IsWhite;
    } break;
    case SDL_WINDOWEVENT_ENTER:
    {
      HM_SDLSetupTexture();
    } break;
    case SDL_WINDOWEVENT_LEAVE:
    {
      DrawColor(CYAN);
    } break;
    default:
    {
      DrawColor(ORANGE);
    } break;
  }
}

void HandleEvent(SDL_Event *Event) {

  switch (Event->type) {
    case SDL_QUIT: {
      printf("SDL_QUIT\n");
      StopRunning();
    } break;
    case SDL_WINDOWEVENT: {
      HandleSdlWindowEvent(Event->window);
    } break;
  }
}

void PollSdlEvents() {
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    HandleEvent(&event);
  }
}

int Quit() {
  SDL_Quit();
  return 1;
}

int main(int argc, char *argv[]) { 
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
  SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1");
  SDL_Log("Starting application with verbose logging...");

  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, 
      "Hello", "Hello, World!", 0);

  bool sdlInitialised = SDL_Init(SDL_INIT_VIDEO) == 0;
  if (!sdlInitialised) {
    LogSdlError("Failed SDL Init");
    return Quit();
  }

  Uint64 window_flags = SDL_WINDOW_OPENGL | 
                        SDL_WINDOW_RESIZABLE;
  hm_sdl.window = SDL_CreateWindow("Handmade App",
                  SDL_WINDOWPOS_CENTERED,
                  SDL_WINDOWPOS_CENTERED,
                  800, 800,
                  window_flags);

  if (!hm_sdl.window) {
    LogSdlError("Post Window SDL Error");
    return Quit();
  }

  hm_sdl.renderer = SDL_CreateRenderer(hm_sdl.window, 
                      -1, SDL_RENDERER_SOFTWARE);

  if (!hm_sdl.renderer) {
    SDL_Log("Post Renderer SDL Error: %s\n", SDL_GetError());
    return Quit();
  }

  StartRunning();

  HM_SDLSetupTexture();
  int offX, offY = 0;
  while(hm_app.IsRunning)
  {
    PollSdlEvents();
    offX = (offX + 1) % 255;
    offY = offX;
    HM_RenderOffsetGradient(offX, offY);
  }
  Quit();
}
