#include"ECCENV.h"
#include"CRC.h"
struct LinkedList* _heapHead = NULL;
void VALIDATOR_initValidator()
{
	_heapHead = malloc(sizeof(struct LinkedList));
	_heapHead->data = malloc(sizeof(ECC_DATA_STRUCT));
	ECC_DATA_STRUCT* actual_data = (ECC_DATA_STRUCT*)(_heapHead->data);
	//save a way to know its the head:
	actual_data->validator_code = 255;
	actual_data->position_in_memory = 0;
	_heapHead->next = NULL;
	//check its working
	printf("(needs to be 255) data = %d\n",actual_data->validator_code);
}


void DEBUG_PRINT_MEMORY()
{
	printf("[DEBUG] Starting to read...\n");
	struct LinkedList* _start = _heapHead;
	ECC_DATA_STRUCT* stored_data = (ECC_DATA_STRUCT*)(_start->data);
	if(stored_data == NULL){printf("no memory saved yet..\n");}
	uint64_t counter=1;
	printf("clearing space...");
	printf("\n\n\n\n\n\n\n\n\n");
	while(_start != NULL)
	{
		printf("[ECC memory block no.%ld]\n",counter);
		printf("------------------------\n");
		stored_data = (ECC_DATA_STRUCT*)(_start->data);
		printf("|valid_code : %d      |\n", stored_data->validator_code);
		printf("|position_in_memory : %d|\n",stored_data->position_in_memory);
		printf("|type : ");
		if (stored_data->algo_type == CRC16){printf("CRC16");}else if(stored_data->algo_type == FLETCHER16){printf("FLETCHER16");}else if(stored_data->algo_type == REEDSOLOMON){printf("REED_SOLOMON");}
		printf("\n");
		printf("-----------------------\n\n");
		_start = _start->next;
		counter++;
	}
	printf("ended.\n\n");
}