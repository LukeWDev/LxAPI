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
    RMS rms(*this, windowSizeMs);

    int numProcessedSamples{0};

    while (buffer.SamplesOut()) // while there are samples to process
    {   
        if (rms.IsComplete())
            break;

        for (int currentSample = 0; currentSample < buffer.SamplesOut(); currentSample++)
        {
            ++numProcessedSamples;

            const ReaSample sampleValue = buffer.GetSampleAt(currentSample);
            rms.RegisterWindowSumSquaredValue(std::pow(sampleValue, 2));

            if (rms.IsEndOfWindow(numProcessedSamples))
            {
                rms.RegisterWindowRMS();
            }
        }
        buffer.RefillSamples();
    }
    std::string totallog{std::format("\ntotal rms = {}dB\n", rms.GetRMS())};
    ShowConsoleMsg(totallog.c_str());

    return rms.GetRMS();
}

//-----------------------------------------------------------------------------

double AudioSource::GetSourceLUFS(const int bufferSize, const int windowSizeMs)
{
    return -150.0;
}

//-----------------------------------------------------------------------------