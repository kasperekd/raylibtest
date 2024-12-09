#ifndef BIT_SEQUENCE_H
#define BIT_SEQUENCE_H

#include "common.h"

namespace BitSequenceModule {
BitSequence encodeToASCII(const std::string& input);
BitSequence generateCRC(const BitSequence& sequence, int crcLength);
BitSequence generateGoldSequence(int length);
}  // namespace BitSequenceModule

#endif  // BIT_SEQUENCE_H