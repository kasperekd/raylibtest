#include "BitSequence.h"

namespace BitSequenceModule {
u_int32_t generateCRC(const BitSequence& data, CRCType crcType,
                      size_t crcLength) {
    if (crcLength <= 0) {
        throw std::invalid_argument(
            "The CRC length must be a positive number.");
    }

    u_int32_t crc = 0;
    BitSequence augmentedData = data;

    switch (crcType) {
        case CRCType::CRC16: {
            const u_int16_t polynomial = 0xA001;  // Полином для CRC-16
            crc = 0;  // Инициализация CRC
            augmentedData.insert(augmentedData.end(), crcLength, 0);

            for (size_t i = 0; i < augmentedData.size(); ++i) {
                crc ^= (augmentedData[i] << 8);
                for (size_t j = 0; j < 8; ++j) {
                    if (crc & 0x8000) {
                        crc = (crc << 1) ^ polynomial;
                    } else {
                        crc <<= 1;
                    }
                }
            }
            crc &= 0xFFFF;
            break;
        }
        case CRCType::CRC32: {
            const u_int32_t polynomial = 0xEDB88320;  // Полином для CRC-32
            crc = 0xFFFFFFFF;
            augmentedData.insert(augmentedData.end(), crcLength, 0);

            for (size_t i = 0; i < augmentedData.size(); ++i) {
                crc ^= augmentedData[i];
                for (size_t j = 0; j < 8; ++j) {
                    if (crc & 1) {
                        crc = (crc >> 1) ^ polynomial;
                    } else {
                        crc >>= 1;
                    }
                }
            }
            crc ^= 0xFFFFFFFF;
            break;
        }
        default:
            throw std::invalid_argument("Unsupported CRC type.");
    }

    return crc;
}
}  // namespace BitSequenceModule
