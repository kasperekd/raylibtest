#ifndef BIT_SEQUENCE_H
#define BIT_SEQUENCE_H

#include <string.h>

#include <bitset>
#include <cstdint>

#include "common.h"

namespace BitSequenceModule {
enum class CRCType {
    CRC16,
    CRC32,
    //...
};
BitSequence encodeToASCII(const std::string& input);
BitSequence numberToBitSequence(int32_t number, size_t bitLength);
u_int32_t generateCRC(const BitSequence& data, CRCType crcType,
                      size_t crcLength);
BitSequence generateGoldSequence(uint8_t x_start = 0b01101,
                                 uint8_t y_start = 0b10100);
}  // namespace BitSequenceModule

#endif  // BIT_SEQUENCE_H