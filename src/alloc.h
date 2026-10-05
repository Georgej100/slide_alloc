#pragma once
#include <stdint.h>

typedef struct 
{
    void* head;
    uint32_t size;
} heap_t;

heap_t heap_init(void* memory, uint32_t size);

void* heap_alloc(heap_t* heap, uint32_t size);
