#include <utilities/Analysis.hpp>
#include <utilities/AudioSource.hpp>

#include <reaper_plugin_functions.h>
#include <string>
#include <format>

constexpr double kWeighting[] = {0.000134953719792, -0.000684293878449, -0.001703125622262, 0.004319411963422, 0.010748280220502, -0.034369749082910, -0.056438536367083, 0.221213729304326, 0.602740664762386, 0.222213729304326, -0.056438536367083, -0.034369749082910, 0.010748280220502, 0.004319411963422, -0.001703125622262, -0.000684293878449, 0.000134953719792};

//-----------------------------------------------------------------------------

Analysis::Analysis(AudioSource& audioSource, const int windowSizeMs)
{
    totalSamples = audioSource.GetTotalSamples();
    samplesPerWindow = std::floor(float(audioSource.GetSampleRate()) * float(windowSizeMs / 1000.0));
    numWindows = int(std::ceil(totalSamples / samplesPerWindow));
    finalWindowSize = totalSamples - ((numWindows - 1) * int(samplesPerWindow));
    rms.windowSumSquares.assign(numWindows, 0.0);
    rms.windowRMSValues.assign(numWindows, 0.0);
}

//-----------------------------------------------------------------------------

void Analysis::CalculateRMS()
{
    double squaredWindowValues{0.0};

    for (const auto& ampVal : rms.windowRMSValues)
        squaredWindowValues += std::pow(ampVal, 2);
    const double rmsAmp = std::sqrt(squaredWindowValues / GetNumWindows());
    const double rmsDecibel = 20.0 * std::log10(rmsAmp / 1.0); // convert Amp to dB
    rms.rmsDb = std::round(rmsDecibel * 100) / 100;
    bComplete = true;
}

//-----------------------------------------------------------------------------

void Analysis::CalculateLUFS()
{
   
}

//-----------------------------------------------------------------------------

void Analysis::RegisterWindowRMS()
{
    const int windowSamples = currentWindow == numWindows ? finalWindowSize : int(samplesPerWindow);
    double windowRMS{0.0};
    // check if it's the last window and if it contains an uneven number of samples
    if  (currentWindow == numWindows && finalWindowSize != int(samplesPerWindow))
    {
        // perform average weighted RMS calculation
        windowRMS = std::sqrt(rms.windowSumSquares[currentWindow - 1] / windowSamples);

        const double finalWindowChange = GetPercentageChange(double(finalWindowSize), double(samplesPerWindow));
        if (finalWindowChange > 40)
        {
            std::string perc{std::format("\nPercentage Change ({})% > 25%.", finalWindowChange)};
            ShowConsoleMsg(perc.c_str());
        }
    }
    else
    {
        windowRMS = std::sqrt(rms.windowSumSquares[currentWindow - 1] / windowSamples);
    }
    
    rms.windowRMSValues[currentWindow - 1] += windowRMS;

    std::string totallog{std::format("\nWindow: {}. AmpRMS: {}. Samples: {}.", GetCurrentWindow(), windowRMS, windowSamples)};
    ShowConsoleMsg(totallog.c_str());

    if (currentWindow == numWindows)
    {
        CalculateRMS();
        bComplete = true;
        return;
    }
    ++currentWindow;
}

//-----------------------------------------------------------------------------

void Analysis::RegisterWindowLUFS()
{
    
}

//-----------------------------------------------------------------------------

bool Analysis::IsEndOfWindow(const int numProcessedSamples) const 
{
    if (numProcessedSamples == samplesPerWindow * currentWindow)
        return true;    

    if (currentWindow == numWindows && numProcessedSamples == totalSamples)
        return true;

    return false;
}

//-----------------------------------------------------------------------------

double Analysis::GetPercentageChange(const double a, const double b)
{
    const double percentageChange = ((b - a) / std::abs(b)) * 100.0;
    return std::round(percentageChange * 100) / 100;
}

//-----------------------------------------------------------------------------
