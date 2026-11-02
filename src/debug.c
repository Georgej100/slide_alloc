#include <stdio.h>
#include "debug.h"
#include "alloc.h"
#include "masks.h"

void print_heap(heap_t* heap)
{
    heap_chunk_t* pointer = heap->memory;
    
    while(1)
    {
        printf("CHUNK @ %p | Raw pointer @ %p | Size: %d | STATUS: %s" , (void*)pointer, (void*)pointer + sizeof(heap_chunk_t), (int)pointer->size, IS_FLAG(pointer->flags, CHUNK_USE_MASK) == 1 ? "INUSE" : "FREE" );
        pointer += pointer->size;

        if(pointer == heap->head)
        {
            break;
        }
    }
}