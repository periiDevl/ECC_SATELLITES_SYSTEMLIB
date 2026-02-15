#include"CRC.h"
uint16_t crc16(const uint8_t *data, size_t length) {
    // Initial value is usually 0xFFFF or 0x0000 depending on the protocol.
    // 0xFFFF helps detect leading zeros that might otherwise be ignored.
    uint16_t crc = 0xFFFF; 

    // Loop through each byte in the input buffer
    for (size_t i = 0; i < length; i++) {
        
        // Move the current data byte into the high byte of the 16-bit CRC register
        // and XOR it with the current CRC state.
        crc ^= (uint16_t)data[i] << 8;

        // Process each of the 8 bits in the current byte
        for (uint8_t j = 0; j < 8; j++) {
            
            // Check the Most Significant Bit (MSB). 
            // 0x8000 is 1000 0000 0000 0000 in binary.
            if (crc & 0x8000) {
                // If the MSB is 1, shift left and XOR with the polynomial.
                // This is the "division" step in polynomial arithmetic.
                crc = (crc << 1) ^ POLYNOMIAL;
            } else {
                // If the MSB is 0, just shift left.
                // The polynomial isn't applied because the "divisor" doesn't fit.
                crc <<= 1;
            }
        }
    }

    // Return the final calculated 16-bit remainder (the CRC)
    return crc;
}
/*
uint8_t VALIDATOR_crc16Validate(void* data, uint64_t size)
{
	//first lets define the crc16 block to track
	//the next step is having 2 diffrent memory blocks
	crc16_Data* _crc16ValidatingMemoryBlock;
	//somehow need to track who is who using pos in mem but for now I will just compare
	struct LinkedList* head = _heapHead;
	uint16_t this_memory_code = crc16(data, size);
	printf("genrated = %d", this_memory_code);
	while (head != NULL)
	{
		_crc16ValidatingMemoryBlock = (crc16_Data*)(head->data);
		if (_crc16ValidatingMemoryBlock->validator_code == this_memory_code)
		{
			return 1;
		}
		head = head->next;
	}
	return 0;
	
	/*
	uint8_t fail = 0;
	for (int i=0;i<size;i++)
	{
		printf("start\n");
		unsigned char cur_byte = ((unsigned char*)data)[i];
		for (int j=0;j<sizeof(unsigned char)*8;j++){
			if (cur_byte & 0x1 == 1){
				fail++;
			}
			cur_byte = cur_byte>>1;
		}
	}
	printf("fail index = %d", fail);
	return 1;	
	*/
//}
/*
void VALIDATOR_crc16Save(void* data, uint64_t sizeData){
	struct LinkedList* _memory = _heapHead;
	//save data into a stuct with crc16
	crc16_Data* crc16_dataStructre = malloc(sizeof(crc16_Data));
	//I need to genrate the code two times to avoid errors
	crc16_dataStructre->validator_code = crc16((uint8_t*)data,sizeData);
	//crntly I am adding the position_in_memory data to make it easy
	//to search in a hash map but for now its redundent
	crc16_dataStructre->position_in_memory = 0;
	crc16_dataStructre->algo_type = CRC16;
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
	freeBuffer->data = malloc(sizeof(crc16_Data));
	freeBuffer->data = crc16_dataStructre;
	freeBuffer->next = NULL;
	_memory->next = freeBuffer;
}
*/