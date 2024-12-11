#ifndef SIGNAL_PROCESSING_H
#define SIGNAL_PROCESSING_H

#include <random>

#include "common.h"

namespace SignalProcessing {
Signal generateWhiteNoise(size_t size, float mu, float sigma);
Signal extractEveryNthValue(const Signal& inputSignal, size_t N);
std::complex<float> calculateSpectrum(const Signal& signal, int sampleRate);

// templates
// oversample
template <typename T>
T oversample(const T& input, size_t oversamplingFactor) {
    T output;
    output.reserve(input.size() * oversamplingFactor);

    for (const auto& element : input) {
        for (size_t i = 0; i < oversamplingFactor; ++i) {
            output.push_back(element);
        }
    }
    return output;
}

inline BitSequence oversample(const BitSequence& input,
                              size_t oversamplingFactor) {
    return oversample<BitSequence>(input, oversamplingFactor);
}

inline Signal oversample(const Signal& input, size_t oversamplingFactor) {
    return oversample<Signal>(input, oversamplingFactor);
}
// oversample

}  // namespace SignalProcessing

#endif  // SIGNAL_PROCESSING_H