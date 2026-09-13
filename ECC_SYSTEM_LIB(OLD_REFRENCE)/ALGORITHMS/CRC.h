#pragma once
#include<stdint.h>
#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include"SimpleDataStructs.h"
#include"ECCENV.h"

/**
 * CRC-16-IBM (also known as CRC-16-ANSI)
 * Generator Polynomial: x^16 + x^15 + x^2 + 1 (0x8005)
 */

#define POLYNOMIAL 0x8005 
uint16_t crc16(const uint8_t *data, size_t length);
