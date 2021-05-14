#include "../virtual_alloc.h"
extern void* virtual_heap;

int main(){
    virtual_heap = virtual_sbrk(0);
    init_allocator(virtual_heap, 20, 8);
    virtual_info(virtual_heap);
    void* array[400];
    for (int i = 0; i < 400; i++){
        array[i] = virtual_malloc(virtual_heap, 256);
    }
    virtual_info(virtual_heap);
    for (int i = 0; i < 400; i++){
        virtual_free(virtual_heap, array[i]);
    }
    virtual_info(virtual_heap);
 
    return 0;
}