#include "../virtual_alloc.h"
extern void* virtual_heap;

int main(){
    virtual_heap = virtual_sbrk(0);
    init_allocator(virtual_heap, 13, 8);
    void* a = virtual_malloc(virtual_heap, 100);
    virtual_info(virtual_heap);
    void* b = virtual_malloc(virtual_heap, 3000);
    virtual_info(virtual_heap);
    void* c = virtual_malloc(virtual_heap, 500);
    virtual_info(virtual_heap);

    virtual_free(virtual_heap, a);
    virtual_info(virtual_heap);
    virtual_free(virtual_heap, b);
    virtual_info(virtual_heap);
    virtual_free(virtual_heap, c);
    virtual_info(virtual_heap);

    a = virtual_malloc(virtual_heap, 1000);
    virtual_info(virtual_heap);

    a = virtual_realloc(virtual_heap, a, 4000);
    virtual_info(virtual_heap);
    virtual_free(virtual_heap, a);
    virtual_info(virtual_heap);
    return 0;
}