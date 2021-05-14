#include "virtual_alloc.h"
#include <stdlib.h>

#define MAX_HEAP_SIZE 10000000

void * virtual_heap = NULL;
void * program_break;


void * virtual_sbrk(int32_t increment) {
    static int initialised_sbrk = 0;
    if (!initialised_sbrk){
        initialised_sbrk = 1;
        program_break = malloc(MAX_HEAP_SIZE);
        return program_break;
    } else {
        void * temp = program_break;
        program_break = (void *) (uint8_t*) program_break + increment;
        if ((uint8_t*) virtual_heap + MAX_HEAP_SIZE < (uint8_t*) program_break){
            printf("sbrk could not be extended.\n");
            return (void *)(-1);
        } else {
            return temp;
        }
    }
}

void line(){
    printf("\n");
}

int main() {
    printf("sizeof struct %ld\n", sizeof(struct block));
    // Your own testing code here
    virtual_heap = virtual_sbrk(0);
    init_allocator(virtual_heap, 23, 8);
    virtual_info(virtual_heap);
    line();

    void* arr[32768];

    for (int i = 0; i < 32767; i ++) {
        arr[i] = virtual_malloc(virtual_heap, 1);
        printf("%d\n", i);
    }

    for (int i = 32767; i >= 0; i--) {
        virtual_free(virtual_heap, arr[i]);
    }

    virtual_info(virtual_heap);

    // void* a = virtual_malloc(virtual_heap, 4194303);
    // virtual_info(virtual_heap);
    // line();

    // void* b = virtual_malloc(virtual_heap, 256);
    // virtual_info(virtual_heap);
    // line();

    // void* c = virtual_malloc(virtual_heap, 512);
    // void* d = virtual_malloc(virtual_heap, 1024);
    // void* e = virtual_malloc(virtual_heap, 2048);

    // virtual_info(virtual_heap);
    // line();



    // virtual_free(virtual_heap, a);
    // virtual_info(virtual_heap);
    // line();

    // virtual_free(virtual_heap, b);
    // virtual_free(virtual_heap, c);
    // virtual_free(virtual_heap, d);
    // virtual_free(virtual_heap, e);
    // virtual_info(virtual_heap);
    // line();

    return 0;
}
