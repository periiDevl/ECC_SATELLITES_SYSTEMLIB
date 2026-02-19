#include "ErrorCheckingCode.h"

// SIMPLEST WAY: Search the list for the record matching the pointer address
static ECC_DATA_STRUCT* find_record_in_list(void* data) {
    struct LinkedList* head = _heapHead;
    while (head != NULL) {
        ECC_DATA_STRUCT* record = (ECC_DATA_STRUCT*)(head->data);
        if ((void*)record->position_in_memory == data) {
            return record;
        }
        head = head->next;
    }
    return NULL;
}
uint8_t ECC_validate(void* data, uint64_t size, ECC_types algo) {
    ECC_DATA_STRUCT* record = find_record_in_list(data);

    if (algo == REEDSOLOMON && record != NULL) {
        // Work on copies so validate never mutates the original data
        uint8_t temp_data[255];
        uint8_t temp_parity[10];
        memcpy(temp_data, data, size);
        memcpy(temp_parity, record->rs_parity, 10);
        int result = rs_decode(temp_data, size, temp_parity, 10);
        return result == 0;  // 0 = clean = valid
    }

    uint16_t this_memory_code = 0;
    if (algo == CRC16) this_memory_code = crc16(data, size);
    else if (algo == FLETCHER16) this_memory_code = fletcher16(data, size);

    if (record && record->validator_code == this_memory_code) return 1;
    return 0;
}

void ECC_save(void* data, uint64_t sizeData, ECC_types algo) {
    ECC_DATA_STRUCT* ds = malloc(sizeof(ECC_DATA_STRUCT));
    ds->position_in_memory = (uint64_t)data; // SAVE THE POINTER ADDRESS
    ds->algo_type = algo;

    if (algo == REEDSOLOMON) {
        rs_init(); // Ensure tables are ready
        rs_encode(data, sizeData, ds->rs_parity, 10);
    } else if (algo == CRC16) {
        ds->validator_code = crc16(data, sizeData);
    } else if (algo == FLETCHER16) {
        ds->validator_code = fletcher16(data, sizeData);
    }

    // Standard list append
    struct LinkedList* newNode = malloc(sizeof(struct LinkedList));
    newNode->data = ds;
    newNode->next = NULL;

    struct LinkedList* temp = _heapHead;
    while (temp->next != NULL) temp = temp->next;
    temp->next = newNode;
}

uint8_t ECC_fix(void* data, uint64_t size, ECC_types algo) {
    if (algo != REEDSOLOMON) return 0;

    ECC_DATA_STRUCT* record = find_record_in_list(data);
    if (!record) return 0;

    // Use the saved parity in the list to fix the data at 'data'
    return rs_decode(data, size, record->rs_parity, 10);
}