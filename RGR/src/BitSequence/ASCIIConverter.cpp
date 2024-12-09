#include <bitset>

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
}  // namespace BitSequenceModule