#include "SignalAnalysis.h"

namespace SignalAnalysis {
size_t getMessageLength(const BitSequence& interpreted, size_t size) {
    if (interpreted.size() < size) {
        throw std::invalid_argument(
            "Недостаточно бит для извлечения длины сообщения.");
    }

    size_t length = 0;

    for (size_t i = 0; i < size; ++i) {
        length <<= 1;
        length |= interpreted[i];
    }

    return length;
}
}  // namespace SignalAnalysis