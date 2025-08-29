#include "SDL_error.h"
#include "SDL_events.h"
#include "SDL_log.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SDL_video.h"
#include "sdl_colors.h"
#include <SDL.h>

SDL_Renderer* DrawColor(const Uint32& windowID, const Color& color) {
  SDL_Window *Window = SDL_GetWindowFromID(windowID);
  SDL_Renderer *Renderer = SDL_GetRenderer(Window);

  SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, 255);
  SDL_RenderClear(Renderer);
  SDL_RenderPresent(Renderer);

  return Renderer;
}

bool HandleWindowEvent(const SDL_WindowEvent& winEvent) {
  SDL_Log("Handling WinEvent: %d\n", winEvent.event);

  SDL_Renderer* Renderer;
  switch (winEvent.event) {
    case SDL_WINDOWEVENT_RESIZED: {
      SDL_Log("SDL_WINDOWEVENT_RESIZED (%d, %d)\n", 
          winEvent.data1, winEvent.data2);
    } break;
    case SDL_WINDOWEVENT_EXPOSED:
    {
      static bool IsWhite = true;
      Renderer = DrawColor(winEvent.windowID, IsWhite ? WHITE : GRAY);
      IsWhite = !IsWhite;
    } break;
    case SDL_WINDOWEVENT_FOCUS_GAINED:
    {
      Renderer = DrawColor(winEvent.windowID, YELLOW);
    } break;
    case SDL_WINDOWEVENT_FOCUS_LOST:
    {
      Renderer = DrawColor(winEvent.windowID, CYAN);
    } break;
    default:
    {
      Renderer = DrawColor(winEvent.windowID, ORANGE);
    } break;
  }

  return false;
}

bool HandleEvent(SDL_Event *Event) {
  bool ShouldQuit = false;

  switch (Event->type) {
    case SDL_QUIT: {
      printf("SDL_QUIT\n");
      ShouldQuit = true;
    } break;
    case SDL_WINDOWEVENT: {
      ShouldQuit = HandleWindowEvent(Event->window);
    } break;
  }

  return (ShouldQuit);
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

  for(;;)
  {
    SDL_Event Event;
    SDL_WaitEvent(&Event);

    bool ShouldQuit = HandleEvent(&Event);
    if (ShouldQuit)
        break;
  }
}
