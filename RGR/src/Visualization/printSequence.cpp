#include "Visualization.h"

namespace Visualization {
void printBitSequence(const BitSequence& data, const std::string& customText) {
    if (!customText.empty()) {
        std::cout << customText << std::endl;
    }

    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << data[i];
        if ((i + 1) % 8 == 0) std::cout << ' ';
    }
    std::cout << std::endl;
}

void printSignal(const Signal& data, size_t N, const std::string& customText) {
    if (!customText.empty()) {
        std::cout << customText << std::endl;
    }

    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << data[i];
        if ((N > 0) && ((i + 1) % N == 0)) std::cout << ' ';
    }
    std::cout << std::endl;
}
}  // namespace Visualization
