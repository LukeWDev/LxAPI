#pragma once

#include <vector>
#include <cmath>

class AudioSource;


class Analysis
{
        struct rmsResults
        {
                std::vector<double> windowRMSValues{};
                std::vector<double> windowSumSquares{};
                double rmsDb{-150.0};
        };
        struct lufsResults : rmsResults
        {
                std::vector<double> windowLUFSValues{};
                double lufsDb{-150.0};
        };
public:

        Analysis() = delete;
        explicit Analysis(AudioSource& audioSource, const int windowSizeMs);

        void RegisterWindowRMS();
        void RegisterWindowLUFS();
        void RegisterWindowSumSquaredValue(const double sample) {  rms.windowSumSquares[currentWindow - 1] += sample; }

        [[nodiscard]] bool IsComplete() { return bComplete; }
        [[nodiscard]] int GetNumWindows() { return numWindows; }
        [[nodiscard]] int GetCurrentWindow() const { return currentWindow; }
        [[nodiscard]] bool IsEndOfWindow(const int numProcessedSamples) const;

        [[nodiscard]] rmsResults GetRMSResults() { return rms; }
        [[nodiscard]] lufsResults GetLUFSResults() { return lufs; }

private:

        rmsResults rms;
        lufsResults lufs;

        int currentWindow{1};
        bool bComplete{false};
        int numWindows{0};
        float samplesPerWindow{0};
        int finalWindowSize{0};
        int totalSamples{0};

        [[nodiscard]] void CalculateRMS();
        [[nodiscard]] void CalculateLUFS();
        [[nodiscard]] double GetPercentageChange(const double a, const double b);
};
