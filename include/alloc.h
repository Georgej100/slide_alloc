#pragma once
#include <stdint.h>
#include <stddef.h>
#include "master_table.h"

#define HEAP_INIT_SUCCESS 0
#define HEAP_INIT_INVALID_MEMORY -1
#define HEAP_INIT_LOW_MEMORY -2
#define HEAP_INIT_INVALID_HEAP -3

#define HEAP_ALLOC_INVALID_HEAP -1
#define HEAP_ALLOC_TOO_SMALL -2
#define HEAP_ALLOC_NO_IDS -3

#define CHUNK_USE_MASK (1 << 0)

typedef struct
{
    uint16_t size;
    _Atomic uint8_t flags;
    table_id_t id; 
} heap_chunk_t;


typedef struct
{
    heap_chunk_t* head;
    uint16_t size;
} heap_state_t;

typedef struct
{
    void* memory;
    master_table_t* master_table;
    _Atomic heap_state_t state;
} heap_t;

/*
 * Initialises the user given heap pointer
 *  Memory must be valid 
 *  Size must be greater than sizeof(heap_chunk_t) + 1
 *
 *  Returns 0 on success or error code beginning in HEAP_INIT
 *
 * */
int heap_init(uint16_t size);

table_id_t salloc(uint16_t size);

int sfree(table_id_t id);

void setup_chunk(heap_chunk_t* chunk, uint16_t size, table_id_t id, uint8_t flags);

void* dereference(table_id_t id);

int exit_dereference(table_id_t id);