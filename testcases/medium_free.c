#include "../virtual_alloc.h"
extern void* virtual_heap;

int main(){
    virtual_heap = virtual_sbrk(0);
    init_allocator(virtual_heap, 14, 8);
    void* a = virtual_malloc(virtual_heap, 100);
    virtual_info(virtual_heap);
    void* b = virtual_malloc(virtual_heap, 300);
    virtual_info(virtual_heap);
    void* c = virtual_malloc(virtual_heap, 1000);
    virtual_info(virtual_heap);
    void* d = virtual_malloc(virtual_heap, 20);
    virtual_info(virtual_heap);
    void* e = virtual_malloc(virtual_heap, 3000);
    virtual_info(virtual_heap);
    void* f = virtual_malloc(virtual_heap, 900);
    virtual_info(virtual_heap);
    void* g = virtual_malloc(virtual_heap, 4000);
    virtual_info(virtual_heap);
    void* h = virtual_malloc(virtual_heap, 50);
    virtual_info(virtual_heap);

    virtual_free(virtual_heap, a);
    virtual_info(virtual_heap);
    virtual_free(virtual_heap, b);
    virtual_info(virtual_heap);
    virtual_free(virtual_heap, c);
    virtual_info(virtual_heap);
    virtual_free(virtual_heap, d);
    virtual_info(virtual_heap);
    virtual_free(virtual_heap, e);
    virtual_info(virtual_heap);
    virtual_free(virtual_heap, f);
    virtual_info(virtual_heap);
    virtual_free(virtual_heap, g);
    virtual_info(virtual_heap);
    virtual_free(virtual_heap, h);
    virtual_info(virtual_heap);
    return 0;
}