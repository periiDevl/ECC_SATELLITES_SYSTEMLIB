#include"CRC.h"
uint16_t crc16(const uint8_t *data, size_t length) {
    // Initial value is usually 0xFFFF or 0x0000 depending on the protocol.
    // 0xFFFF helps detect leading zeros that might otherwise be ignored.
    uint16_t crc = 0xFFFF; 

    // Loop through each byte in the input buffer
    for (size_t i = 0; i < length; i++) {
        
        // Move the current data byte into the high byte of the 16-bit CRC register
        // and XOR it with the current CRC state.
        crc ^= (uint16_t)data[i] << 8;

        // Process each of the 8 bits in the current byte
        for (uint8_t j = 0; j < 8; j++) {
            
            // Check the Most Significant Bit (MSB). 
            // 0x8000 is 1000 0000 0000 0000 in binary.
            if (crc & 0x8000) {
                // If the MSB is 1, shift left and XOR with the polynomial.
                // This is the "division" step in polynomial arithmetic.
                crc = (crc << 1) ^ POLYNOMIAL;
            } else {
                // If the MSB is 0, just shift left.
                // The polynomial isn't applied because the "divisor" doesn't fit.
                crc <<= 1;
            }
        }
    }

    // Return the final calculated 16-bit remainder (the CRC)
    return crc;
}
