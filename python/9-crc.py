CRC_POLYNOMIAL = 0xEDB88320
CRC_SEED = 0xFFFFFFFF

# Function to calculate CRC checksum
def calculate_crc(data):
    crc = CRC_SEED
    for byte in data:
        crc ^= byte
        for _ in range(8):
            if crc & 1:
                crc = (crc >> 1) ^ CRC_POLYNOMIAL
            else:
                crc >>= 1
    return crc

# Function to verify CRC checksum
def verify_crc(data, checksum):
    crc = calculate_crc(data)
    return crc == checksum

if __name__ == "__main__":
    data = b"Hello, world!"
    checksum = calculate_crc(data)
    print(f"CRC Checksum: 0x{checksum:x}")

    if verify_crc(data, checksum):
        print("CRC Checksum is correct.")
    else:
        print("CRC Checksum is incorrect.")

