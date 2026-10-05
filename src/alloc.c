#include <stdint.h>
#include <assert.h>
#include "alloc.h"


heap_t heap_init(void* memory, uint32_t size)
{
    heap_t heap;

    assert(memory);

    heap.head = memory;
    heap.size = size;

    return heap;
}


void* heap_alloc(heap_t* heap, uint32_t size)
{
    assert(size < heap->size);

    heap->head += size;
    heap->size -= size;

    return heap->head - size;
}

