#include "Utilities.h"

namespace Utilities {
std::string readStringFromUser() {
    std::string input;
    std::getline(std::cin, input);
    return input;
}
size_t readSize_tFromUser(size_t min, size_t max) {
    size_t value;
    while (true) {
        // std::cout << "Введите значение size_t (от " << min << " до " << max
        //           << "): ";
        std::string input = readStringFromUser();
        std::stringstream ss(input);
        if (ss >> value && value >= min && value <= max) {
            return value;
        } else {
            std::cout << "Ошибка: введите корректное значение." << std::endl;
        }
    }
}

double readDoubleFromUser(double min, double max) {
    double value;
    while (true) {
        // std::cout << "Введите значение double (от " << min << " до " << max
        //           << "): ";
        std::string input = readStringFromUser();
        std::stringstream ss(input);
        if (ss >> value && value >= min && value <= max) {
            return value;
        } else {
            std::cout << "Ошибка: введите корректное значение." << std::endl;
        }
    }
}
}  // namespace Utilities
