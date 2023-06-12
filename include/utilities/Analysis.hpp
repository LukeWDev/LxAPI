#pragma once

#include <cmath>
#include <vector>

class AudioSource;

class Analysis {
    struct Results {
        std::vector<double> windowRMSValues {};
        std::vector<double> windowSumSquares {};
        double rmsDb {-150.0};
        double lufsDb {-150.0};
    };

  public:
    Analysis() = delete;
    explicit Analysis(AudioSource& audioSource, const int windowSizeMs);

    void RegisterWindowRMS();

    void SetCurrentWindowSumValue(const double value)
    {
        result.windowSumSquares[currentWindow - 1] += value;
    }
    void SetCurrentWindowRMSValue(const double value)
    {
        result.windowRMSValues[currentWindow - 1] += value;
    }
    [[nodiscard]] bool IsComplete()
    {
        return bComplete;
    }
    [[nodiscard]] int GetNumWindows()
    {
        return numWindows;
    }
    [[nodiscard]] int GetCurrentWindow() const
    {
        return currentWindow;
    }
    [[nodiscard]] int GetCurrentWindowSamples();
    [[nodiscard]] bool IsEndOfWindow(const int numProcessedSamples) const;

    [[nodiscard]] Results GetResults()
    {
        return result;
    }

  private:
    Results result;

    int currentWindow {1};
    bool bComplete {false};
    int numWindows {0};
    float samplesPerWindow {0};
    int finalWindowSize {0};
    int totalSamples {0};

    [[nodiscard]] double CalculateTotalRMS(
        const std::vector<double> windowValues);
    [[nodiscard]] double GetPercentageChange(const double a, const double b);
    [[nodiscard]] double GetWindowWeighting(const int windowNum);
    [[nodiscard]] double CalculateWeightedRMS(
        const double windowSumSquares,
        const int numSamples);
    [[nodiscard]] double AmpToDecibels(const double amp);
};
