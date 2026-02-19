#pragma once
#include"CRC.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include"ECCENV.h"
#include"SimpleDataStructs.h"
#include"FLETCHER.h"
#include"REEDSOLOMON.h"
#include<string.h>
//FOR FLETCHER16, CRC16, REEDSOLOMON
void ECC_save(void* data, uint64_t sizeData, ECC_types algo);
//FOR FLETCHER16, CRC16, REEDSOLOMON
uint8_t ECC_validate(void* data, uint64_t size, ECC_types algo);
uint8_t ECC_fix(void* data, uint64_t size, ECC_types algo);

//WORKFLOW FOR REED-SOLOMON
//ECC_VALIDATE -> (ECC_FIX -> ECC_VALIDATE)
