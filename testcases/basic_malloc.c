#include "../virtual_alloc.h"
extern void* virtual_heap;

int main(){
    virtual_heap = virtual_sbrk(0);
    init_allocator(virtual_heap, 11, 8);
    void* a = virtual_malloc(virtual_heap, 256);
    virtual_info(virtual_heap);
    void* b = virtual_malloc(virtual_heap, 512);
    virtual_info(virtual_heap);
    void* c = virtual_malloc(virtual_heap, 1024);
    virtual_info(virtual_heap);
    return 0;
}