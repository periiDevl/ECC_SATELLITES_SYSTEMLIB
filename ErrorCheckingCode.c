#include"ErrorCheckingCode.h"

uint8_t ECC_validate(void* data, uint64_t size, ECC_types algo)
{
	//first lets define the crc16 block to track
	//the next step is having 2 diffrent memory blocks
	ECC_DATA_STRUCT* _crc16ValidatingMemoryBlock;
	//somehow need to track who is who using pos in mem but for now I will just compare
	struct LinkedList* head = _heapHead;
	uint16_t this_memory_code = 0x41;//CHANGE TO UNDEFIND
    if(algo == CRC16){
        this_memory_code=crc16(data, size);
    }
    else if(algo == FLETCHER16)
    {
        this_memory_code=fletcher16(data, size);
    }
	while (head != NULL)
	{
		_crc16ValidatingMemoryBlock = (ECC_DATA_STRUCT*)(head->data);
		if (_crc16ValidatingMemoryBlock->validator_code == this_memory_code)
		{
			return 1;
		}
		head = head->next;
	}
	return 0;
}

void ECC_save(void* data, uint64_t sizeData, ECC_types algo){
	struct LinkedList* _memory = _heapHead;
	//save data into a stuct with crc16
	ECC_DATA_STRUCT* crc16_dataStructre = malloc(sizeof(ECC_DATA_STRUCT));
	//I need to genrate the code two times to avoid errors
	crc16_dataStructre->validator_code = 0; //UNDEF
    if (algo == CRC16)
    {
        crc16_dataStructre->validator_code = crc16(data, sizeData);
    }
    else if (algo == FLETCHER16)
    {
        crc16_dataStructre->validator_code = fletcher16(data,sizeData);
    }
	//crntly I am adding the position_in_memory data to make it easy
	//to search in a hash map but for now its redundent
	crc16_dataStructre->position_in_memory = 0;
	crc16_dataStructre->algo_type = algo;
	//save data into memory structure(a LinkedList for now)
	//look for a free spot in memory
	struct LinkedList* freeBuffer = NULL;
	if (_memory == NULL){freeBuffer = _memory;}
	else{
		while (_memory->next != NULL){_memory = _memory->next;}
		freeBuffer = _memory->next;
	}
	//ALLOCATE!
	freeBuffer = malloc(sizeof(struct LinkedList));
	freeBuffer->data = malloc(sizeof(ECC_DATA_STRUCT));
	freeBuffer->data = crc16_dataStructre;
	freeBuffer->next = NULL;
	_memory->next = freeBuffer;
}