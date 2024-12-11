#ifndef SIGNAL_ANALYSIS_H
#define SIGNAL_ANALYSIS_H

#include "common.h"

namespace SignalAnalysis {
double normalizedCorrelation(const BitSequence& seq1, const BitSequence& seq2);
double Correlation(const BitSequence& seq1, const BitSequence& seq2);
size_t findSequenceInSignal(const Signal& signal, const BitSequence& sequence,
                            double threshold = 0.7);
BitSequence interpretSymbols(const Signal& signal, size_t N, double P);
size_t getMessageLength(const BitSequence& interpreted, size_t size = 16);
int findSynchronizationPoint(const Signal& noisySignal,
                             const BitSequence& goldSequence,
                             int samplesPerBit);
BitSequence decodeSignal(const Signal& signal, int samplesPerBit,
                         float threshold);
bool validateCRC(const BitSequence& data, int crcLength);
std::string decodeASCII(const BitSequence& bits);
}  // namespace SignalAnalysis

#endif  // SIGNAL_ANALYSIS_H