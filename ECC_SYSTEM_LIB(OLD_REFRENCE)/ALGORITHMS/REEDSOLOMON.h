#ifndef REED_SOLOMON_H
#define REED_SOLOMON_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Initializes the Galois Field lookup tables. 
 * Must be called once before any encoding/decoding.
 */
void rs_init(void);

/**
 * @brief Generates parity bytes for a given data block.
 * @param data Pointer to the struct or buffer.
 * @param data_len Size of data in bytes.
 * @param parity Buffer to store generated ECC bytes.
 * @param ecc_len Number of parity bytes to generate.
 */
void rs_encode(void* data, size_t data_len, uint8_t* parity, size_t ecc_len);

/**
 * @brief Detects and repairs errors in-place.
 * @param data Pointer to the data to be checked/fixed.
 * @param data_len Size of data in bytes.
 * @param parity Pointer to the stored ECC bytes.
 * @param ecc_len Number of parity bytes used.
 * @return 0 if clean, 1 if repaired, -1 if unrepairable.
 */
int rs_decode(void* data, size_t data_len, uint8_t* parity, size_t ecc_len);

#endif