#include "../virtual_alloc.h"
extern void* virtual_heap;

int main(){
    virtual_heap = virtual_sbrk(0);
    init_allocator(virtual_heap, 11, 8);
    
    void* a = virtual_malloc(virtual_heap, 100000);
    virtual_info(virtual_heap);

    if (a != NULL){
        printf("Malloc should not have succeeded.\n");
    }

    virtual_info(virtual_heap);

    return 0;
}