#pragma once

#include <vector>
#include <cmath>

class AudioSource;

class RMS
{
public:

        RMS() = delete;
        explicit RMS(AudioSource& audioSource, const int windowSizeMs);

        void RegisterWindowRMS();
        void RegisterWindowSumSquaredValue(const double sample);
        [[nodiscard]] double GetRMS();
        [[nodiscard]] double GetLUFS();

        [[nodiscard]] bool IsComplete() { return bComplete; }
        [[nodiscard]] int GetNumWindows() { return numWindows; }
        [[nodiscard]] int GetCurrentWindow() const { return currentWindow; }
        [[nodiscard]] bool IsEndOfWindow(const int numProcessedSamples) const;

private:

        std::vector<double> windowRMSValues{};
        std::vector<double> windowSumSquares{};
        std::vector<double> windowLUFSValues{};
        double rmsDb{0.0};
        int currentWindow{1};
        bool bComplete{false};
        int numWindows{0};
        float samplesPerWindow{0};
        int finalWindowSize{0};
        int totalSamples{0};

        [[nodiscard]] double GetPercentageChange(const double a, const double b);
};