#include <iostream>
#include <stdexcept>

#include "Visualization.h"

namespace Visualization {
void printBitSequence(const BitSequence& data) {
    std::cout << "Битовая последовательность:" << std::endl;
    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << data[i];
        if ((i + 1) % 8 == 0) std::cout << ' ';
    }
    std::cout << std::endl;
}
}  // namespace Visualization
