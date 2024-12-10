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
        constexpr size_t CRC_LENGTH = 16;
        constexpr BitSequenceModule::CRCType CRC_TYPE =
            BitSequenceModule::CRCType::CRC16;

        u_int32_t crc =
            BitSequenceModule::generateCRC(bitSequence, CRC_TYPE, CRC_LENGTH);

        std::cout << "CRC: " << std::hex << crc << std::dec << "\n";
        BitSequence crc_bit =
            BitSequenceModule::numberToBitSequence(crc, CRC_LENGTH);
        Visualization::printBitSequence(crc_bit, "CRC bits:");

        // 4. GOLD
        // TODO: Визуализировать на графике ? мб автокорреляцию
        BitSequence gold = BitSequenceModule::generateGoldSequence();
        Visualization::printBitSequence(gold, "GOLD bits:");

        // 5. Сборка и оверсэмплинг
        BitSequence combinedSeq = Utilities::combineBitSequences(
            Utilities::combineBitSequences(gold, bitSequence), crc_bit);
        Signal signal = Utilities::convertToSignal(combinedSeq);
        Visualization::printSignal(signal, 10, "Signal:");
        // TODO: Остановился на оверсемплинге

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
