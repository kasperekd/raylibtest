#include "SignalProcessing.h"

namespace SignalProcessing {
Signal extractEveryNthValue(const Signal& inputSignal, size_t N) {
    Signal outputSignal;
    for (size_t i = 0; i < inputSignal.size(); i += N) {
        outputSignal.push_back(inputSignal[i]);
    }
    return outputSignal;
}
}  // namespace SignalProcessing