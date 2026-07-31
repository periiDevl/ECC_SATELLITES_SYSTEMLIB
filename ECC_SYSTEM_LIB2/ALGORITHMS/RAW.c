#include "RAW.h"

char* ECCLAGORAW_debug=NULL;
void ECCALGO_RAW(void* data, uint64_t length)
{
    if (ECCLAGORAW_debug != NULL) {
        free(ECCLAGORAW_debug);
        ECCLAGORAW_debug = NULL;
    }

    ECCLAGORAW_debug = (char*)malloc(length + 1);
    
    if (ECCLAGORAW_debug == NULL) {
        return;
    }

    memcpy(ECCLAGORAW_debug, data, length);
    ECCLAGORAW_debug[length] = '\0'; 
}