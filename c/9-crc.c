#include <stdio.h>
#include <string.h>

#define CRC_POLYNOMIAL 0xEDB88320L
#define CRC_SEED 0xFFFFFFFFL

// Function to calculate CRC checksum
unsigned long calculateCRC(unsigned char *data, int length) {
    unsigned long crc = CRC_SEED;
    int i, j;

    for (i = 0; i < length; ++i) {
        crc ^= data[i];
        for (j = 0; j < 8; j++) {
            if (crc & 1)
                crc = (crc >> 1) ^ CRC_POLYNOMIAL;
            else
                crc >>= 1;
        }
    }

    return crc;
}

// Function to verify CRC checksum
int verifyCRC(unsigned char *data, int length, unsigned long checksum) {
    unsigned long crc = calculateCRC(data, length);
    return (crc == checksum);
}

int main() {
    unsigned char data[] = "Hello, world!";
    int data_length = strlen((char*)data);
    unsigned long checksum;

    // Calculate CRC checksum
    checksum = calculateCRC(data, data_length);
    printf("CRC Checksum: 0x%lX\n", checksum);

    // Verify CRC checksum
    if (verifyCRC(data, data_length, checksum)) {
        printf("CRC Checksum is correct.\n");
    } else {
        printf("CRC Checksum is incorrect.\n");
    }

    return 0;
}

