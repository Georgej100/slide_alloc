#pragma once
#include <stdint.h>


#define MAX_TABLE_ENTRIES 10

typedef struct {
    void* raw;
    uint32_t size;
    char inuse;
} table_entry_t;

typedef int entry_id_t;

void* dereference(entry_id_t id);

typedef struct 
{
    void* head;
    uint32_t size;
} heap_t;

heap_t heap_init(void* memory, uint32_t size);

entry_id_t heap_alloc(heap_t* heap, uint32_t size);
