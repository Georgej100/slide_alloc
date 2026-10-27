#include <stdint.h>
#include <stddef.h>
#include "alloc.h"
#include "assert.h"

int initialize_heap(heap_t* heap, void* memory, size_t size)
{
    ASSERT_RET(memory != NULL, HEAP_INIT_INVALID_MEMORY);
    ASSERT_RET(size > sizeof(heap_chunk_t) + 1, HEAP_INIT_LOW_MEMORY);

    heap_chunk_t* first = (heap_chunk_t*)memory;
    first->size = (size - sizeof(heap_chunk_t));
    first->flags &= CHUNK_FREE;

    heap->head = first;
    heap->memory = memory;
    heap->size = size;

    return HEAP_INIT_SUCCESS;
}
