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

void hm_sdl_setup_audio() {
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

struct sdl_sound_output {
  int SamplesPerSecond = 48000;
  int ToneHz = 256;
  int16_t ToneVolume = 3000;
  uint32_t RunningSampleIndex = 0;
  int WavePeriod;
  int BytesPerSample = sizeof(int16_t) * 2;
  int SecondaryBufferSize;
};

void hm_audio_test_square_wave() {
  static struct sdl_sound_output SoundOutput;
  SoundOutput.WavePeriod = SoundOutput.SamplesPerSecond / SoundOutput.ToneHz;
  int bytesToWrite = 800 * SoundOutput.BytesPerSample;

  // Lock while writing
  SDL_LockAudioDevice(device);

  uint32_t writeIndex = GlobalAudioBuffer.WriteCursor;
  uint8_t *buffer = GlobalAudioBuffer.Data;

  for (int i = 0; i < bytesToWrite; i += SoundOutput.BytesPerSample) {
    int16_t sample =
        ((SoundOutput.RunningSampleIndex++ / SoundOutput.WavePeriod) % 2)
            ? SoundOutput.ToneVolume
            : -SoundOutput.ToneVolume;

    *(int16_t *)(buffer + writeIndex) = sample;
    *(int16_t *)(buffer + writeIndex + 2) = sample;

    writeIndex =
        (writeIndex + SoundOutput.BytesPerSample) % GlobalAudioBuffer.Size;
  }

  GlobalAudioBuffer.WriteCursor = writeIndex;

  SDL_UnlockAudioDevice(device);
}

void sdl_fill_sound_buffer(sdl_sound_output *SoundOutput, int ByteToLock,
                           int BytesToWrite) {
  // Lock while writing
  SDL_LockAudioDevice(device);

  uint32_t writeIndex = GlobalAudioBuffer.WriteCursor;
  uint8_t *buffer = GlobalAudioBuffer.Data;

  for (int i = 0; i < BytesToWrite; i += SoundOutput->BytesPerSample) {
    real32 t = 2.0f * Pi32 * SoundOutput->RunningSampleIndex++ /
               (real32)SoundOutput->WavePeriod;
    real32 SineValue = sinf(t);
    int16_t sample = (int16_t)(SineValue * SoundOutput->ToneVolume);

    *(int16_t *)(buffer + writeIndex) = sample;
    *(int16_t *)(buffer + writeIndex + 2) = sample;

    writeIndex =
        (writeIndex + SoundOutput->BytesPerSample) % GlobalAudioBuffer.Size;
  }

  GlobalAudioBuffer.WriteCursor = writeIndex;

  SDL_UnlockAudioDevice(device);
}

void hm_sdl_audio_test_sine() {
  static struct sdl_sound_output SoundOutput;
  SoundOutput.WavePeriod = SoundOutput.SamplesPerSecond / SoundOutput.ToneHz;
  int bytesToWrite = 800 * SoundOutput.BytesPerSample;

  sdl_fill_sound_buffer(&SoundOutput, 0, bytesToWrite);
}
