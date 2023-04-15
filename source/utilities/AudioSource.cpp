#include <utilities/AudioSource.hpp>
#include <utilities/AudioBuffer.hpp>
#include <cmath>

// DEBUG
#include <reaper_plugin_functions.h>
#include <string>
#include <format>
#include <sstream>
// DEBUG

using std::vector;

AudioSource::AudioSource(PCM_source* source)
{
    audioSourcePtr = source;
}

double AudioSource::CalculateRMS(const vector<double> windowValues)
{
    double totalAmp{};
    for (const auto& ampVal : windowValues)
    {
        totalAmp += ampVal * ampVal;
    }
    return std::sqrt(totalAmp / int(windowValues.size()));
}

double AudioSource::CalculateRMS(const double windowSumSquares, const int samplesPerWindow)
{
    return std::sqrt(windowSumSquares / samplesPerWindow);
}

void AudioSource::GetSourceRMS(const int bufferSize, const int windowSizeMs)
{
    AudioBuffer buffer(*this, bufferSize);
    RMS rms;

    rms.samplesPerWindow = GetWindowSizeSamples(windowSizeMs);
    rms.numWindows = int(GetTotalSamples() / rms.samplesPerWindow);

    std::string log10{std::format("samples per windows: {}.\n", rms.samplesPerWindow)};
    ShowConsoleMsg(log10.c_str());
    
    double currentWindowSumSquares{0.0};

    while (buffer.SamplesOut()) // while there are samples to process
    {   
        if (rms.isComplete)
            break;

        for (int currentSample = 0; currentSample < buffer.SamplesOut(); currentSample++)
        {
            ++rms.numProcessedSamples;

            const ReaSample sampleValue = buffer.GetSampleAt(currentSample);
            currentWindowSumSquares += std::pow(sampleValue, 2);

            if (rms.numProcessedSamples == (rms.samplesPerWindow * rms.currentWindow) || rms.numProcessedSamples == GetTotalSamples())
            {
                if (rms.currentWindow != rms.numWindows)
                {
                    const double windowRMS = CalculateRMS(currentWindowSumSquares, rms.samplesPerWindow);
                    rms.windowRMSValues.push_back(windowRMS);
                    rms.windowSumSquares.push_back(currentWindowSumSquares);

                    std::string log{std::format("window: {}. samples: {}. rms: {}.\n", rms.currentWindow, rms.samplesPerWindow, windowRMS)};
                    ShowConsoleMsg(log.c_str());

                    currentWindowSumSquares = 0.0;
                    ++rms.currentWindow;    
                }
                else // final window -> we need to apply a weighted average if the num samples is uneven
                {
                    const int finalWindowSamples = GetTotalSamples() % rms.samplesPerWindow;
                    double nonWeightedSumSquares{0}; 
                    for (const double value : rms.windowSumSquares)
                    {
                        nonWeightedSumSquares += value;
                    }

                    const double weightedSumValues = nonWeightedSumSquares + currentWindowSumSquares * finalWindowSamples / rms.samplesPerWindow;
                    const double weightedWindowRMS = CalculateRMS(weightedSumValues, GetTotalSamples());

                    rms.windowRMSValues.push_back(weightedWindowRMS);
                    rms.windowSumSquares.push_back(currentWindowSumSquares);

                    std::string log{std::format("window: {}. samples: {}. weighted rms: {}.\n", rms.currentWindow, finalWindowSamples, weightedWindowRMS)};
                    ShowConsoleMsg(log.c_str());
                    rms.isComplete = true;
                }
            }
        }
        buffer.RefillSamples();
    }
    rms.rmsDb = AmpToDecibel(CalculateRMS(rms.windowRMSValues));
    std::string totallog{std::format("\ntotal rms = {}dB\n", rms.rmsDb)};
    ShowConsoleMsg(totallog.c_str());
}

void AudioSource::AnalyseSamples(AudioBuffer& buffer, RMS& rms)
{

}

double AudioSource::AmpToDecibel(const double rms, const double ref)
{
    const double dB = 20.0 * std::log10(rms / ref);
    return std::round(dB * 100) / 100;
}
