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
    struct LinkedList* _start = _heapHead;
    uint64_t counter = 1;

    printf("\n[DEBUG] MEMORY DUMP START\n");

    while(_start != NULL)
    {
        ECC_DATA_STRUCT* stored_data = (ECC_DATA_STRUCT*)(_start->data);
        
        const char* algo = "UNKNOWN";
        if (stored_data->algo_type == CRC16) algo = "CRC16";
        else if (stored_data->algo_type == FLETCHER16) algo = "FLETCHER16";
        else if (stored_data->algo_type == REEDSOLOMON) algo = "REED_SOLOMON";

        printf("+---------------------------------+\n");
        printf("| ECC Block No. %-17ld |\n", counter);
        printf("+---------------------------------+\n");
        printf("| validator_code : %-6x%-7d  |\n", stored_data->validator_code,stored_data->validator_code);
        printf("| memory_pos     : %-14ld |\n", stored_data->position_in_memory);
        printf("| algo_type      : %-14s |\n", algo);
        printf("+---------------------------------+\n\n");

        _start = _start->next;
        counter++;
    }
    printf("[DEBUG] MEMORY DUMP END\n\n");
}