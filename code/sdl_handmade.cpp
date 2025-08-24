#include "SDL_error.h"
#include "SDL_events.h"
#include "SDL_video.h"
#include <SDL.h>

bool HandleWindowEvent(SDL_WindowEvent *winEvent) {
  switch (winEvent->event) {
    case SDL_WINDOWEVENT_RESIZED: {
      printf("SDL_WINDOWEVENT_RESIZED (%d, %d)\n", winEvent->data1, winEvent->data2);
    } break;
    case SDL_WINDOWEVENT_EXPOSED:
    {
      SDL_Window *Window = SDL_GetWindowFromID(winEvent->windowID);
      SDL_Renderer *Renderer = SDL_GetRenderer(Window);
      static bool IsWhite = true;
      if (IsWhite)
      {
        SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
      }
      else
      {
        SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
      }
      IsWhite = !IsWhite;
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
      ShouldQuit = HandleWindowEvent(&(Event->window));
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
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Hello", "Hello, World!", 0);
  bool sdlInitialised = SDL_Init(SDL_INIT_VIDEO) == 0;
  if (!sdlInitialised) {

    const char* error = SDL_GetError();
    printf("Failed SDL Init %s\n", error);
    return Quit();
  }
  SDL_Window *Window =
      SDL_CreateWindow("Handmade",
          0, 0, 320, 480,
          SDL_WINDOW_RESIZABLE);

  if (!Window) {
    return Quit();
  }

// Create a "Renderer" for our window.
  SDL_Renderer *Renderer = SDL_CreateRenderer(Window, -1, 0);

  if (!Renderer) {
    return Quit();
  }

  for(;;)
  {
    SDL_Event Event;
    SDL_WaitEvent(&Event);
    if (HandleEvent(&Event))
    {
        break;
    }
  }
}
