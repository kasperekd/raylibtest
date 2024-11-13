#include <nmmintrin.h>  // sse4.1 для CRC32
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define DATA_BITS 250
#define POLY 0xBD  // polynomial (0xBD = 10111101)
#define POLY_BITS 8

uint8_t calculate_crc(uint8_t data[], int length, uint8_t poly) {
    uint8_t crc = 0;
    for (int i = 0; i < length; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ poly;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

uint32_t calculate_crc32_hardware(uint8_t data[], int length) {
    uint32_t crc = 0xFFFFFFFF;  // Начальное значение
    for (int i = 0; i < length; i++) {
        crc = _mm_crc32_u8(crc, data[i]);
    }
    return crc ^ 0xFFFFFFFF;  // Инверсия
}

bool check_crc(uint8_t data[], int length, uint8_t poly) {
    return calculate_crc(data, length, poly) == 0;
}

void flip_bit(uint8_t *data, int bit_pos) {
    int byte_pos = bit_pos / 8;
    int bit_offset = bit_pos % 8;
    data[byte_pos] ^= (1 << (7 - bit_offset));  // Инвертирование бита
}

void print_bits(const uint8_t *data, size_t length) {
    for (size_t i = 0; i < length; i++) {
        for (int j = 7; j >= 0; j--) {
            printf("%d", (data[i] >> j) & 1);
        }
    }
}

int main() {
    uint8_t data[DATA_BITS / 8] = {0};
    for (int i = 0; i < DATA_BITS / 8; i++) {
        data[i] = i % 2 ? 0xFF : 0x00;
    }

    print_bits(data, sizeof(data));
    printf("\n");

    uint8_t crc8 = calculate_crc(data, DATA_BITS / 8, POLY);
    printf("Calculated CRC-8: 0x%02X\n", crc8);

    uint32_t crc32_hw = calculate_crc32_hardware(data, DATA_BITS / 8);
    printf("Calculated CRC-32 (hardware): 0x%08X\n", crc32_hw);

    uint8_t transmitted_data[(DATA_BITS / 8) + 1];
    for (int i = 0; i < DATA_BITS / 8; i++) {
        transmitted_data[i] = data[i];
    }
    transmitted_data[DATA_BITS / 8] = crc8;

    if (check_crc(transmitted_data, (DATA_BITS / 8) + 1, POLY)) {
        printf("Transmitted packet accepted without errors.\n");
    } else {
        printf("Error in the transmitted packet!\n");
    }

    print_bits(transmitted_data, sizeof(transmitted_data));
    printf("\n");

    int detected_errors = 0, undetected_errors = 0;
    // for (int bit = 0; bit < DATA_BITS + POLY_BITS - 2; bit++) {
    for (int bit = 0; bit < DATA_BITS; bit++) {
        flip_bit(transmitted_data, bit);

        if (check_crc(transmitted_data, (DATA_BITS / 8) + 1, POLY)) {
            undetected_errors++;
        } else {
            detected_errors++;
        }

        flip_bit(transmitted_data, bit);
    }

    printf("Detected errors: %d\n", detected_errors);
    printf("Undetected errors: %d\n", undetected_errors);

    return 0;
}
