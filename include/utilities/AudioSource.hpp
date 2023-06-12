#pragma once

#include <cmath>
#include <reaper_plugin.h>
#include <vector>

class AudioBuffer;
class Analysis;

class AudioSource {
  public:
    AudioSource() = delete;
    explicit AudioSource(PCM_source* source);

    AudioSource(const AudioSource& other) = delete;
    AudioSource& operator=(const AudioSource&) = delete;
    AudioSource(AudioSource&&) = delete;
    AudioSource& operator=(AudioSource&&) = delete;

    void GetSamples(PCM_source_transfer_t& buffer) const
    {
        audioSourcePtr->GetSamples(&buffer);
    }
    [[nodiscard]] bool IsValid() const
    {
        return audioSourcePtr;
    }
    [[nodiscard]] int GetSampleRate() const
    {
        return int(audioSourcePtr->GetSampleRate());
    }
    [[nodiscard]] int GetNumChannels() const
    {
        return audioSourcePtr->GetNumChannels();
    }
    [[nodiscard]] double GetLength() const
    {
        return audioSourcePtr->GetLength();
    }
    [[nodiscard]] int GetTotalSamples() const
    {
        return std::floor(GetSampleRate() * audioSourcePtr->GetLength());
    }
    [[nodiscard]] int GetWindowSizeSamples(const int windowSizeMs) const
    {
        return std::floor(GetTotalSamples() * windowSizeMs / 1000);
    }

    [[nodiscard]] double GetSourceRMS(
        const int bufferSize = 512,
        const int windowSizeMs = 300);

  private:
    PCM_source* audioSourcePtr {};
};