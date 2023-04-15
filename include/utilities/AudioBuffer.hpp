#pragma once

#include <reaper_plugin.h>

class AudioSource;

class AudioBuffer 
{
  public:

    AudioBuffer() = delete;
    explicit AudioBuffer(const AudioSource& source, int bufferLength, int startSample = 0);
    ~AudioBuffer();

    void RefillSamples();
    
    [[nodiscard]] int SamplesOut() const { return buffer.samples_out; }
    [[nodiscard]] int ChannelCount() const { return buffer.nch; }
    [[nodiscard]] ReaSample GetSampleAt(const int index) const { return buffer.samples[index]; }
    [[nodiscard]] const AudioSource* GetBufferAudioSource() const { return bufferAudioSource; }

  private:

    const AudioSource* bufferAudioSource{};
    PCM_source_transfer_t buffer{};
    int bufferFrame{0};

};