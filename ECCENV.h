#pragma once
#include<stdint.h>
#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include"SimpleDataStructs.h"
typedef enum
{
    CRC16 = 0, //slow (detection only)
    FLETCHER16 = 1,//fast (detection only)
    REEDSOLOMON = 2//reliable (fixing too)
}ECC_types;
typedef struct
{
    union {
        uint16_t validator_code;    // For CRC16 / Fletcher
        uint8_t rs_parity[10];      // For Reed-Solomon (fixes up to 5 bytes)
    };
    uint64_t position_in_memory;
    ECC_types algo_type;
}ECC_DATA_STRUCT;

extern struct LinkedList* _heapHead;
void VALIDATOR_initValidator();
void DEBUG_PRINT_MEMORY();