#include <utilities/Analysis.hpp>
#include <utilities/AudioBuffer.hpp>
#include <utilities/AudioSource.hpp>

// DEBUG
#include <format>
#include <reaper_plugin_functions.h>
#include <sstream>
#include <string>
// DEBUG

using std::vector;

//-----------------------------------------------------------------------------

AudioSource::AudioSource(PCM_source* source)
{
    audioSourcePtr = source;
}

//-----------------------------------------------------------------------------

double AudioSource::GetSourceRMS(const int bufferSize, const int windowSizeMs)
{
    AudioBuffer buffer(*this, bufferSize);
    Analysis analysis(*this, windowSizeMs);

    int numProcessedSamples {0};

    while (buffer.SamplesOut()) // while there are samples to process
    {
        if (analysis.IsComplete())
            break;

        for (int currentSample = 0; currentSample < buffer.SamplesOut();
             currentSample++) {
            ++numProcessedSamples;

            const ReaSample sampleValue = buffer.GetSampleAt(currentSample);
            analysis.SetCurrentWindowSumValue(std::pow(sampleValue, 2));

            if (analysis.IsEndOfWindow(numProcessedSamples)) {
                analysis.RegisterWindowRMS();
            }
        }
        buffer.RefillSamples();
    }
    const auto rmsResult = analysis.GetResults().rmsDb;
    std::string rmsresult {std::format("\nrms = {}dB.\n", rmsResult)};
    ShowConsoleMsg(rmsresult.c_str());

    return rmsResult;
}

//-----------------------------------------------------------------------------
