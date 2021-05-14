#include "../virtual_alloc.h"
extern void* virtual_heap;

int main(){
    virtual_heap = virtual_sbrk(0);
    init_allocator(virtual_heap, 5, 2);
    virtual_info(virtual_heap);
    
    void* a = virtual_malloc(virtual_heap, 4);
    void* b = virtual_malloc(virtual_heap, 4);
    void* c = virtual_malloc(virtual_heap, 4);
    void* d = virtual_malloc(virtual_heap, 4);
    void* e = virtual_malloc(virtual_heap, 4);
    void* f = virtual_malloc(virtual_heap, 4);
    void* g = virtual_malloc(virtual_heap, 4);
    void* h = virtual_malloc(virtual_heap, 4);

    virtual_free(virtual_heap, a);
    virtual_free(virtual_heap, e);

    virtual_info(virtual_heap);
    int* i = virtual_realloc(virtual_heap, d, 8);
    virtual_info(virtual_heap);
    return 0;
}