#include "../virtual_alloc.h"
extern void* virtual_heap;

int main(){
    virtual_heap = virtual_sbrk(0);
    init_allocator(virtual_heap, 11, 8);
    
    void* a = virtual_malloc(virtual_heap, 512);
    void* b = virtual_malloc(virtual_heap, 512);
    void* c = virtual_malloc(virtual_heap, 1024);

    void* d = virtual_malloc(virtual_heap, 256);
    virtual_info(virtual_heap);

    if (d != NULL){
        printf("Malloc should not have succeeded.\n");
    }

    virtual_info(virtual_heap);


    virtual_free(virtual_heap, a);
    virtual_free(virtual_heap, b);
    virtual_free(virtual_heap, c);

    virtual_info(virtual_heap);

    return 0;
}