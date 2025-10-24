#include "HM_sdl_main.h"

#include "HM_common.h"
#include "SDL.h"
#include "SDL_gamecontroller.h"
#include "SDL_haptic.h"
#include "SDL_log.h"
#include "SDL_render.h"
#include "SDL_audio.h"
#include "SDL_stdinc.h"
#include <cstdint>
#include <sys/mman.h>

#include <cstdlib>

// Maps extern
struct HM_Sdl hm_sdl;

// Maps extern
struct HM_Controls Ctrlers[maxControllers];
int activeCtrlers;
SDL_GameController* CtrlerHandles[maxControllers];
SDL_Haptic* RumbleHandles[maxControllers];

bool HM_SDLSetup() {
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
  SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1");
  SDL_Log("Starting application with verbose logging...");

  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, 
      "Hello", "Hello, World!", 0);
  
  atexit(SDL_Quit);
  int sdlInit = SDL_Init(
        SDL_INIT_VIDEO |
        SDL_INIT_GAMECONTROLLER |
        SDL_INIT_HAPTIC |
        SDL_INIT_AUDIO);
  if (sdlInit != 0) {
    LogSdlError("Failed SDL Init");
    SDL_Quit();
    return false;
  }

  bool vidOk = HM_SdlSetupVideo();
  if (!vidOk)
    return false;

  HM_SdlCtrlrsOpenAll();
  HM_SdlAudioSetup();
  return true;
}

bool HM_SdlSetupVideo() {
  Uint64 window_flags = SDL_WINDOW_OPENGL | 
                        SDL_WINDOW_RESIZABLE;
  hm_sdl.window = SDL_CreateWindow("Handmade App",
                  SDL_WINDOWPOS_CENTERED,
                  SDL_WINDOWPOS_CENTERED,
                  800, 800,
                  window_flags);

  if (!hm_sdl.window) {
    LogSdlError("Post Window SDL Error");
    SDL_Quit();
    return false;
  }

  hm_sdl.renderer = SDL_CreateRenderer(hm_sdl.window, 
                      -1, SDL_RENDERER_SOFTWARE);

  if (!hm_sdl.renderer) {
    LogSdlError("Post Renderer SDL Error");
    SDL_Quit();
    return false;
  }
  return true;
}
void HM_RenderTexture() {
  if (SDL_UpdateTexture(
        hm_sdl.bitmapTexture, 0, 
        hm_sdl.bitmapMemory, hm_sdl.pitch)
  ) {
    LogSdlError("SDL Update Texture failed");
  }

  SDL_RenderCopy(hm_sdl.renderer, hm_sdl.bitmapTexture, 0, 0);
  SDL_RenderPresent(hm_sdl.renderer);
}

void HM_SDLSetupTexture() {
  int w, h;
  SDL_GetWindowSize(hm_sdl.window, &w, &h);

  if (hm_sdl.bitmapTexture) {
    SDL_DestroyTexture(hm_sdl.bitmapTexture);
  }

  hm_sdl.bitmapTexture = SDL_CreateTexture(
      hm_sdl.renderer, 
      SDL_PIXELFORMAT_ARGB8888,
      SDL_TEXTUREACCESS_STREAMING,
      w, h);
  
  // Make texture width accessible globally
  hm_sdl.bitmapWidth = w;
  hm_sdl.bitmapHeight = h;
  short bytesPerPixel = 4;
  hm_sdl.pitch = hm_sdl.bitmapWidth * bytesPerPixel;
  hm_sdl.textureSize = hm_sdl.pitch * hm_sdl.bitmapHeight;

  if (hm_sdl.bitmapMemory) {
    munmap(hm_sdl.bitmapMemory, hm_sdl.textureSize);
  }

  hm_sdl.bitmapMemory = mmap(0,
      hm_sdl.textureSize,
    PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_PRIVATE,
    -1, 0);

  // malloc() and free() posix compliant option:
  // if (hm_sdl.bitmapMemory) {
  //   free(hm_sdl.bitmapMemory);
  // }
  // hm_sdl.bitmapMemory = malloc(hm_sdl.textureSize);

  HM_RenderTexture();
}

void HM_RenderOffsetGradient (int offX, int offY) {
  Uint8* row = (Uint8*) hm_sdl.bitmapMemory;

  for (int y = 0; y < hm_sdl.bitmapHeight; ++y) {
    Uint8* pixel = (Uint8*) row;
    
    for (int x = 0; x < hm_sdl.bitmapWidth; ++x) {
      *pixel = (Uint8)(x + offX);
      ++pixel;

      *pixel = (Uint8)(y + offY);
      ++pixel;

      *pixel = 0;
      ++pixel;

      *pixel = 0;
      ++pixel;
    }
    row += hm_sdl.pitch;
  }
  HM_RenderTexture();
}

void HM_SdlLoadControls(SDL_GameController* ctrler) {
  for (int ctrlerIdx = 0; ctrlerIdx < maxControllers; ++ctrlerIdx) {
    SDL_GameController* handle = CtrlerHandles[ctrlerIdx];
    if(handle == 0 || SDL_GameControllerGetAttached(handle)) {
      continue;
    }

    struct HM_Controls ctrler;
    ctrler.Up =
			SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_DPAD_UP);
    ctrler.Down =
			SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
    ctrler.Left =
			SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
    ctrler.Right =
			SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
    ctrler.Start =
			SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_START);
    ctrler.Back =
			SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_BACK);
    ctrler.LeftShoulder =
			SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
    ctrler.RightShoulder =
			SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
    ctrler.AButton =
			SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_A);
    ctrler.BButton =
			SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_B);
    ctrler.XButton =
			SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_X);
    ctrler.YButton =
			SDL_GameControllerGetButton(handle, SDL_CONTROLLER_BUTTON_Y);

    ctrler.StickX = SDL_GameControllerGetAxis(handle, SDL_CONTROLLER_AXIS_LEFTX);
    ctrler.StickY = SDL_GameControllerGetAxis(handle, SDL_CONTROLLER_AXIS_LEFTY);

    Ctrlers[ctrlerIdx] = ctrler;
    const char* ctrlerName = SDL_GameControllerNameForIndex(ctrlerIdx);
    SDL_LogDebug(0, "Started controller index %d named: %s", ctrlerIdx, ctrlerName);
    SDL_Joystick *JoyHandle = SDL_GameControllerGetJoystick(handle);
    RumbleHandles[ctrlerIdx] = SDL_HapticOpenFromJoystick(JoyHandle);

    if (SDL_HapticRumbleInit(RumbleHandles[ctrlerIdx]) != 0) {
        SDL_HapticClose(RumbleHandles[ctrlerIdx]);
        RumbleHandles[ctrlerIdx] = 0;
    }
  }
}

void HM_SdlCtrlrsOpenAll() {
  HM_SdlCtrlersCloseAll();
  SDL_LogDebug(0, "Opening Controllers");
  int joyMax = SDL_NumJoysticks();
  activeCtrlers = 0;
  for(int joyIdx=0; joyIdx < joyMax; ++joyIdx)
  {
      if (!SDL_IsGameController(joyIdx)) {
          continue;
      }
      if (activeCtrlers >= maxControllers) {
          break;
      }
      CtrlerHandles[activeCtrlers] = SDL_GameControllerOpen(joyIdx);
      activeCtrlers++;
  }
  SDL_LogDebug(0, "Opened %d controllers", activeCtrlers);
}

void HM_SdlCtrlerClose(int joyIdx) {
  if (CtrlerHandles[joyIdx]) {
    SDL_GameControllerClose(CtrlerHandles[joyIdx]);
  }
  if (RumbleHandles[joyIdx])
    SDL_HapticClose(RumbleHandles[joyIdx]);
}

void HM_SdlCtrlersCloseAll() {
  for (int joyIdx = activeCtrlers - 1; joyIdx >= 0; --joyIdx) {
    HM_SdlCtrlerClose(joyIdx);
    activeCtrlers--;
  }
}

void
SDLAudioCallback(void *UserData, Uint8 *AudioData, int Length)
{
    // Clear our audio buffer to silence.
    memset(AudioData, 0, Length);
}

void HM_SdlAudioSetup() {
  if (!SoundIsPlaying)
  {
    SDL_PauseAudio(0);
    SoundIsPlaying = true;
  }

  SDL_AudioSpec AudioSettings = {0};
  int SamplesPerSecond = 48000;

  // Todo: Likely wrong, check buffe reqs
  int BytesPerSample = sizeof(int16_t) * 2;
  int BytesToWrite = 800 * BytesPerSample;
  Uint16 BufferSize = BytesToWrite;

  AudioSettings.freq = SamplesPerSecond;
  AudioSettings.format = AUDIO_S16LSB;
  AudioSettings.channels = 2;
  AudioSettings.samples = BufferSize / 2;
  AudioSettings.callback = &SDLAudioCallback;

  SDL_OpenAudio(&AudioSettings, 0);

  if (AudioSettings.format != AUDIO_S16LSB) {
    SDL_LogError(0, "Incorrect Audio Format returned: %d", AudioSettings.format);
    SDL_CloseAudio();
  }
}

void HM_AudioTest() {
  // NOTE: Sound test
  int SamplesPerSecond = 48000;
  int ToneHz = 256;
  int16_t ToneVolume = 3000;
  uint32_t RunningSampleIndex = 0;
  int SquareWavePeriod = SamplesPerSecond / ToneHz;
  int HalfSquareWavePeriod = SquareWavePeriod / 2;
  int BytesPerSample = sizeof(int16_t) * 2;
  // See https://davidgow.net/handmadepenguin/ch8.html
  int BytesToWrite = 800 * BytesPerSample;

  // Prep Memory
  void *SoundBuffer = malloc(BytesToWrite);
  int16_t *SampleOut = (int16_t *)SoundBuffer;
  int SampleCount = BytesToWrite/BytesPerSample;

  // Write square wave
  for(int SampleIndex = 0; SampleIndex < SampleCount; ++SampleIndex)
  {
      int16_t SampleValue = ((RunningSampleIndex++ / HalfSquareWavePeriod) % 2) ? ToneVolume : -ToneVolume;
      *SampleOut++ = SampleValue;
      *SampleOut++ = SampleValue;
  }

  SDL_QueueAudio(1, SoundBuffer, BytesToWrite);
  free(SoundBuffer);
}
