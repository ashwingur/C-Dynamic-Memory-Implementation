#include "../virtual_alloc.h"
extern void* virtual_heap;

int main(){
    virtual_heap = virtual_sbrk(0);
    init_allocator(virtual_heap, 11, 8);
    
    void* a = virtual_malloc(virtual_heap, 700);
    virtual_info(virtual_heap);

    int x = 10;
    if (virtual_free(virtual_heap, a) != 0){
        printf("Free should have succeeded.\n");
    }
    if (virtual_free(virtual_heap, a) == 0){
        printf("Free should have failed.\n");
    }
    if (virtual_free(virtual_heap, &x) == 0){
        printf("Free should have failed.\n");
    }

    virtual_info(virtual_heap);

    return 0;
}