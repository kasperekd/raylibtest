#ifndef UTILITIES_H
#define UTILITIES_H

#include "common.h"

namespace Utilities {
std::string readStringFromUser();
void writeToFile(const std::string& filePath, const Signal& data);
void writeToFile(const std::string& filePath, const BitSequence& data);
void readFromFile(const std::string& filePath, Signal& data);
}  // namespace Utilities

#endif  // UTILITIES_H