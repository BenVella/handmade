#include "SDL_audio.h"
#include "../hm_common.h"
#include <cmath>
#include <cstdint>

static SDL_AudioDeviceID device;

void SDLAudioCallback(void *UserData, uint8_t *AudioData, int Length) {

  hm_audio_ring_buffer *RingBuffer = (hm_audio_ring_buffer *)UserData;

  int Region1Size = Length;
  int Region2Size = 0;

  if (RingBuffer->PlayCursor + Length > RingBuffer->Size) {
    Region1Size = RingBuffer->Size - RingBuffer->PlayCursor;
    Region2Size = Length - Region1Size;
  }

  memcpy(AudioData, (uint8_t *)(RingBuffer->Data) + RingBuffer->PlayCursor,
         Region1Size);
  memcpy(&AudioData[Region1Size], RingBuffer->Data, Region2Size);

  RingBuffer->PlayCursor = (RingBuffer->PlayCursor + Length) % RingBuffer->Size;
  RingBuffer->WriteCursor = (RingBuffer->PlayCursor + 2048) % RingBuffer->Size;
}

void HM_SdlAudioSetup() {
  SDL_AudioSpec want = {};
  want.freq = 48000;
  want.format = AUDIO_S16LSB;
  want.channels = 2;
  want.samples = 4096;
  want.callback = SDLAudioCallback; // Enable
  want.userdata = &GlobalAudioBuffer;

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

void hm_audio_test_square_wave() {
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

#define Pi32 3.14159265358979f

typedef float real32;
typedef double real64;

void hm_audio_test_sine_wave() {
  int SamplesPerSecond = 48000;
  int ToneHz = 256;
  int16_t ToneVolume = 3000;
  static uint32_t RunningSampleIndex = 0;

  int WavePeriod = SamplesPerSecond / ToneHz;

  int bytesPerSample = sizeof(int16_t) * 2;
  int bytesToWrite = 800 * bytesPerSample;

  // Lock while writing
  SDL_LockAudioDevice(device);

  uint32_t writeIndex = GlobalAudioBuffer.WriteCursor;
  uint8_t *buffer = GlobalAudioBuffer.Data;

  for (int i = 0; i < bytesToWrite; i += bytesPerSample) {
    real32 t = 2.0f * Pi32 * RunningSampleIndex / (real32)WavePeriod;
    real32 SineValue = sinf(t);
    int16_t sample = (int16_t)(SineValue * ToneVolume);

    *(int16_t *)(buffer + writeIndex) = sample;
    *(int16_t *)(buffer + writeIndex + 2) = sample;

    writeIndex = (writeIndex + bytesPerSample) % GlobalAudioBuffer.Size;
  }

  GlobalAudioBuffer.WriteCursor = writeIndex;

  SDL_UnlockAudioDevice(device);
}
