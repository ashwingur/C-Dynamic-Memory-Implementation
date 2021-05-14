#include "virtual_alloc.h"

void init_allocator(void * heapstart, uint8_t initial_size, uint8_t min_size) {
    // Basic error checks
    if (heapstart == NULL){
        printf("NULL heapstart.\n");
        return;
    }
    if (min_size > initial_size){
        printf("min_size cannot be greater than initial_size.\n");
        return;
    }

    // Requesting space for: metadata, allocatable memory, and the first buddy allocator block
    void* result = virtual_sbrk(sizeof(struct meta_data) + pow(2, initial_size) + sizeof(struct block));
    if (result == (void *)(- 1)){
        printf("Could not initialise heap because virtual_sbrk failed.\n");
        return;
    }

    // Initialising the meta data which lives at the start of the virtual heap
    struct meta_data* meta = (struct meta_data*) heapstart;
    meta->initial_size = initial_size;
    meta->min_size = min_size;
    meta->block_count = 1;
    
    // Initialise the first buddy allocator block which is the initial_size
    struct block* ptr = find_head(heapstart);
    ptr->size = initial_size;
    ptr->is_free = TRUE;
}

void* virtual_malloc(void * heapstart, uint32_t size) {
    uint8_t max_size = ((struct meta_data*) heapstart)->initial_size;
    uint8_t min_size = ((struct meta_data*) heapstart)->min_size;
    
    if (size == 0 || size > pow(2,max_size)){
        return NULL;
    }

    struct block* head = find_head(heapstart);
    int j = find_smallest_indice(size);

    // Smallest block size you can have is min_size
    if (j < min_size){
        j = min_size;
    }

    // Find the leftmost unallocated block of size j
    struct block* ptr = head;
    while (ptr <= find_tail(heapstart)){
        // If we have found a block of the correct size and it is free then allocate it and return
        if (ptr->size == j && ptr->is_free){
            ptr->is_free = FALSE;
            return find_location(heapstart, ptr);
        }
        ptr++;
    }

    // No such unallocated blocks exist from the previous loop. We must create an unallocated block of size j.
    // Try find the smallest block that exists, which is greater than j
    // If a block is found, then split it until it has the optimal size
    int k = j + 1;
    while (k <= max_size){
        struct block* b = find_unallocated_block(heapstart, k, head);
        if (b != NULL){
            // Found a block, now split it until it's size is k and allocate it
            b = split_block(heapstart, b, k - j);
            if (b == (void*)(-1)){
                return NULL;
            }
            b->is_free = FALSE;
            return find_location(heapstart, b);
        }
        k++;
    }
    // Nothing could be allocated
    return NULL;
}

int virtual_free(void * heapstart, void * ptr) {
    if (ptr == NULL || heapstart == NULL){
        return 1;
    }
    // Checking if ptr is an allocated memory by checking the locations of all allocated blocks
    struct block* b = find_head(heapstart);
    while (b <= find_tail(heapstart)){
        if (!b->is_free && find_location(heapstart, b) == ptr){
            b->is_free = TRUE;
            // Now that b has been freed, try merge it as high as possible
            int result = merge_block(heapstart, b);
            if (result == 1){
                return 1;
            }
            return 0;
        }
        b++;
    }
    // Ptr is already free or it does not exist
    return 1;
}

void* virtual_realloc(void * heapstart, void * ptr, uint32_t size) {
    if (heapstart == NULL){
        return NULL;
    }
    // If ptr is null or size is 0, behave as if virtual_free was called
    if (ptr == NULL || size == 0){
        virtual_free(heapstart, ptr);
        return NULL;
    }

    // Find the block associated with this ptr
    struct block* head = find_head(heapstart);
    struct block* b = head;
    while (b <= find_tail(heapstart)){
        if (find_location(heapstart, b) == ptr){
            break;
        }
        b++;
    }
    if (b == NULL || b->is_free){
        // Could not find the block, or the block is already free
        return NULL;
    }

    // Find the tail
    struct block* tail = find_tail(heapstart);

    int original_size = pow(2, b->size);

    int increment = original_size + 3 * sizeof(struct block);

    // Requesting some additional space to temporarily store a copy of the data at ptr
    void* result = virtual_sbrk(increment);
    if (result == (void*)(-1)){
        return NULL;
    }
    // Do tail + 3 to give virtual_malloc some room to shift without affecting the temporary data
    memmove(tail + 3, ptr, original_size);

    // Free ptr and try malloc the new size, if it fails then malloc the old size back
    virtual_free(heapstart, ptr);
    void* free = virtual_malloc(heapstart, size);
    if (free == NULL){
        // The realloc failed, malloc the old location back
        // Restoring the heap to how it was before realloc was called
        int mallocs_done = -1;
        struct block** temporary_malloc = (struct block**) (((char*) (tail + 3)) + original_size);
        while (free != ptr){
            mallocs_done++;
            // Keep doing 'fake' mallocs until we get to ptr
            free = virtual_malloc(heapstart, original_size);
            result = virtual_sbrk(sizeof(struct block**));
            memmove(temporary_malloc + mallocs_done, &free, sizeof(void*));
        }
        memmove(free, tail + 3, original_size);
        for (int i = 0; i < mallocs_done; i++){
            virtual_free(heapstart, *(temporary_malloc + i));
        }
        virtual_sbrk(-((mallocs_done + 1) * sizeof(struct block**)));
        virtual_sbrk(-increment);
        return NULL;
    } else {
        // Realloc successful, copy over the data and truncate if needed
        if (size < original_size){ 
            memmove(free, tail + 3, size);
        } else {
            memmove(free, tail + 3, original_size);
        }
        virtual_sbrk(-increment);
        return free;
    }  
}

void virtual_info(void * heapstart) {
    // Loop through all the buddy allocator blocks and print its info.
    struct block* ptr = find_head(heapstart);
    while (ptr <= find_tail(heapstart)){
        if (ptr->is_free){
            printf("free %d\n", (int) pow(2, ptr->size));
        } else {
            printf("allocated %d\n", (int) pow(2, ptr->size));
        }
        ptr++;
    }
}

int find_smallest_indice(uint32_t k){
    int i = 0;
    while (k > pow(2, i)){
        i++;
    }
    return i;
}

struct block* find_unallocated_block(void* heapstart, int size, struct block* head){
    // Find a block of size: size and return it's struct location, otherwise return null
    while (head <= find_tail(heapstart)){
        if (head->size == size && head->is_free){
            return head;
        }
        head++;
    }
    return NULL;
}

struct block* split_block(void* heapstart, struct block* b, int n){
    // Splits a block n times recursively. Note that only the left most split block is split

    // Base case is if n = 0, there is nothing more to split, so just return b
    if (n == 0){
        return b;
    }

    // Request an additional sizeof(block) space so we can fit another block in after the split
    void* result = virtual_sbrk(sizeof(struct block));
    if (result == (void*)(-1)){
        return result;
    }

    // First find the tail
    struct block* tail = find_tail(heapstart);
    
    // Now go backwards from the tail all the way to b, shifting everything to the right by 1 by copying
    while (tail != b){
        memmove(tail + 1, tail, sizeof(struct block));
        tail -= 1;
    }

    // Now the block after b is considered 'empty' and we can fill it with a new block
    // Halve the size of the block b
    b->size -= 1;

    // Create the right block
    (b + 1)->size = b->size;
    (b + 1)->is_free = TRUE;

    increment_block_counter(heapstart);
    // Recursive
    return split_block(heapstart, b, n - 1);
}

int merge_block(void* heapstart, struct block* b){
    // The left block will double in size while the right block gets 'deleted'
    struct block* left;
    struct block* right;

   if (is_left_buddy(heapstart, b)){
        left = b;
        right = b + 1;
    } else {
        left = b - 1;
        right = b;
    }
    
    // Checking for the edge and base cases where b cannot merge any further
    if (left < find_head(heapstart) || right > find_tail(heapstart)){
        return 0;
    }
    if (!left->is_free || !right->is_free){
        return 0;
    }
    if (left->size != right->size){
        return 0;
    }

    // Now we know both left and right can be merged, perform the merge
    // Left doubles in size
    left->size++;

    // Assume right is now deleted, so we can shift everything after right to the left by 1 block
    while (right < find_tail(heapstart)){
        // Left shift
        memmove(right, right + 1, sizeof(struct block));
        right++; 
    }

    // Since we have deleted a block we can decrement the sbrk
    void* result = virtual_sbrk(-((int32_t) sizeof(struct block)));
    if (result == (void*)(-1)){
        return 1;
    }

    decrement_block_counter(heapstart);

    // Now try to merge left further
    return merge_block(heapstart, left);
}

void* find_location(void* heapstart, struct block* block){
    struct block* b = find_head(heapstart);
    char* location = (char*) heapstart + sizeof(struct meta_data);
    while (b != block){
        location += (int) pow(2, b->size);
        b++;
        if (b > find_tail(heapstart)){
            return NULL;
        }
    }
    return (void*) location;
}

struct block* find_head(void* heapstart){
    int initial_size = ((struct meta_data*) heapstart)->initial_size;
    return (struct block*) ((char*) heapstart + (int) (sizeof(struct meta_data) + pow(2, initial_size)));
}

struct block* find_tail(void* heapstart){
    struct block* tail = find_head(heapstart);
    tail += ((struct meta_data*) heapstart)->block_count - 1;
    return tail;
}

void increment_block_counter(void* heapstart){
    ((struct meta_data*) heapstart)->block_count++;
}

void decrement_block_counter(void* heapstart){
    ((struct meta_data*) heapstart)->block_count--;
}

int is_left_buddy(void* heapstart, struct block* b){
    // If the total size of all the blocks to the left of b, divided by the size of b is even
    // then b is a left buddy, otherwise b is a right buddy
    int sum = 0;
    struct block* cursor = find_head(heapstart);
    while (cursor != b){
        sum += pow(2, cursor->size);
        cursor++;
    }
    int equivalent_b = sum / pow(2, b->size); 
    if (equivalent_b % 2 == 0){
        return 1;
    } else {
        return 0;
    }
}