#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <string>
#include <variant>
#include <vector>

#include "common.h"

namespace Utilities {
std::string readStringFromUser();
BitSequence combineBitSequences(const BitSequence& vec1,
                                const BitSequence& vec2);
Signal convertToSignal(const BitSequence& bitSequence);
void writeToFile(const std::string& filePath, const Signal& data);
void writeToFile(const std::string& filePath, const BitSequence& data);
void readFromFile(const std::string& filePath, Signal& data);
}  // namespace Utilities

#endif  // UTILITIES_H