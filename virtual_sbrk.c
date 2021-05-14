#include "virtual_alloc.h"
#include <stdlib.h>

#define MAX_HEAP_SIZE 10000000

void* virtual_heap = NULL;
void* program_break;


void* virtual_sbrk(int32_t increment) {
    static int initialised_sbrk = 0;
    if (!initialised_sbrk){
        initialised_sbrk = 1;
        program_break = malloc(MAX_HEAP_SIZE);
        return program_break;
    } else {
        void * temp = program_break;
        program_break = (void *) (uint8_t*) program_break + increment;
        if ((uint8_t*) virtual_heap + MAX_HEAP_SIZE < (uint8_t*) program_break){
            return (void *)(-1);
        } else {
            return temp;
        }
    }
}