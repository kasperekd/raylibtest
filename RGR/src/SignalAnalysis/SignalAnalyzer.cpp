#include "SignalAnalysis.h"

namespace SignalAnalysis {
BitSequence interpretSymbols(const Signal& signal, size_t N, double P) {
    if (N <= 0) {
        throw std::invalid_argument("N должно быть больше нуля.");
    }

    BitSequence result = {};
    size_t numSamples = signal.size();

    for (size_t i = 0; i + N <= numSamples; i += N) {
        double sum = 0.0;

        for (size_t j = 0; j < N; ++j) {
            sum += signal[i + j];
        }

        double average = sum / N;

        if (average >= P) {
            result.push_back(1);
        } else {
            result.push_back(0);
        }
    }

    return result;
}
}  // namespace SignalAnalysis