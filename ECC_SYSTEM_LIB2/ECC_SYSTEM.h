#pragma once
#include"ECC_ENV.h"



void ECC_SAVE(void* data, uint64_t sizeData, ECC_TYPE algo);
ECC_RESULT ECC_VALIDATE(void* data, uint64_t size, ECC_TYPE algo);

ECC_RESULT ECC_fix(void* data, uint64_t size, ECC_TYPE algo);