#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <string>
#include <variant>
#include <vector>

#include "common.h"

namespace Utilities {
std::string readStringFromUser();
size_t readSize_tFromUser(size_t min = 0,
                          size_t = std::numeric_limits<size_t>::max());
double readDoubleFromUser(double min = std::numeric_limits<double>::lowest(),
                          double max = std::numeric_limits<double>::max());
BitSequence combineBitSequences(const BitSequence& vec1,
                                const BitSequence& vec2);
Signal convertToSignal(const BitSequence& bitSequence);
void insertVector(BitSequence& target, const BitSequence& source,
                  size_t position);
void insertVector(Signal& target, const Signal& source, size_t position);
void removeRange(BitSequence& bits, size_t start, size_t end);
void removeRange(Signal& bits, size_t start, size_t end);
BitSequence getRange(const BitSequence& bits, size_t start, size_t end);
Signal getRange(const Signal& bits, size_t start, size_t end);
}  // namespace Utilities

#endif  // UTILITIES_H