#include "Visualization.h"

namespace Visualization {
void saveGraphDescription(const std::string& metadataFilePath,
                          const std::vector<GraphDescription>& descriptions) {
    std::ofstream file(metadataFilePath);
    if (!file.is_open()) {
        throw std::ios_base::failure(
            "|saveGraphDescription| Failed to open metadata file: " +
            metadataFilePath);
    }

    for (const auto& desc : descriptions) {
        file << desc.filePath << "; " << desc.plotType << "; " << desc.columns
             << "; "
             << "\"" << desc.title << "\"; "
             << "\"" << desc.xLabel << "\"; "
             << "\"" << desc.yLabel << "\"; "
             << (desc.overlay ? "true" : "false") << "\n";
    }

    file.close();
}

}  // namespace Visualization