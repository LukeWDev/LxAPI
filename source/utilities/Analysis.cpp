#include <utilities/Analysis.hpp>
#include <utilities/AudioSource.hpp>

#include <format>
#include <reaper_plugin_functions.h>
#include <string>

using std::pow;
using std::sqrt;
using std::vector;

// filter coefficients for the K-weighting filter
constexpr double kWeighting[] = {
    0.000134953719792,
    -0.000684293878449,
    -0.001703125622262,
    0.004319411963422,
    0.010748280220502,
    -0.034369749082910,
    -0.056438536367083,
    0.221213729304326,
    0.602740664762386,
    0.222213729304326,
    -0.056438536367083,
    -0.034369749082910,
    0.010748280220502,
    0.004319411963422,
    -0.001703125622262,
    -0.000684293878449,
    0.000134953719792};

//-----------------------------------------------------------------------------

Analysis::Analysis(AudioSource& audioSource, const int windowSizeMs)
{
    totalSamples = audioSource.GetTotalSamples();
    samplesPerWindow = std::floor(
        float(audioSource.GetSampleRate()) * float(windowSizeMs / 1000.0));
    numWindows = int(std::ceil(totalSamples / samplesPerWindow));
    finalWindowSize = totalSamples - ((numWindows - 1) * int(samplesPerWindow));
    result.windowSumSquares.assign(numWindows, 0.0);
    result.windowRMSValues.assign(numWindows, 0.0);
}

//-----------------------------------------------------------------------------

double Analysis::CalculateTotalRMS(const vector<double> windowValues)
{
    double squaredWindowValues {0.0};

    for (const auto& ampVal : windowValues)
        squaredWindowValues += pow(ampVal, 2);

    const double rmsAmp = sqrt(squaredWindowValues / GetNumWindows());
    return AmpToDecibels(rmsAmp);
}

//-----------------------------------------------------------------------------

void Analysis::RegisterWindowRMS()
{
    double windowRMS = CalculateWeightedRMS(
        result.windowSumSquares[currentWindow - 1],
        GetCurrentWindowSamples());
    SetCurrentWindowRMSValue(windowRMS);

    std::string totallog {std::format(
        "\nWindow: {}. AmpRMS: {}. Samples: {}. Weighting: {}",
        GetCurrentWindow(),
        windowRMS,
        GetCurrentWindowSamples(),
        GetWindowWeighting(currentWindow))};
    ShowConsoleMsg(totallog.c_str());

    if (currentWindow == numWindows) {
        result.rmsDb = CalculateTotalRMS(result.windowRMSValues);
        bComplete = true;
        return;
    }
    ++currentWindow;
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

double Analysis::GetWindowWeighting(const int windowNum)
{
    const int currSamples = GetCurrentWindowSamples();
    const double weighting = double(currSamples) / double(totalSamples);
    return weighting;
}

//-----------------------------------------------------------------------------

double Analysis::CalculateWeightedRMS(
    const double windowSumSquares,
    const int numSamples)
{
    double scaleFactor = numSamples / (numSamples - 1.0);
    double meanPower = windowSumSquares / numSamples;
    return std::sqrt(scaleFactor * meanPower);
}

//-----------------------------------------------------------------------------

double Analysis::AmpToDecibels(const double amp)
{
    return 20.0 * std::log10(amp / 1.0);
}

//-----------------------------------------------------------------------------

int Analysis::GetCurrentWindowSamples()
{
    const int samples =
        currentWindow == numWindows ? finalWindowSize : int(samplesPerWindow);
    return samples;
}

//-----------------------------------------------------------------------------
