#include "BitSequence.h"

namespace BitSequenceModule {
BitSequence generateCRC(const BitSequence& data, int crcLength) {
    if (crcLength <= 0) {
        throw std::invalid_argument(
            "The CRC length must be a positive number.");
    }

    BitSequence crc(crcLength, 0);
    BitSequence augmentedData = data;
    augmentedData.insert(augmentedData.end(), crc.begin(), crc.end());

    // для CRC-16-CCITT
    const unsigned int polynomial = 0x1021;

    for (size_t i = 0; i < data.size(); ++i) {
        if (augmentedData[i] == 1) {
            for (int j = 0; j < crcLength; ++j) {
                augmentedData[i + j] ^= (polynomial >> (crcLength - 1 - j)) & 1;
            }
        }
    }

    for (int i = 0; i < crcLength; ++i) {
        crc[i] = augmentedData[data.size() + i];
    }
    return crc;
}
}  // namespace BitSequenceModule