#include "../virtual_alloc.h"
extern void* virtual_heap;

int main(){
    virtual_heap = virtual_sbrk(0);
    init_allocator(virtual_heap, 13, 6);
    void* a = virtual_malloc(virtual_heap, 1024);
    virtual_info(virtual_heap);
    void* b = virtual_malloc(virtual_heap, 8);
    virtual_info(virtual_heap);
    void* c = virtual_malloc(virtual_heap, 2048);
    virtual_info(virtual_heap);
    void* d = virtual_malloc(virtual_heap, 2048);
    virtual_info(virtual_heap);
    return 0;
}