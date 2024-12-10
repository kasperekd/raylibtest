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
}  // namespace Utilities
