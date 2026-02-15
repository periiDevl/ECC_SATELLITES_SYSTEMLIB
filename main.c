#include<stdio.h>
#include <stdint.h>
#include"ECCENV.h"
#include"CRC.h"
#include"ErrorCheckingCode.h"
int main()
{
	uint8_t data = 8;
	VALIDATOR_initValidator();
	ECC_save(&data,(uint64_t)(sizeof(data)),CRC16);
	uint64_t data2 = 98;
	ECC_save(&data,(uint64_t)(sizeof(data)),FLETCHER16);
	DEBUG_PRINT_MEMORY();
	if(ECC_validate(&data,sizeof(data),FLETCHER16)){printf("\nfletcher16 is working.\n");}
	if(ECC_validate(&data,sizeof(data),CRC16)){printf("\ncrc16 working.\n");}
	
}

