#pragma once
#include <stdint.h>
#include <stddef.h>
#include "master_table.h"

#define HEAP_INIT_SUCCESS 0
#define HEAP_INIT_INVALID_MEMORY 1
#define HEAP_INIT_LOW_MEMORY 2
#define HEAP_INIT_INVALID_HEAP 3

#define HEAP_ALLOC_INVALID_HEAP 1
#define HEAP_ALLOC_TOO_SMALL 2
#define HEAP_ALLOC_NO_IDS 3

#define CHUNK_USE_MASK (1 << 0)

typedef struct
{
    size_t size;
    _Atomic uint16_t flags;
    unsigned int id; //TEMP TODO
} heap_chunk_t;

typedef struct 
{
    void* memory;   // Memory array given by OS/Hardware
    master_table_t* master_table;
    heap_chunk_t* head;     // Head of bump allocator
    size_t size;    // Size of total memory
} heap_t;


/*
 * Initialises the user given heap pointer
 *  Memory must be valid 
 *  Size must be greater than sizeof(heap_chunk_t) + 1
 *
 *  Returns 0 on success or error code beginning in HEAP_INIT
 *
 * */
int heap_init(heap_t* heap, void* memory, size_t size);

table_id_t salloc(heap_t* heap, size_t size);

int sfree(heap_t* heap, table_id_t id);