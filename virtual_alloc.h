#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define META_DATA_SIZE 2

// The buddy allocator data structures is essentially just an arraylist of 'blocks' in order
struct block{
    uint8_t size;
    uint8_t is_free;
};

// Stored at the start of the virtual heap containing useful information
struct meta_data{
    uint8_t initial_size;
    uint8_t min_size;
    uint64_t block_count;
};

// Initialises the virtual heap
void init_allocator(void * heapstart, uint8_t initial_size, uint8_t min_size);

// Tries to allocate memory based on the given input size
void* virtual_malloc(void * heapstart, uint32_t size);

// Tries to free a malloced block if it exists
int virtual_free(void* heapstart, void* ptr);

// Tries to reallocate a given malloced block to the new size, copying over the data
void* virtual_realloc(void* heapstart, void* ptr, uint32_t size);

// Prints the state of all the allocated and free blocks in the virtual heap
void virtual_info(void* heapstart);

// Increases and decreases the virtual heap size
void* virtual_sbrk(int32_t increment);

// Split a given block n times (Only the left most block gets split)
struct block* split_block(void* heapstart, struct block* b, int n);

// Finds the first block from the left with the given size if it exists
struct block* find_unallocated_block(void* heapstart, int size, struct block* head);

// Finds the smallest power of 2 that is greater than k
int find_smallest_indice(uint32_t k);

// Given a block b, merges it as much as possible while it still has a free buddy
int merge_block(void* heapstart, struct block* b);

// Finds the start point of a given block's allocatable memory.
void* find_location(void* heapstart, struct block* block);

// Returns the leftmost buddy allocator data structure block
struct block* find_head(void* heapstart);

// Returns the rightmost buddy allocator data structure block
struct block* find_tail(void* heapstart);

// Increment the block count stored in the metadata by 1
void increment_block_counter(void* heapstart);

// Decrement the block count stored in the metadata by 1
void decrement_block_counter(void* heapstart);

// Returns 1 if b is a left buddy, or 0 if it is a right buddy.
int is_left_buddy(void* heapstart, struct block* b);