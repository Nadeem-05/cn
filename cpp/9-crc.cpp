#include <iostream>
#include <string>
#include <cstdint>

using namespace std;

constexpr uint32_t CRC_POLYNOMIAL = 0xEDB88320L;
constexpr uint32_t CRC_SEED = 0xFFFFFFFFL;

// Function to calculate CRC checksum
uint32_t calculateCRC(const string& data) {
    uint32_t crc = CRC_SEED;
    
    for (char byte : data) {
        crc ^= byte;
        for (int j = 0; j < 8; j++) {
            if (crc & 1)
                crc = (crc >> 1) ^ CRC_POLYNOMIAL;
            else
                crc >>= 1;
        }
    }

    return crc;
}

// Function to verify CRC checksum
bool verifyCRC(const string& data, uint32_t checksum) {
    uint32_t crc = calculateCRC(data);
    return (crc == checksum);
}

int main() {
    string data = "Hello, world!";
    uint32_t checksum;

    // Calculate CRC checksum
    checksum = calculateCRC(data);
    cout << "CRC Checksum: 0x" << hex << checksum << endl;

    // Verify CRC checksum
    if (verifyCRC(data, checksum)) {
        cout << "CRC Checksum is correct." << endl;
    } else {
        cout << "CRC Checksum is incorrect." << endl;
    }

    return 0;
}
