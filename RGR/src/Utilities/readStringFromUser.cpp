#include <iostream>
#include <string>

#include "Utilities.h"

namespace Utilities {
std::string readStringFromUser() {
    std::string input;
    std::getline(std::cin, input);
    return input;
}
}  // namespace Utilities
