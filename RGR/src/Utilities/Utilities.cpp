#include "Utilities.h"

namespace Utilities {
BitSequence combineBitSequences(const BitSequence& vec1,
                                const BitSequence& vec2) {
    BitSequence combined;
    combined.reserve(vec1.size() + vec2.size());

    combined.insert(combined.end(), vec1.begin(), vec1.end());
    combined.insert(combined.end(), vec2.begin(), vec2.end());

    return combined;
}

Signal convertToSignal(const BitSequence& bitSequence) {
    Signal signal;
    signal.reserve(bitSequence.size());

    for (const auto& bit : bitSequence) {
        signal.push_back(static_cast<float>(bit));
    }

    return signal;
}

void insertVector(BitSequence& target, const BitSequence& source,
                  size_t position) {
    if (position > target.size()) {
        throw std::out_of_range("Позиция вставки выходит за пределы вектора.");
    }

    target.insert(target.begin() + position, source.begin(), source.end());
}

void insertVector(Signal& target, const Signal& source, size_t position) {
    if (position > target.size()) {
        throw std::out_of_range("Позиция вставки выходит за пределы вектора.");
    }

    target.insert(target.begin() + position, source.begin(), source.end());
}

void removeRange(BitSequence& bits, size_t start, size_t end) {
    if (start >= bits.size()) {
        throw std::out_of_range("Начальный индекс выходит за пределы вектора.");
    } else if (end > bits.size()) {
        end = bits.size();
    } else if (start == end) {
        return;
    } else if (start >= end) {
        throw std::invalid_argument(
            "Начальный индекс должен быть меньше конечного индекса.");
    }

    bits.erase(bits.begin() + start, bits.begin() + end);
}

void removeRange(Signal& bits, size_t start, size_t end) {
    if (start >= bits.size()) {
        throw std::out_of_range("Начальный индекс выходит за пределы вектора.");
    } else if (end > bits.size()) {
        end = bits.size();
    } else if (start == end) {
        return;
    } else if (start >= end) {
        throw std::invalid_argument(
            "Начальный индекс должен быть меньше конечного индекса.");
    }

    bits.erase(bits.begin() + start, bits.begin() + end);
}

BitSequence getRange(const BitSequence& bits, size_t start, size_t end) {
    if (start >= bits.size()) {
        throw std::out_of_range("Начальный индекс выходит за пределы вектора.");
    } else if (end > bits.size()) {
        end = bits.size();
    } else if (start == end) {
        return bits;
    } else if (start >= end) {
        throw std::invalid_argument(
            "Начальный индекс должен быть меньше конечного индекса.");
    }

    return BitSequence(bits.begin() + start, bits.begin() + end);
}

Signal getRange(const Signal& bits, size_t start, size_t end) {
    if (start >= bits.size()) {
        throw std::out_of_range("Начальный индекс выходит за пределы вектора.");
    } else if (end > bits.size()) {
        end = bits.size();
    } else if (start == end) {
        return bits;
    } else if (start >= end) {
        throw std::invalid_argument(
            "Начальный индекс должен быть меньше конечного индекса.");
    }

    return Signal(bits.begin() + start, bits.begin() + end);
}

}  // namespace Utilities
