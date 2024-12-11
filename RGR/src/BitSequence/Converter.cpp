#include "BitSequence.h"

namespace BitSequenceModule {
BitSequence encodeToASCII(const std::string& input) {
    BitSequence result;
    for (char ch : input) {
        std::bitset<8> bits(ch);
        for (int i = 7; i >= 0; --i) {
            result.push_back(bits[i]);
        }
    }
    return result;
}

std::string decodeFromASCII(const BitSequence& bits) {
    std::string result;
    if (bits.size() % 8 != 0) {
        throw std::invalid_argument(
            "Bit sequence length must be a multiple of 8");
    }

    for (size_t i = 0; i < bits.size(); i += 8) {
        std::bitset<8> byte;
        for (int j = 0; j < 8; ++j) {
            byte[7 - j] = bits[i + j];
        }
        result += static_cast<char>(byte.to_ulong());
    }
    return result;
}
BitSequence numberToBitSequence(int32_t number, size_t bitLength) {
    BitSequence result;
    for (int i = bitLength - 1; i >= 0; --i) {
        result.push_back((number >> i) & 1);
    }
    return result;
}

int32_t bitSequenceToNumber(const BitSequence& bits) {
    int32_t result = 0;
    for (size_t i = 0; i < bits.size(); ++i) {
        result <<= 1;       // Сдвигаем влево
        result |= bits[i];  // Добавляем текущий бит
    }
    return result;
}

}  // namespace BitSequenceModule