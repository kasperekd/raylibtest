#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define N 5
#define LENGTH ((1 << N) - 1)  // Длина последовательности Голда, 2^N - 1

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

double autocorrelation(uint8_t seq1[], uint8_t seq2[]) {
    int sum = 0;
    for (int i = 0; i < LENGTH; i++) {
        sum += (seq1[i] == seq2[i]) ? 1 : -1;
    }
    return (double)sum / LENGTH;
}

double cross_correlation(uint8_t seq1[], uint8_t seq2[]) {
    int sum = 0;
    for (int i = 0; i < LENGTH; i++) {
        sum += (seq1[i] == seq2[i]) ? 1 : -1;
    }
    return (double)sum / LENGTH;
}

int main() {
    uint8_t x_start = 0b01101;  // x = 13 в двоичной системе
    uint8_t y_start = 0b10100;  // y = x + 7 в двоичной системе

    uint8_t x_seq[LENGTH];
    uint8_t y_seq[LENGTH];
    lfsr_sequence(x_seq, x_start);  // полином x
    lfsr_sequence(y_seq, y_start);  // полином y

    uint8_t gold_seq[LENGTH];
    // uint8_t gold_seq[LENGTH] = {1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1,
    // 0,
    //                             1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1};
    gold_sequence(x_seq, y_seq, gold_seq);

    printf("Gold sequence:\n");
    for (int i = 0; i < LENGTH; i++) {
        printf("%d ", gold_seq[i]);
    }
    printf("\n\n");

    for (int shift = 0; shift < LENGTH; shift++) {
        uint8_t shifted_seq[LENGTH];
        cyclic_shift(gold_seq, shifted_seq, shift);

        printf("%2d | ", shift);
        for (int i = 0; i < LENGTH; i++) {
            printf("%d | ", shifted_seq[i]);
        }

        // автокорреляция для сдвинутой последовательности
        double autocorr_value = autocorrelation(gold_seq, shifted_seq);
        printf("% .3f\n", autocorr_value);
    }

    // Создаем новую последовательность Голда: x = x + 1, y = y - 5
    uint8_t x_new_start = x_start + 1;
    uint8_t y_new_start = y_start - 5;
    uint8_t new_x_seq[LENGTH];
    uint8_t new_y_seq[LENGTH];
    lfsr_sequence(new_x_seq, x_new_start);
    lfsr_sequence(new_y_seq, y_new_start);

    uint8_t new_gold_seq[LENGTH];
    gold_sequence(new_x_seq, new_y_seq, new_gold_seq);
    printf("\nNew Gold sequence:\n");
    for (int i = 0; i < LENGTH; i++) {
        printf("%d ", new_gold_seq[i]);
    }

    // взаимная корреляция исходной и новой последовательностей
    double cross_corr_value = cross_correlation(gold_seq, new_gold_seq);
    printf("\nCross-correlation of the original and new sequences:%.3f\n",
           cross_corr_value);

    return 0;
}
