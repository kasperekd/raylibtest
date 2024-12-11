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
}  // namespace Utilities

#endif  // UTILITIES_H