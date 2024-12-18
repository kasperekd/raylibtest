#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "common.h"

namespace Visualization {
struct GraphDescription {
    std::string filePath = ".";     // Path to data file
    std::string plotType = "plot";  // "plot", "stem"
    std::string columns = "x,y";    // Columns used for plotting (e.g., "x,y")
    std::string title = "title";    // Title of the graph
    std::string xLabel = "X";       // X-axis label
    std::string yLabel = "Y";       // Y-axis label
    bool overlay = false;  // Flag to indicate whether to overlay this graph
};

void printBitSequence(const BitSequence& data,
                      const std::string& customText = "");
void printSignal(const Signal& data, size_t N = 0,
                 const std::string& customText = "");

void saveGraphDescription(const std::string& metadataFilePath,
                          const std::vector<GraphDescription>& descriptions);
// TODO: Изменить на темплейты ?
// TODO: Добавить насильную запись полного пути для файла
void saveGraphData(const std::string& filePath, const Signal& data);
void saveGraphData(const std::string& filePath, const BitSequence& data);
void saveGraphData(const std::string& filePath,
                   const std::vector<std::pair<double, double>>& data);
void saveGraphData(const std::string& filePath,
                   const std::vector<std::pair<double, int>>& data);
void saveGraphData(const std::string& filePath,
                   const std::vector<std::pair<int, double>>& data);
}  // namespace Visualization

#endif  // VISUALIZATION_H