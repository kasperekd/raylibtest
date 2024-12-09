#ifndef SIGNAL_PROCESSING_H
#define SIGNAL_PROCESSING_H

#include "common.h"

namespace SignalProcessing {
Signal modulateSignal(const BitSequence& bits, int samplesPerBit);
Signal insertSignal(const Signal& original, int position, int totalLength);
Signal addNoise(const Signal& signal, float mean, float stddev);
std::complex<float> calculateSpectrum(const Signal& signal, int sampleRate);
}  // namespace SignalProcessing

#endif  // SIGNAL_PROCESSING_H