#pragma once
#include"CRC.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include"ECCENV.h"
#include"SimpleDataStructs.h"
#include"FLETCHER.h"


void ECC_save(void* data, uint64_t sizeData, ECC_types algo);
uint8_t ECC_validate(void* data, uint64_t size, ECC_types algo);
