#include "BitSequence.h"

namespace BitSequenceModule {
static const int N = 5;
static const int LENGTH =
    ((1 << N) - 1);  // Длина последовательности Голда, 2^N - 1

// сдвиг LFSR
static inline int lfsr_shift(uint8_t reg[], const int taps[], int num_taps) {
    int feedback = reg[taps[0]];
    for (int i = 1; i < num_taps; i++) {
        feedback ^= reg[taps[i]];
    }
    int output = reg[N - 1];

    memmove(&reg[1], &reg[0], N - 1);
    reg[0] = feedback;

    return output;
}

void lfsr_sequence(uint8_t seq[], uint8_t start) {
    uint8_t state[N] = {0};
    for (int i = 0; i < N; i++) {
        state[i] = (start >> i) & 1;
    }

    int taps[] = {0, 2};
    int num_taps = sizeof(taps) / sizeof(taps[0]);

    for (int i = 0; i < LENGTH; i++) {
        seq[i] = lfsr_shift(state, taps, num_taps);
    }
}

void gold_sequence(const uint8_t x_seq[], const uint8_t y_seq[],
                   uint8_t gold_seq[]) {
    int taps_x[] = {0, 2, 1};
    int taps_y[] = {0, 3, 2, 4};
    int num_taps_x = sizeof(taps_x) / sizeof(taps_x[0]);
    int num_taps_y = sizeof(taps_y) / sizeof(taps_y[0]);

    uint8_t x[N], y[N];
    memcpy(x, x_seq, N);
    memcpy(y, y_seq, N);

    for (int i = 0; i < LENGTH; ++i) {
        int bit_x = lfsr_shift(x, taps_x, num_taps_x);
        int bit_y = lfsr_shift(y, taps_y, num_taps_y);

        gold_seq[i] = bit_x ^ bit_y;
    }
}

void cyclic_shift(uint8_t seq[], uint8_t shifted_seq[], int shift) {
    for (int i = 0; i < LENGTH; i++) {
        shifted_seq[i] = seq[(i + shift) % LENGTH];
    }
}

BitSequence generateGoldSequence(uint8_t x_start, uint8_t y_start) {
    uint8_t x_seq[LENGTH];
    uint8_t y_seq[LENGTH];

    lfsr_sequence(x_seq, x_start);
    lfsr_sequence(y_seq, y_start);

    BitSequence gold_seq(LENGTH);

    uint8_t gold_seq_temp[LENGTH];
    gold_sequence(x_seq, y_seq, gold_seq_temp);

    for (int i = 0; i < LENGTH; i++) {
        gold_seq[i] = static_cast<int>(gold_seq_temp[i]);
    }

    return gold_seq;
}
}  // namespace BitSequenceModule