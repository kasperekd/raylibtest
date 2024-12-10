#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <string>

#include "common.h"

namespace Visualization {
void printBitSequence(const BitSequence& data,
                      const std::string& customText = "");
void printSignal(const Signal& data, size_t N = 0,
                 const std::string& customText = "");
void prepareVisualizationFile(const std::string& filePath, const Signal& data,
                              const std::string& plotType,
                              const std::string& title,
                              const std::string& xAxisLabel,
                              const std::string& yAxisLabel);
void prepareVisualizationFile(const std::string& filePath,
                              const BitSequence& data,
                              const std::string& plotType,
                              const std::string& title,
                              const std::string& xAxisLabel,
                              const std::string& yAxisLabel);
}  // namespace Visualization

#endif  // VISUALIZATION_H