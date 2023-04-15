#pragma once

#include <reaper_plugin.h>
#include <vector>

class AudioBuffer;

class AudioSource
{
public:

    struct RMS
    {
        std::vector<double> windowRMSValues{};
        std::vector<double> windowSumSquares{};
        int numWindows{1};
        int currentWindow{1};
        int samplesPerWindow{0};
        int numProcessedSamples{0};
        double rmsDb{0};
        bool isComplete{false};
    };

    AudioSource() = delete;

    explicit AudioSource(PCM_source* source);

    void GetSamples(PCM_source_transfer_t& buffer) const { audioSourcePtr->GetSamples(&buffer); }
    [[nodiscard]] bool IsValid() const {  return audioSourcePtr; }
    [[nodiscard]] int GetSampleRate() const { return int(audioSourcePtr->GetSampleRate()); }
    [[nodiscard]] int GetNumChannels() const { return audioSourcePtr->GetNumChannels(); }
    [[nodiscard]] double GetLength() const { return audioSourcePtr->GetLength(); }
    [[nodiscard]] int GetTotalSamples() const { return int(GetSampleRate() * audioSourcePtr->GetLength()); }
    [[nodiscard]] int GetWindowSizeSamples(const int windowSizeMs) const { return std::max(1, GetTotalSamples() * windowSizeMs / 1000); }

    void GetSourceRMS(const int bufferSize = 512, const int windowSizeMs = 300);
    void AnalyseSamples(AudioBuffer& buffer, RMS& rms);

private:

    PCM_source* audioSourcePtr{}; 

    [[nodiscard]] double CalculateRMS(const std::vector<double> windowValues);
    [[nodiscard]] double CalculateRMS(const double windowSumSquares, const int samplesPerWindow);
    [[nodiscard]] double AmpToDecibel(const double rms, const double ref = 1.0);

};