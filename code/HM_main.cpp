#include "SDL_error.h"
#include "SDL_events.h"
#include "SDL_log.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SDL_video.h"
#include "HM_sdl_main.h"
#include <SDL.h>

struct hm_app {
  bool IsRunning;
  SDL_Window* Window;
  SDL_Renderer* Renderer;
} hm_app;

// Gobal static app state
static struct hm_app HM_App;

void StopRunning () {
  HM_App.IsRunning = false;
}

SDL_Renderer* DrawColor(SDL_Renderer* Renderer, const Color& color) {
  SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, 255);
  SDL_RenderClear(Renderer);
  SDL_RenderPresent(Renderer);

  return Renderer;
}

void HandleSdlWindowEvent(const SDL_WindowEvent& winEvent) {
  SDL_Log("Handling WinEvent: %d\n", winEvent.event);

  SDL_Window* window = SDL_GetWindowFromID(winEvent.event);
  SDL_Renderer* renderer = SDL_GetRenderer(window);
  switch (winEvent.event) {
    case SDL_WINDOWEVENT_SIZE_CHANGED: {
      SDL_Log("SDL_WINDOWEVENT_SIZE_CHANGED (%d, %d)\n", 
          winEvent.data1, winEvent.data2);
      HM_SDLResizeTexture(window);
    } break;
    case SDL_WINDOWEVENT_EXPOSED:
    {
      static bool IsWhite = true;
      DrawColor(renderer, IsWhite ? WHITE : GRAY);
      IsWhite = !IsWhite;
    } break;
    case SDL_WINDOWEVENT_FOCUS_GAINED:
    {
      DrawColor(renderer, YELLOW);
    } break;
    case SDL_WINDOWEVENT_FOCUS_LOST:
    {
      DrawColor(renderer, CYAN);
    } break;
    default:
    {
      DrawColor(renderer, ORANGE);
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

int Quit() {
  SDL_Quit();
  return 1;
}

int main(int argc, char *argv[]) { 
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
  SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1");
  SDL_Log("Starting application with verbose logging...");

  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Hello", "Hello, World!", 0);

  bool sdlInitialised = SDL_Init(SDL_INIT_VIDEO) == 0;
  if (!sdlInitialised) {
    SDL_Log("Failed SDL Init %s\n", SDL_GetError());
    return Quit();
  }

  Uint64 window_flags = SDL_WINDOW_OPENGL | 
                        SDL_WINDOW_RESIZABLE;
  SDL_Window* Window = SDL_CreateWindow("Handmade App",
                  SDL_WINDOWPOS_CENTERED,
                  SDL_WINDOWPOS_CENTERED,
                  800, 800,
                  window_flags);

  if (!Window) {
    SDL_Log("Post Window SDL Error: %s\n", SDL_GetError());
    return Quit();
  }

  SDL_Renderer *Renderer = SDL_CreateRenderer(Window, 
                      -1, SDL_RENDERER_SOFTWARE);

  if (!Renderer) {
    SDL_Log("Post Renderer SDL Error: %s\n", SDL_GetError());
    return Quit();
  }

  hm_app.IsRunning = true;
  while(hm_app.IsRunning)
  {
    SDL_Event Event;
    SDL_WaitEvent(&Event);

    HandleEvent(&Event);
  }
  Quit();
}
