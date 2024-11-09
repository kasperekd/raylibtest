#include <stdint.h>
#include <stdio.h>

#define POLY 0xBD  // полином G = 10111101 (в двоичном виде)
#define DATA_LEN 250

uint8_t calculate_crc(uint8_t *data, int length) {
    uint8_t crc = 0;
    for (int i = 0; i < length; i++) {
        crc ^= (data[i] << (8 - 1));
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ POLY;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

// Функция для искажения бита в данных
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
    uint8_t data[DATA_LEN / 8 + 1] = {0};  // Инициализация массива данных

    // Заполнение данных для теста
    for (int i = 0; i < DATA_LEN / 8; i++) {
        data[i] = i % 2 ? 0xFF : 0x00;
    }
    print_bits(data, sizeof(data));
    printf("\n");

    int errors_detected = 0;
    int errors_undetected = 0;

    uint8_t crc = calculate_crc(data, DATA_LEN / 8);
    data[DATA_LEN / 8] = crc;  // CRC в конец
    print_bits(data, sizeof(data));
    printf("\n");
    printf("CRC for original package: 0x%X\n", crc);

    uint8_t received_crc = calculate_crc(data, DATA_LEN / 8 + 1);
    if (received_crc == 0) {
        printf(
            "No errors were detected in the received packet without "
            "distortion.\n");
    } else {
        printf("Error in received packet without distortion.\n");
    }

    for (int i = 0; i < DATA_LEN + 8; i++) {
        flip_bit(data, i);  // Искажение

        received_crc = calculate_crc(data, DATA_LEN / 8 + 1);
        if (received_crc != 0) {
            errors_detected++;
        } else {
            errors_undetected++;
            printf("Undetected errors: %d\n0x%X\n", i, received_crc);
        }

        // flip_bit(data, i);  // Восстановление
    }
    // print_bits(data, sizeof(data));
    // printf("\n");
    printf("Errors found: %d\n", errors_detected);
    printf("Undetected errors: %d\n", errors_undetected);

    return 0;
}
