#include "SDL_audio.h"
#include "hm_sdl.h"
#include "../hm_common.h"

struct hm_audio_ring_buffer {
  uint8_t *Data;
  uint32_t Size;        // total buffer size in bytes
  uint32_t WriteCursor; // write position (game side)
  uint32_t PlayCursor;  // read position (audio callback side)
};

static hm_audio_ring_buffer GlobalAudioBuffer;
static SDL_AudioDeviceID device;

void SDLAudioCallback(void *userData, Uint8 *stream, int len) {
  hm_audio_ring_buffer *buffer = &GlobalAudioBuffer;

  uint32_t playIndex = buffer->PlayCursor;
  uint32_t size = buffer->Size;

  for (int i = 0; i < len; ++i) {
    stream[i] = buffer->Data[playIndex];
    playIndex = (playIndex + 1) % size;
  }

  buffer->PlayCursor = playIndex;
}

void HM_SdlAudioSetup() {
  SDL_AudioSpec want = {};
  want.freq = 48000;
  want.format = AUDIO_S16LSB;
  want.channels = 2;
  want.samples = 4096;
  want.callback = SDLAudioCallback; // <-- ENABLE CALLBACK NOW

  device = SDL_OpenAudioDevice(NULL, 0, &want, NULL, 0);
  if (!device) {
    SDL_Log("Audio init failed: %s", SDL_GetError());
    return;
  }

  // Allocate 2 seconds of audio buffer space
  int bytesPerSample = sizeof(int16_t) * 2;
  int bufferSampleCount = want.freq * 2;
  GlobalAudioBuffer.Size = bufferSampleCount * bytesPerSample;
  GlobalAudioBuffer.Data = (uint8_t *)malloc(GlobalAudioBuffer.Size);
  GlobalAudioBuffer.WriteCursor = 0;
  GlobalAudioBuffer.PlayCursor = 0;

  SDL_PauseAudioDevice(device, 0);
}

void HM_AudioTest() {
  int SamplesPerSecond = 48000;
  int ToneHz = 256;
  int16_t ToneVolume = 3000;
  static uint32_t RunningSampleIndex = 0;

  int SquareWavePeriod = SamplesPerSecond / ToneHz;
  int HalfSquareWavePeriod = SquareWavePeriod / 2;

  int bytesPerSample = sizeof(int16_t) * 2;
  int bytesToWrite = 800 * bytesPerSample;

  // Lock while writing
  SDL_LockAudioDevice(device);

  uint32_t writeIndex = GlobalAudioBuffer.WriteCursor;
  uint8_t *buffer = GlobalAudioBuffer.Data;

  for (int i = 0; i < bytesToWrite; i += bytesPerSample) {
    int16_t sample = ((RunningSampleIndex++ / HalfSquareWavePeriod) % 2)
                         ? ToneVolume
                         : -ToneVolume;

    *(int16_t *)(buffer + writeIndex) = sample;
    *(int16_t *)(buffer + writeIndex + 2) = sample;

    writeIndex = (writeIndex + bytesPerSample) % GlobalAudioBuffer.Size;
  }

  GlobalAudioBuffer.WriteCursor = writeIndex;

  SDL_UnlockAudioDevice(device);
}
