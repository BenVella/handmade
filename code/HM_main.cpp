#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_log.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <SDL_video.h>

#include "HM_sdl_main.h"
#include "HM_common.h"
#include "SDL_keycode.h"

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

void HandleKeys(SDL_Event *Event) {
  bool AltKeyWasDown = (Event->key.keysym.mod & KMOD_ALT);
  SDL_Keycode KeyCode = Event->key.keysym.sym;
  SDL_LogDebug(0, "Key pressed %d: %d", 
      Event->type, KeyCode);

  bool KeyWasDown = Event->key.state == SDL_RELEASED || Event->key.repeat != 0;
  if (KeyWasDown) {

    switch(KeyCode) {
      case SDLK_F4: {
        StopRunning();
      } break;
      case SDLK_w: {
        SDL_LogDebug(0, "Forward!");
      } break;
    }
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
    case SDL_CONTROLLERDEVICEADDED:
    case SDL_CONTROLLERDEVICEREMOVED:
    case SDL_CONTROLLERDEVICEREMAPPED: {
      HM_SdlCtrlrsOpenAll();
    } break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
    {
      HandleKeys(Event);
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

  if (!HM_SDLSetup()) {
    SDL_Log("Exiting due to failed Window or Renderer startup");
  }

  SDL_Log("SDL Window started auccessfully");

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
