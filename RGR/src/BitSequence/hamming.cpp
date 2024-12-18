#include "BitSequence.h"

namespace BitSequenceModule {
BitSequence encodeHamming(const BitSequence& input) {
    BitSequence encodedMessage;

    for (size_t i = 0; i < input.size(); i += 4) {
        BitSequence block(4, 0);
        for (size_t j = 0; j < 4; ++j) {
            if (i + j < input.size()) {
                block[j] = input[i + j];
            }
        }

        // Кодирование блока
        BitSequence code(7);
        code[2] = block[0];
        code[4] = block[1];
        code[5] = block[2];
        code[6] = block[3];

        // Вычисление битов контроля
        code[0] = code[2] ^ code[4] ^ code[6];  // p1
        code[1] = code[2] ^ code[5] ^ code[6];  // p2
        code[3] = code[4] ^ code[5] ^ code[6];  // p3

        encodedMessage.insert(encodedMessage.end(), code.begin(), code.end());
    }

    return encodedMessage;
}

BitSequence decodeHamming(const BitSequence& encoded) {
    BitSequence decodedMessage;
    for (size_t i = 0; i < encoded.size(); i += 7) {
        BitSequence block(7);
        for (size_t j = 0; j < 7; ++j) {
            if (i + j < encoded.size()) {
                block[j] = encoded[i + j];
            }
        }

        // Проверка битов контроля
        int p1 = block[0] ^ block[2] ^ block[4] ^ block[6];
        int p2 = block[1] ^ block[2] ^ block[5] ^ block[6];
        int p3 = block[3] ^ block[4] ^ block[5] ^ block[6];

        int errorPosition = p1 * 1 + p2 * 2 + p3 * 4;  // позиция ошибки

        if (errorPosition != 0) {
            // cout << "errorPosition: " << errorPosition << endl;
            block[errorPosition - 1] ^= 1;  // Исправление
        }

        // Извлечение данных
        if (block.size() == 7) {
            decodedMessage.push_back(block[2]);
            decodedMessage.push_back(block[4]);
            decodedMessage.push_back(block[5]);
            decodedMessage.push_back(block[6]);
        }
    }

    return decodedMessage;
}

}  // namespace BitSequenceModule