#include "SignalAnalysis.h"

namespace SignalAnalysis {
size_t findSequenceInSignal(const Signal& signal, const BitSequence& sequence,
                            double threshold) {
    size_t signalSize = signal.size();
    size_t goldSize = sequence.size();

    double maxCorrelation = -std::numeric_limits<double>::infinity();
    size_t bestMatchIndex = 0;

    for (size_t i = 0; i <= signalSize - goldSize; ++i) {
        BitSequence window(goldSize);

        for (size_t j = 0; j < goldSize; ++j) {
            window[j] = (signal[i + j] > 0) ? 1 : 0;
        }

        double normalizedCorrelation =
            SignalAnalysis::normalizedCorrelation(window, sequence);
        // double normalizedCorrelation =
        //     SignalAnalysis::normalizeCorrelation(correlation, goldSize);

        // std::cout << normalizedCorrelation << " " << correlation << "\n";
        if (normalizedCorrelation > maxCorrelation) {
            // std::cout << normalizedCorrelation << "\n";
            maxCorrelation = normalizedCorrelation;
            bestMatchIndex = i;
        }
    }

    if (maxCorrelation >= threshold) {
        // std::cout << maxCorrelation << "\n";
        return bestMatchIndex;
    } else {
        return std::numeric_limits<size_t>::max();  // не найдено
    }
}
}  // namespace SignalAnalysis