#include <iostream>
#include <string>

#include "BitSequence.h"
#include "Utilities.h"
#include "Visualization.h"

int main() {
    try {
        // 1. Input
        std::cout << "Введите ваше имя и фамилию латиницей: ";
        std::string nameSurname = Utilities::readStringFromUser();

        // 2. Bit sequence
        BitSequence bitSequence = BitSequenceModule::encodeToASCII(nameSurname);
        Visualization::printBitSequence(bitSequence);

        // 3. CRC
        // TODO: Унифицровать crc по стандартам
        constexpr int CRC_LENGTH = 16;
        // TODO: Возвращать число и отдельной функцие конвертировать
        BitSequence crc =
            BitSequenceModule::generateCRC(bitSequence, CRC_LENGTH);

        std::cout << "CRC (" << CRC_LENGTH << " bit): ";
        Visualization::printBitSequence(crc);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
