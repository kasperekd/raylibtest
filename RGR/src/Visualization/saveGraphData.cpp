#include "Visualization.h"

namespace Visualization {
void saveGraphData(const std::string& filePath,
                   const std::vector<std::pair<double, double>>& data) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        throw std::ios_base::failure("|saveGraphData| Failed to open file: " +
                                     filePath);
    }

    for (const auto& point : data) {
        file << point.first << "; " << point.second << "\n";
    }

    file.close();
}

void saveGraphData(const std::string& filePath, const Signal& data) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        throw std::ios_base::failure("|saveGraphData| Failed to open file: " +
                                     filePath);
    }

    for (size_t i = 0; i < data.size(); ++i) {
        file << data[i] << "; " << i << "\n";
    }

    file.close();
}

void saveGraphData(const std::string& filePath, const BitSequence& data) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        throw std::ios_base::failure("|saveGraphData| Failed to open file: " +
                                     filePath);
    }

    for (size_t i = 0; i < data.size(); ++i) {
        file << data[i] << "; " << i << "\n";
    }

    file.close();
}
}  // namespace Visualization