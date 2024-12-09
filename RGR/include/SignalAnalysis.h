#ifndef SIGNAL_ANALYSIS_H
#define SIGNAL_ANALYSIS_H

#include "common.h"

namespace SignalAnalysis {
int findSynchronizationPoint(const Signal& noisySignal,
                             const BitSequence& goldSequence,
                             int samplesPerBit);
BitSequence decodeSignal(const Signal& signal, int samplesPerBit,
                         float threshold);
bool validateCRC(const BitSequence& data, int crcLength);
std::string decodeASCII(const BitSequence& bits);
}  // namespace SignalAnalysis

#endif  // SIGNAL_ANALYSIS_H