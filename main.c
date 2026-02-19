#include<stdio.h>
#include <stdint.h>
#include"ECCENV.h"
#include"CRC.h"
#include"ErrorCheckingCode.h"
#include<string.h>
typedef struct
{
	long _SomeSortOfBIG_LONG;
	int _SomeSortOfValue;
	char _SomeSortOfString[20];
}MockSensorData;
int main()
{
	VALIDATOR_initValidator();
	rs_init();
	MockSensorData mockSensorData;
	mockSensorData._SomeSortOfBIG_LONG = 0xdeadbeef;
	mockSensorData._SomeSortOfValue = 35;
	char myString[20] = "I am a String\0";
	memcpy(mockSensorData._SomeSortOfString, myString, strlen(myString));
	

	
	ECC_save(&mockSensorData, sizeof(mockSensorData),REEDSOLOMON);
	uint8_t data = 8;
	ECC_save(&data,(uint64_t)(sizeof(data)),CRC16);
	uint64_t data2 = 98;
	ECC_save(&data2,(uint64_t)(sizeof(data2)),FLETCHER16);
	DEBUG_PRINT_MEMORY();

	((uint8_t*)&mockSensorData)[0] ^= 0xFF; 

    if (!ECC_validate(&mockSensorData, sizeof(mockSensorData), REEDSOLOMON)) {
        printf("Corruption detected.\n");
        if (ECC_fix(&mockSensorData, sizeof(mockSensorData), REEDSOLOMON)) {
            printf("Fixed.\n");
        }
    }
	if(ECC_validate(&mockSensorData, sizeof(mockSensorData),REEDSOLOMON)){printf("\nREED-SOLOMON data is valid.\n");}
	//Checks:
	if(ECC_validate(&data2,sizeof(data2),FLETCHER16)){printf("\nfletcher16 is valid.\n");}
	if(ECC_validate(&data,sizeof(data),CRC16)){printf("\ncrc16 valid.\n");}
	if (ECC_validate(&mockSensorData, sizeof(mockSensorData),FLETCHER16)){printf("\nSensor data is valid\n");}
}
