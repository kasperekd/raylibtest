#include <iostream>
#include <string>

#include "BitSequence.h"
#include "SignalAnalysis.h"
#include "SignalProcessing.h"
#include "Utilities.h"
#include "Visualization.h"

#define N 10

int main() {
    try {
        std::vector<Visualization::GraphDescription> descriptions = {};
        // 1. Input
        std::cout << "Введите ваше имя и фамилию латиницей: ";
        std::string nameSurname = Utilities::readStringFromUser();

        // 2. Bit sequence
        BitSequence bitSequence = BitSequenceModule::encodeToASCII(nameSurname);
        Visualization::printBitSequence(bitSequence);

        Visualization::saveGraphData("./data/task2.txt", bitSequence);
        descriptions.push_back({"./data/task2.txt", "plot", "x,y",
                                "bitSequence", "Index", "Value", false});

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

        Visualization::saveGraphData("./data/task4.txt", gold);
        descriptions.push_back({"./data/task4.txt", "plot", "x,y", "gold",
                                "Index", "Value", false});

        // 5. Сборка и оверсэмплинг
        size_t messageLength = bitSequence.size();
        if (messageLength > (1 << 16)) {
            throw std::overflow_error("Размер сообщения превышает 2^16 бит.");
        }
        // BitSequence combinedSeq = Utilities::combineBitSequences(
        //     Utilities::combineBitSequences(gold, bitSequence), crc_bit);
        BitSequence len(16, 0);
        for (int i = 0; i < 16; ++i) {
            len[15 - i] = (messageLength >> i) & 1;
        }
        Visualization::printBitSequence(len, "Message len :");
        std::cout << messageLength << "\n";

        BitSequence combinedSeq = Utilities::combineBitSequences(
            Utilities::combineBitSequences(gold, len),
            Utilities::combineBitSequences(bitSequence, crc_bit));
        Signal signal = Utilities::convertToSignal(combinedSeq);
        signal = SignalProcessing::oversample(signal, N);
        size_t Nx = signal.size();  // Размер сообщения

        Visualization::printSignal(signal, N, "Signal:");

        Visualization::saveGraphData("./data/task5.txt", signal);
        descriptions.push_back({"./data/task5.txt", "plot", "x,y",
                                "Oversampled", "Index", "Value", false});

        // 6. Чтение, вставка
        Signal signalArray(Nx * 2, 0.0f);

        std::cout << "Введите число от 0 до " << Nx << "\n";
        size_t user_input = Utilities::readSize_tFromUser(0, Nx);
        std::copy(signal.begin(), signal.end(),
                  signalArray.begin() + user_input);
        Visualization::saveGraphData("./data/task6.txt", signalArray);
        descriptions.push_back({"./data/task6.txt", "plot", "x,y",
                                "signalArray", "Index", "Value", false});

        // 7. Noise
        // double mu = Utilities::readDoubleFromUser(0, 1);
        double mu = 0.0;
        std::cout << "Введите sigma от 0.0 до " << 1.0 << "\n";
        double sigma = Utilities::readDoubleFromUser(0, 1.0);
        Signal noise = SignalProcessing::generateWhiteNoise(Nx * 2, mu, sigma);

        for (size_t i = 0; (i < signalArray.size()) || (i < noise.size());
             i++) {
            signalArray[i] += noise[i];
        }

        Visualization::saveGraphData("./data/task7_1.txt", noise);
        descriptions.push_back({"./data/task7_1.txt", "plot", "x,y", "Noise",
                                "Index", "Value", false});

        Visualization::saveGraphData("./data/task7_2.txt", signalArray);
        descriptions.push_back({"./data/task7_2.txt", "plot", "x,y",
                                "Signal + noise", "Index", "Value", false});

        // 8. Прием
        BitSequence gold_oversampled = SignalProcessing::oversample(gold, N);

        Signal received = signalArray;
        // Signal received =
        //     SignalProcessing::extractEveryNthValue(signalArray, N - 1);
        size_t start_index = SignalAnalysis::findSequenceInSignal(
            received, gold_oversampled, 0.2);
        std::cout << "start_index: " << start_index << "\n";

        if (start_index <= received.size()) {
            received.erase(received.begin(), received.begin() + start_index);
        } else {
            std::cout << "Индекс выходит за пределы вектора." << std::endl;
        }

        Visualization::saveGraphData("./data/task8_1.txt", received);
        descriptions.push_back({"./data/task8_1.txt", "plot", "x,y",
                                "received - start noise", "Index", "Value",
                                false});

        // 9. downsample + resolve
        BitSequence interpreted =
            SignalAnalysis::interpretSymbols(received, N, 0.5);

        Visualization::saveGraphData("./data/task9.txt", interpreted);
        descriptions.push_back({"./data/task9.txt", "plot", "x,y",
                                "interpreted signal", "Index", "Value", false});

        // 10. -sync
        if (gold.size() <= interpreted.size()) {
            interpreted.erase(interpreted.begin(),
                              interpreted.begin() + gold.size());
        } else {
            std::cout << "Индекс выходит за пределы вектора." << std::endl;
        }

        // 10,1. get len
        size_t messageLengthReceive =
            SignalAnalysis::getMessageLength(interpreted);
        std::cout << messageLengthReceive << "\n";

        // 10,2. get message

        // Сохранение описаний для графиков
        Visualization::saveGraphDescription("./data/descriptions.txt",
                                            descriptions);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
