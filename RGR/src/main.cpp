#include <iostream>
#include <string>

#include "BitSequence.h"
#include "SignalAnalysis.h"
#include "SignalProcessing.h"
#include "Utilities.h"
#include "Visualization.h"

// #define MAIN 1
#define N 10

int additionalTask(std::vector<Visualization::GraphDescription>& descriptions) {
    BitSequence bitSequence = BitSequenceModule::encodeToASCII("we we");
    BitSequence bitSequence_ = BitSequenceModule::encodeHamming(bitSequence);

    constexpr size_t CRC_LENGTH = 16;
    constexpr BitSequenceModule::CRCType CRC_TYPE = BitSequenceModule::CRCType::CRC16;
    u_int32_t crc = BitSequenceModule::generateCRC(bitSequence, CRC_TYPE, CRC_LENGTH);
    u_int32_t crc_ = BitSequenceModule::generateCRC(bitSequence_, CRC_TYPE, CRC_LENGTH);

    BitSequence crc_bit = BitSequenceModule::numberToBitSequence(crc, CRC_LENGTH);
    BitSequence crc_bit_ = BitSequenceModule::numberToBitSequence(crc_, CRC_LENGTH);
    BitSequence gold = BitSequenceModule::generateGoldSequence();

    size_t messageLength = bitSequence.size();
    size_t messageLength_ = bitSequence_.size();
    if ((messageLength > (1 << 16)) && (messageLength > (1 << 16))) {
        throw std::overflow_error("Размер сообщения превышает 2^16 бит.");
    }

    BitSequence len(16, 0);
    for (int i = 0; i < 16; ++i) {
        len[15 - i] = (messageLength >> i) & 1;
    }

    BitSequence len_(16, 0);
    for (int i = 0; i < 16; ++i) {
        len_[15 - i] = (messageLength_ >> i) & 1;
    }

    BitSequence combinedSeq = Utilities::combineBitSequences(
        Utilities::combineBitSequences(gold, len),
        Utilities::combineBitSequences(bitSequence, crc_bit));
    Signal signal = Utilities::convertToSignal(combinedSeq);
    signal = SignalProcessing::oversample(signal, N);
    size_t Nx = signal.size();

    BitSequence combinedSeq_ = Utilities::combineBitSequences(
        Utilities::combineBitSequences(gold, len_),
        Utilities::combineBitSequences(bitSequence_, crc_bit_));
    Signal signal_ = Utilities::convertToSignal(combinedSeq_);
    signal_ = SignalProcessing::oversample(signal_, N);
    size_t Nx_ = signal_.size();

    double mu = 0.0;
    std::vector<std::pair<int, double>> sigmaErrors;
    std::vector<std::pair<int, double>> sigmaErrors_;
    std::vector<std::pair<int, double>> sigmaErrors__;

    size_t goldSize = gold.size() * N;
    size_t lenStart = 0;
    size_t lenEnd = lenStart + 16 * N + goldSize;

    const int numIterations = 20;

    for (double sigma = 0.0; sigma <= 0.9; sigma += 0.01) {
            Signal signalArray = signal;
        Signal signalArray_ = signal_;
        double totalBitErrors = 0;
        double totalBitErrors_ = 0;
        double totalBitErrors__ = 0;

        for (int iteration = 0; iteration < numIterations; iteration++) {
            Signal noise = SignalProcessing::generateWhiteNoise(Nx_, mu, sigma);

            for (size_t i = 0; i < signalArray.size() && i < noise.size(); i++) {
                if (i < lenStart || i >= lenEnd) {  // Пропускаем диапазон
                    signalArray[i] += noise[i];
                }
            }

            for (size_t i = 0; i < signalArray_.size() && i < noise.size(); i++) {
                if (i < lenStart || i >= lenEnd) {  // Пропускаем диапазон
                    signalArray_[i] += noise[i];
                }
            }

            // Прием
            BitSequence gold_oversampled = SignalProcessing::oversample(gold, N);
            Signal received = signalArray;
            size_t start_index = SignalAnalysis::findSequenceInSignal(received, gold_oversampled, 0.6);
            Utilities::removeRange(received, 0, start_index);
            BitSequence interpreted = SignalAnalysis::interpretSymbols(received, N, sigma);
            Utilities::removeRange(interpreted, 0, gold.size());
            BitSequence receivedLen = Utilities::getRange(interpreted, 0, 16);
            Utilities::removeRange(interpreted, 0, 16);
            size_t messageLengthReceive = SignalAnalysis::getMessageLength(receivedLen);
            BitSequence receivedBitMessage = Utilities::getRange(interpreted, 0, messageLengthReceive);

            Signal received_ = signalArray_;
            size_t start_index_ = SignalAnalysis::findSequenceInSignal(received_, gold_oversampled, 0.6);
            Utilities::removeRange(received_, 0, start_index_);
            BitSequence interpreted_ = SignalAnalysis::interpretSymbols(received_, N, sigma);
            Utilities::removeRange(interpreted_, 0, gold.size());
            BitSequence receivedLen_ = Utilities::getRange(interpreted_, 0, 16);
            Utilities::removeRange(interpreted_, 0, 16);
            size_t messageLengthReceive_ = SignalAnalysis::getMessageLength(receivedLen_);
            BitSequence receivedBitMessage_ = Utilities::getRange(interpreted_, 0, messageLengthReceive_);

             size_t bitErrors = 0;
        for (size_t i = 0; i < receivedBitMessage.size(); ++i) {
            if (i < bitSequence.size() &&
                receivedBitMessage[i] != bitSequence[i]) {
                ++bitErrors;
            }
        }
        totalBitErrors+= bitErrors;
        size_t bitErrors__ = 0;
        for (size_t i = 0; i < receivedBitMessage_.size(); ++i) {
            if (i < bitSequence.size() &&
                receivedBitMessage_[i] != bitSequence_[i]) {
                ++bitErrors__;
            }
        }
        totalBitErrors__ +=bitErrors__;
        receivedBitMessage_ =
            BitSequenceModule::decodeHamming(receivedBitMessage_);
        // BitSequenceModule::decodeHammingWithParity(receivedBitMessage_);
        size_t bitErrors_ = 0;
        for (size_t i = 0; i < receivedBitMessage_.size(); ++i) {
            if (i < bitSequence.size() &&
                receivedBitMessage_[i] != bitSequence[i]) {
                ++bitErrors_;
            }
        }
        totalBitErrors_ += bitErrors_;
        }

        // size_t averageBitErrors = totalBitErrors;
        // size_t averageBitErrors_ = totalBitErrors_;
        // size_t averageBitErrors__ = totalBitErrors__ ;
        size_t averageBitErrors = totalBitErrors / numIterations;
        size_t averageBitErrors_ = totalBitErrors_ / numIterations;
        size_t averageBitErrors__ = totalBitErrors__ / numIterations;

        sigmaErrors.emplace_back(averageBitErrors, sigma);
        sigmaErrors_.emplace_back(averageBitErrors_, sigma);
        sigmaErrors__.emplace_back(averageBitErrors__, sigma);

        // std::cout << "Sigma: " << sigma << " | Average Bit errors: " << averageBitErrors << "\n";
    }

    Visualization::saveGraphData("./data/(1,3,1)extra_1.txt", sigmaErrors);
    descriptions.push_back({"./data/(1,3,1)extra_1.txt", "plot", "x,y",
                            "Message len: " + std::to_string(messageLength),
                            "sigma", "bit err", true});
    Visualization::saveGraphData("./data/(1,3,2)extra_2.txt", sigmaErrors_);
    descriptions.push_back(
        {"./data/(1,3,2)extra_2.txt", "plot", "x,y",
         "Hamming. Message len: " + std::to_string(messageLength), "sigma",
         "bit err", true});
    Visualization::saveGraphData("./data/(1,3,3)extra_3.txt", sigmaErrors__);
    descriptions.push_back(
        {"./data/(1,3,3)extra_3.txt", "plot", "x,y",
         "Hamming (origin). Message len: " + std::to_string(messageLength),
         "sigma", "bit err", true});

    return 0;

}




// int additionalTask(std::vector<Visualization::GraphDescription>& descriptions) {
//     BitSequence bitSequence = BitSequenceModule::encodeToASCII("we we");

//     BitSequence bitSequence_ = BitSequenceModule::encodeHamming(bitSequence);
//     // BitSequence bitSequence_ =
//     //     BitSequenceModule::decodeHammingWithParity(bitSequence_);

//     constexpr size_t CRC_LENGTH = 16;
//     constexpr BitSequenceModule::CRCType CRC_TYPE =
//         BitSequenceModule::CRCType::CRC16;
//     u_int32_t crc =
//         BitSequenceModule::generateCRC(bitSequence, CRC_TYPE, CRC_LENGTH);
//     u_int32_t crc_ =
//         BitSequenceModule::generateCRC(bitSequence_, CRC_TYPE, CRC_LENGTH);

//     BitSequence crc_bit =
//         BitSequenceModule::numberToBitSequence(crc, CRC_LENGTH);
//     BitSequence crc_bit_ =
//         BitSequenceModule::numberToBitSequence(crc_, CRC_LENGTH);

//     BitSequence gold = BitSequenceModule::generateGoldSequence();

//     size_t messageLength = bitSequence.size();
//     size_t messageLength_ = bitSequence_.size();
//     // std::cout << messageLength << "\n";
//     if ((messageLength > (1 << 16)) && (messageLength > (1 << 16))) {
//         throw std::overflow_error("Размер сообщения превышает 2^16 бит.");
//     }

//     BitSequence len(16, 0);
//     for (int i = 0; i < 16; ++i) {
//         len[15 - i] = (messageLength >> i) & 1;
//     }

//     BitSequence len_(16, 0);
//     for (int i = 0; i < 16; ++i) {
//         len_[15 - i] = (messageLength_ >> i) & 1;
//     }

//     BitSequence combinedSeq = Utilities::combineBitSequences(
//         Utilities::combineBitSequences(gold, len),
//         Utilities::combineBitSequences(bitSequence, crc_bit));
//     Signal signal = Utilities::convertToSignal(combinedSeq);
//     signal = SignalProcessing::oversample(signal, N);
//     size_t Nx = signal.size();  // Размер сообщения

//     BitSequence combinedSeq_ = Utilities::combineBitSequences(
//         Utilities::combineBitSequences(gold, len_),
//         Utilities::combineBitSequences(bitSequence_, crc_bit_));
//     Signal signal_ = Utilities::convertToSignal(combinedSeq_);
//     signal_ = SignalProcessing::oversample(signal_, N);
//     size_t Nx_ = signal_.size();  // Размер сообщения

//     // Signal signalArray(Nx * 2, 0.0f);
//     // std::copy(signal.begin(), signal.end(),
//     //           signalArray.begin() + signal.size() / 2);

//     Signal signalArray = signal;
//     Signal signalArray_ = signal_;

//     double mu = 0.0;

//     std::vector<std::pair<int, double>> sigmaErrors;
//     std::vector<std::pair<int, double>> sigmaErrors_;
//     std::vector<std::pair<int, double>> sigmaErrors__;

//     size_t goldSize = gold.size() * N;
//     // size_t lenStart = goldSize;  // len начинается сразу за GOLD
//     size_t lenStart = 0;
//     size_t lenEnd = lenStart + 16 * N + goldSize;

//     for (double sigma = 0.00; sigma <= 0.18; sigma += 0.00005) {
//         Signal noise = SignalProcessing::generateWhiteNoise(Nx_, mu, sigma);

//         for (size_t i = 0; i < signalArray.size() && i < noise.size(); i++) {
//             if (i < lenStart || i >= lenEnd) {  // Пропускаем диапазон
//                 signalArray[i] += noise[i];
//             }
//         }

//         for (size_t i = 0; i < signalArray_.size() && i < noise.size(); i++) {
//             if (i < lenStart || i >= lenEnd) {  // Пропускаем диапазон
//                 signalArray_[i] += noise[i];
//             }
//         }

//         // Прием
//         BitSequence gold_oversampled = SignalProcessing::oversample(gold, N);
//         Signal received = signalArray;
//         // 0.6 это thrust factor для поиска корреляции
//         size_t start_index = SignalAnalysis::findSequenceInSignal(
//             received, gold_oversampled, 0.6);
//         Utilities::removeRange(received, 0, start_index);
//         BitSequence interpreted =
//             SignalAnalysis::interpretSymbols(received, N, sigma);
//         // SignalAnalysis::interpretSymbols(received, N, 0.5);
//         Utilities::removeRange(interpreted, 0, gold.size());
//         BitSequence receivedLen = Utilities::getRange(interpreted, 0, 16);
//         Utilities::removeRange(interpreted, 0, 16);
//         size_t messageLengthReceive =
//             SignalAnalysis::getMessageLength(receivedLen);

//         BitSequence receivedBitMessage =
//             Utilities::getRange(interpreted, 0, messageLengthReceive);

//         // BitSequence receivedBitCRC = Utilities::getRange(
//         //     interpreted, messageLengthReceive, messageLengthReceive + 16);
//         // int32_t receivedCRC =
//         //     BitSequenceModule::bitSequenceToNumber(receivedBitCRC);
//         // u_int32_t newCrc = BitSequenceModule::generateCRC(receivedBitMessage,
//         //   CRC_TYPE, CRC_LENGTH);

//         Signal received_ = signalArray_;
//         size_t start_index_ = SignalAnalysis::findSequenceInSignal(
//             received_, gold_oversampled, 0.6);
//         Utilities::removeRange(received_, 0, start_index_);
//         BitSequence interpreted_ =
//             // SignalAnalysis::interpretSymbols(received_, N, 0.5);
//             SignalAnalysis::interpretSymbols(received_, N, sigma);
//         Utilities::removeRange(interpreted_, 0, gold.size());
//         BitSequence receivedLen_ = Utilities::getRange(interpreted_, 0, 16);
//         Utilities::removeRange(interpreted_, 0, 16);
//         size_t messageLengthReceive_ =
//             SignalAnalysis::getMessageLength(receivedLen_);

//         BitSequence receivedBitMessage_ =
//             Utilities::getRange(interpreted_, 0, messageLengthReceive_);

//         // BitSequence receivedBitCRC_ = Utilities::getRange(
//         //     interpreted_, messageLengthReceive_, messageLengthReceive_ + 16);
//         // int32_t receivedCRC_ =
//         //     BitSequenceModule::bitSequenceToNumber(receivedBitCRC_);
//         // u_int32_t newCrc_ = BitSequenceModule::generateCRC(
//         //     receivedBitMessage_, CRC_TYPE, CRC_LENGTH);
//         // if (newCrc != receivedCRC) {
//         //     // std::cout << "Received CRC incorrect: " << std::hex <<
//         //     // receivedCRC
//         //     //           << std::dec << "\n";
//         // }
//         // if (newCrc_ != receivedCRC_) {
//         //     // std::cout << "Received CRC incorrect: " << std::hex <<
//         //     // receivedCRC
//         //     //           << std::dec << "\n";
//         // }

//         // Подсчет числа ошибок
//         size_t bitErrors = 0;
//         for (size_t i = 0; i < receivedBitMessage.size(); ++i) {
//             if (i < bitSequence.size() &&
//                 receivedBitMessage[i] != bitSequence[i]) {
//                 ++bitErrors;
//             }
//         }
//         size_t bitErrors__ = 0;
//         for (size_t i = 0; i < receivedBitMessage_.size(); ++i) {
//             if (i < bitSequence.size() &&
//                 receivedBitMessage_[i] != bitSequence_[i]) {
//                 ++bitErrors__;
//             }
//         }

//         receivedBitMessage_ =
//             BitSequenceModule::decodeHamming(receivedBitMessage_);
//         // BitSequenceModule::decodeHammingWithParity(receivedBitMessage_);
//         size_t bitErrors_ = 0;
//         for (size_t i = 0; i < receivedBitMessage_.size(); ++i) {
//             if (i < bitSequence.size() &&
//                 receivedBitMessage_[i] != bitSequence[i]) {
//                 ++bitErrors_;
//             }
//         }

//         sigmaErrors.emplace_back(bitErrors, sigma);
//         sigmaErrors_.emplace_back(bitErrors_, sigma);
//         sigmaErrors__.emplace_back(bitErrors__, sigma);

//         // std::cout << "Sigma: " << sigma << " | Bit errors: " << bitErrors
//         //           << "\n";
//     }

//     Visualization::saveGraphData("./data/(1,3,1)extra_1.txt", sigmaErrors);
//     descriptions.push_back({"./data/(1,3,1)extra_1.txt", "plot", "x,y",
//                             "Message len: " + std::to_string(messageLength),
//                             "sigma", "bit err", true});
//     Visualization::saveGraphData("./data/(1,3,2)extra_2.txt", sigmaErrors_);
//     descriptions.push_back(
//         {"./data/(1,3,2)extra_2.txt", "plot", "x,y",
//          "Hamming. Message len: " + std::to_string(messageLength), "sigma",
//          "bit err", true});
//     Visualization::saveGraphData("./data/(1,3,3)extra_3.txt", sigmaErrors__);
//     descriptions.push_back(
//         {"./data/(1,3,3)extra_3.txt", "plot", "x,y",
//          "Hamming (origin). Message len: " + std::to_string(messageLength),
//          "sigma", "bit err", true});

//     return 0;
// }

// int additionalTask2(
//     std::vector<Visualization::GraphDescription>& descriptions) {
//     BitSequence bitSequence_ =
//         BitSequenceModule::encodeToASCII("Text! MORE TEXT!");
//     BitSequence bitSequence = BitSequenceModule::encodeHamming(bitSequence_);

//     constexpr size_t CRC_LENGTH = 16;
//     constexpr BitSequenceModule::CRCType CRC_TYPE =
//         BitSequenceModule::CRCType::CRC16;
//     u_int32_t crc =
//         BitSequenceModule::generateCRC(bitSequence, CRC_TYPE, CRC_LENGTH);

//     BitSequence crc_bit =
//         BitSequenceModule::numberToBitSequence(crc, CRC_LENGTH);

//     BitSequence gold = BitSequenceModule::generateGoldSequence();

//     size_t messageLength = bitSequence.size();
//     // std::cout << messageLength << "\n";
//     if (messageLength > (1 << 16)) {
//         throw std::overflow_error("Размер сообщения превышает 2^16 бит.");
//     }

//     BitSequence len(16, 0);
//     for (int i = 0; i < 16; ++i) {
//         len[15 - i] = (messageLength >> i) & 1;
//     }

//     BitSequence combinedSeq = Utilities::combineBitSequences(
//         Utilities::combineBitSequences(gold, len),
//         Utilities::combineBitSequences(bitSequence, crc_bit));
//     Signal signal = Utilities::convertToSignal(combinedSeq);
//     signal = SignalProcessing::oversample(signal, N);
//     size_t Nx = signal.size();  // Размер сообщения

//     // Signal signalArray(Nx * 2, 0.0f);
//     // std::copy(signal.begin(), signal.end(),
//     //           signalArray.begin() + signal.size() / 2);

//     Signal signalArray = signal;

//     double mu = 0.0;

//     std::vector<std::pair<int, double>> sigmaErrors;

//     size_t goldSize = gold.size() * N;
//     // size_t lenStart = goldSize;  // len начинается сразу за GOLD
//     size_t lenStart = 0;
//     size_t lenEnd = lenStart + 16 * N + goldSize;

//     for (double sigma = 0.0; sigma <= 5.0; sigma += 0.01) {
//         Signal noise = SignalProcessing::generateWhiteNoise(Nx * 2, mu,
//         sigma);

//         for (size_t i = 0; i < signalArray.size() && i < noise.size(); i++) {
//             if (i < lenStart || i >= lenEnd) {  // Пропускаем диапазон
//                 signalArray[i] += noise[i];
//             }
//         }

//         // Прием
//         BitSequence gold_oversampled = SignalProcessing::oversample(gold, N);
//         Signal received = signalArray;
//         // 0.6 это thrust factor для поиска корреляции
//         size_t start_index = SignalAnalysis::findSequenceInSignal(
//             received, gold_oversampled, 0.6);
//         Utilities::removeRange(received, 0, start_index);
//         BitSequence interpreted =
//             SignalAnalysis::interpretSymbols(received, N, 0.5);
//         Utilities::removeRange(interpreted, 0, gold.size());
//         BitSequence receivedLen = Utilities::getRange(interpreted, 0, 16);
//         Utilities::removeRange(interpreted, 0, 16);
//         size_t messageLengthReceive =
//             SignalAnalysis::getMessageLength(receivedLen);

//         BitSequence receivedBitMessage =
//             Utilities::getRange(interpreted, 0, messageLengthReceive);

//         BitSequence receivedBitCRC = Utilities::getRange(
//             interpreted, messageLengthReceive, messageLengthReceive + 16);
//         int32_t receivedCRC =
//             BitSequenceModule::bitSequenceToNumber(receivedBitCRC);
//         u_int32_t newCrc = BitSequenceModule::generateCRC(receivedBitMessage,
//                                                           CRC_TYPE,
//                                                           CRC_LENGTH);
//         receivedBitMessage =
//             BitSequenceModule::decodeHamming(receivedBitMessage);

//         if (newCrc != receivedCRC) {
//             // std::cout << "Received CRC incorrect: " << std::hex <<
//             // receivedCRC
//             //           << std::dec << "\n";
//         }

//         // Подсчет числа ошибок
//         size_t bitErrors = 0;
//         for (size_t i = 0; i < receivedBitMessage.size(); ++i) {
//             if (i < bitSequence_.size() &&
//                 receivedBitMessage[i] != bitSequence_[i]) {
//                 ++bitErrors;
//             }
//         }

//         sigmaErrors.emplace_back(bitErrors, sigma);

//         // std::cout << "Sigma: " << sigma << " | Bit errors: " << bitErrors
//         //           << "\n";
//     }

//     Visualization::saveGraphData("./data/(1,2,2)extra_2.txt", sigmaErrors);
//     descriptions.push_back(
//         {"./data/(1,2,2)extra_2.txt", "plot", "x,y",
//          "Hamming. Message len: " + std::to_string(messageLength), "sigma",
//          "bit err", true});

//     return 0;
// }

int main() {
    try {
        std::vector<Visualization::GraphDescription> descriptions = {};
        std::vector<Visualization::GraphDescription> descriptions_13 = {};
#ifdef MAIN
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

        for (size_t i = 0; (i < signalArray.size()) && (i < noise.size());
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
            received, gold_oversampled, 0.6);
        std::cout << "start_index: " << start_index << "\n";

        Utilities::removeRange(received, 0, start_index);

        Visualization::saveGraphData("./data/task8_1.txt", received);
        descriptions.push_back({"./data/task8_1.txt", "plot", "x,y",
                                "received - start noise", "Index", "Value",
                                false});

        // 9. downsample + resolve
        BitSequence interpreted =
            SignalAnalysis::interpretSymbols(received, N, sigma);

        Visualization::saveGraphData("./data/task9.txt", interpreted);
        descriptions.push_back({"./data/task9.txt", "plot", "x,y",
                                "interpreted signal", "Index", "Value", false});

        // 10. -sync
        Utilities::removeRange(interpreted, 0, gold.size());

        // 10,1. get len (16 бит на кодирование длины сообщения)
        BitSequence receivedLen = Utilities::getRange(interpreted, 0, 16);
        Utilities::removeRange(interpreted, 0, 16);
        size_t messageLengthReceive =
            SignalAnalysis::getMessageLength(receivedLen);
        // std::cout << messageLengthReceive << "\n";

        // 10,2. get message
        BitSequence receivedBitMessage =
            Utilities::getRange(interpreted, 0, messageLengthReceive);
        Visualization::printBitSequence(receivedBitMessage,
                                        "Принятое сообщение: ");

        // 10,3. get CRC from message
        BitSequence receivedBitCRC = Utilities::getRange(
            interpreted, messageLengthReceive, messageLengthReceive + 16);
        int32_t receivedCRC =
            BitSequenceModule::bitSequenceToNumber(receivedBitCRC);

        // 11. calc new CRC
        u_int32_t newCrc = BitSequenceModule::generateCRC(receivedBitMessage,
                                                          CRC_TYPE, CRC_LENGTH);
        if (newCrc == receivedCRC) {
            // 12. decode
            std::string receivedMessage =
                BitSequenceModule::decodeFromASCII(receivedBitMessage);
            std::cout << "Received message without errors :\n"
                      << receivedMessage << "\n";
        } else {
            std::cout << "Received CRC incorrect: " << std::hex << receivedCRC
                      << std::dec << "\n";
        }

        size_t bitErrors = 0;
        for (size_t i = 0; i < receivedBitMessage.size(); ++i) {
            if (i < bitSequence.size() &&
                receivedBitMessage[i] != bitSequence[i]) {
                ++bitErrors;
            }
        }
        std::cout << "errors :\n" << bitErrors << "\n";
        // 13. change
        BitSequence bitForChange = combinedSeq;
        Signal signalForChange = Utilities::convertToSignal(bitForChange);

        // TODO: Перенастроить линтер ! Это ужос !
        Signal shortSignal =
            SignalProcessing::oversample(signalForChange, N / 2);
        Signal mediumSignal = SignalProcessing::oversample(signalForChange, N);
        Signal largeSignal =
            SignalProcessing::oversample(signalForChange, N * 40);

        Signal shortSignalArray(shortSignal.size() * 2, 0.0f);
        Signal mediumSignalArray(mediumSignal.size() * 2, 0.0f);
        Signal largeSignalArray(largeSignal.size() * 2, 0.0f);

        Utilities::insertVector(shortSignalArray, shortSignal, 0);
        Utilities::insertVector(mediumSignalArray, mediumSignal, 0);
        Utilities::insertVector(largeSignalArray, largeSignal, 0);

        Visualization::saveGraphData("./data/13/(1,3,1)task13_1_1.txt",
                                     shortSignalArray);
        descriptions_13.push_back({"./data/13/(1,3,1)task13_1_1.txt", "plot",
                                   "x,y", "shortSignal", "Index", "Value",
                                   false});
        Visualization::saveGraphData("./data/13/(1,3,2)task13_1_2.txt",
                                     mediumSignalArray);
        descriptions_13.push_back({"./data/13/(1,3,2)task13_1_2.txt", "plot",
                                   "x,y", "mediumSignal", "Index", "Value",
                                   false});
        Visualization::saveGraphData("./data/13/(1,3,3)task13_1_3.txt",
                                     largeSignalArray);
        descriptions_13.push_back({"./data/13/(1,3,3)task13_1_3.txt", "plot",
                                   "x,y", "largeSignal", "Index", "Value",
                                   false});

        noise = SignalProcessing::generateWhiteNoise(largeSignalArray.size(),
                                                     mu, sigma);

        for (size_t i = 0; (i < shortSignalArray.size()) && (i < noise.size());
             i++) {
            shortSignalArray[i] += noise[i];
        }
        for (size_t i = 0; (i < mediumSignalArray.size()) && (i < noise.size());
             i++) {
            mediumSignalArray[i] += noise[i];
        }
        for (size_t i = 0; (i < largeSignalArray.size()) && (i < noise.size());
             i++) {
            largeSignalArray[i] += noise[i];
        }

        Visualization::saveGraphData("./data/13/(2,3,1)task13_2_1.txt",
                                     shortSignalArray);
        descriptions_13.push_back({"./data/13/(2,3,1)task13_2_1.txt", "plot",
                                   "x,y", "shortSignal", "Index", "Value",
                                   false});
        Visualization::saveGraphData("./data/13/(2,3,2)task13_2_2.txt",
                                     mediumSignalArray);
        descriptions_13.push_back({"./data/13/(2,3,2)task13_2_2.txt", "plot",
                                   "x,y", "mediumSignal", "Index", "Value",
                                   false});
        Visualization::saveGraphData("./data/13/(2,3,3)task13_2_3.txt",
                                     largeSignalArray);
        descriptions_13.push_back({"./data/13/(2,3,3)task13_2_3.txt", "plot",
                                   "x,y", "largeSignal", "Index", "Value",
                                   false});
#else
        // Дополнительное задание
        additionalTask(descriptions);
        // additionalTask2(descriptions);
#endif
        // Сохранение описаний для графиков
        Visualization::saveGraphDescription("./data/descriptions.txt",
                                            descriptions);
        Visualization::saveGraphDescription("./data/13/descriptions.txt",
                                            descriptions_13);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
