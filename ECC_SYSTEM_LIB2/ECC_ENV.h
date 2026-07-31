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
    ECC_IMPROPER_USE_OF_ALGORITHM = -1
} ECC_RESULT;
struct ECC_NODE
{
    union {
        uint16_t validator_code;    // For CRC16 / Fletcher
        uint8_t rs_parity[10];      // For Reed-Solomon (fixes up to 5 bytes)
    };
    uint8_t tank; // just to be extra safe, make sure there is enougth nodes
    //if > 2 then dominant check else just equal check
    //if the tank doesnt match the currect elemnt then we return false
    //example we have 3 nodes
    //node(3) -> node(2) -> node(1) if the number isnt dec then we have an issue
    void* next;  //next node
};

typedef struct
{
    struct ECC_NODE* node;
    ECC_types algo_type;
}ECC_DATA_STRUCT;
