#include "../virtual_alloc.h"
extern void* virtual_heap;

int main(){
    virtual_heap = virtual_sbrk(0);
    init_allocator(virtual_heap, 11, 8);
    
    void* a = virtual_malloc(virtual_heap, 256);
    int* b = virtual_malloc(virtual_heap, 256);
    virtual_info(virtual_heap);

    int array[5] = {1,2,3,4,5};

    b[0] = 1;
    b[1] = 2;
    b[2] = 3;
    b[3] = 4;
    b[4] = 5;

    b = virtual_realloc(virtual_heap, b, 1000);

    if (memcmp(b, array, 5*sizeof(int)) != 0){
        printf("Realloc modified the contents when it should not have.\n");
    }
    virtual_info(virtual_heap);

    virtual_free(virtual_heap, a);
    virtual_free(virtual_heap, b);

    virtual_info(virtual_heap);

    return 0;
}