#pragma once

#include <reaper_plugin.h>
#include <vector>
#include <cmath>

class AudioBuffer;
class RMS;

class AudioSource
{
public:

    AudioSource() = delete;

    explicit AudioSource(PCM_source* source);

    void GetSamples(PCM_source_transfer_t& buffer) const { audioSourcePtr->GetSamples(&buffer); }
    [[nodiscard]] bool IsValid() const {  return audioSourcePtr; }
    [[nodiscard]] int GetSampleRate() const { return int(audioSourcePtr->GetSampleRate()); }
    [[nodiscard]] int GetNumChannels() const { return audioSourcePtr->GetNumChannels(); }
    [[nodiscard]] double GetLength() const { return audioSourcePtr->GetLength(); }
    [[nodiscard]] int GetTotalSamples() const { return std::floor(GetSampleRate() * audioSourcePtr->GetLength()); }
    [[nodiscard]] int GetWindowSizeSamples(const int windowSizeMs) const { return std::floor(GetTotalSamples() * windowSizeMs / 1000); }

    void GetSourceRMS(const int bufferSize = 512, const int windowSizeMs = 300);

private:

    PCM_source* audioSourcePtr{}; 

};