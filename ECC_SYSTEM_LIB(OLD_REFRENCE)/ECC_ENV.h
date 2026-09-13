#pragma once
#include<stdint.h>
#include<stdio.h>
#include<stdint.h>
#include<string.h>

typedef enum
{
    RAW,
    CRC16,
    FLETCHER16,
    REEDSOLOMON
}ECC_TYPE;
typedef enum {
    ECC_OK_VALID = 1,
    ECC_INVALID = 0,
    ECC_IMPROPER_USE_OF_ALGORITHM = -1,
    ECC_INVALID_TANK = -2,


} ECC_RESULT;

typedef struct
{
    union {
        uint16_t validator_code;    // For CRC16 / Fletcher
        uint8_t rs_parity[10];      // For Reed-Solomon (fixes up to 5 bytes)
    };
    struct ECC_NODE* node
    ECC_types algo_type;
    uint8_t tank;
    ECC_DATA_STRUCT* next; 
}ECC_DATA_STRUCT;
