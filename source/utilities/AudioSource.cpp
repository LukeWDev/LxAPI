#include <utilities/AudioSource.hpp>
#include <utilities/AudioBuffer.hpp>
#include <utilities/Analysis.hpp>

// DEBUG
#include <reaper_plugin_functions.h>
#include <string>
#include <format>
#include <sstream>
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

    int numProcessedSamples{0};

    while (buffer.SamplesOut()) // while there are samples to process
    {   
        if (analysis.IsComplete())
            break;

        for (int currentSample = 0; currentSample < buffer.SamplesOut(); currentSample++)
        {
            ++numProcessedSamples;

            const ReaSample sampleValue = buffer.GetSampleAt(currentSample);
            analysis.RegisterWindowSumSquaredValue(std::pow(sampleValue, 2));

            if (analysis.IsEndOfWindow(numProcessedSamples))
            {
                analysis.RegisterWindowRMS();
            }
        }
        buffer.RefillSamples();
    }
    const auto rmsResult = analysis.GetRMSResults().rmsDb;
    std::string rmsresult{std::format("\ntotal rms = {}dB\n", rmsResult)};
    ShowConsoleMsg(rmsresult.c_str());

    return rmsResult;
}

//-----------------------------------------------------------------------------

double AudioSource::GetSourceLUFS(const int bufferSize, const int windowSizeMs)
{
    AudioBuffer buffer(*this, bufferSize);
    Analysis analysis(*this, windowSizeMs);

     int numProcessedSamples{0};

    while (buffer.SamplesOut()) // while there are samples to process
    {   
        if (analysis.IsComplete())
            break;

        for (int currentSample = 0; currentSample < buffer.SamplesOut(); currentSample++)
        {
            ++numProcessedSamples;

            const ReaSample sampleValue = buffer.GetSampleAt(currentSample);
            analysis.RegisterWindowSumSquaredValue(std::pow(sampleValue, 2));

            if (analysis.IsEndOfWindow(numProcessedSamples))
            {
                analysis.RegisterWindowRMS();
            }
        }
        buffer.RefillSamples();
    }

    const auto lufsResult = analysis.GetLUFSResults().lufsDb;
    std::string lufsresult{std::format("\ntotal rms = {}dB\n", lufsResult)};
    ShowConsoleMsg(lufsresult.c_str());

    return lufsResult;
}

//-----------------------------------------------------------------------------