#include "SignalProcessing.h"

namespace SignalProcessing {
Signal generateWhiteNoise(size_t size, float mu, float sigma) {
    Signal noise(size, 0.0f);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> dist(mu, sigma);

    for (size_t i = 0; i < noise.size(); ++i) {
        noise[i] = dist(gen);
    }

    return noise;
}
}  // namespace SignalProcessing