#include <stdio.h>
#include "debug.h"
#include "alloc.h"
#include "masks.h"

void print_heap(heap_t* heap)
{
    heap_chunk_t* pointer = heap->memory;
    
    while(1)
    {
        if(pointer == heap->head)
        {
            
            printf("CHUNK @ %p | Raw pointer @ %p | Size: %d | STATUS: %s\nEND OF HEAP\n" , (void*)pointer, (void*)pointer + sizeof(heap_chunk_t), (int)pointer->size, IS_FLAG(pointer->flags, CHUNK_USE_MASK) == 1 ? "INUSE" : "FREE" );
            break;
        }
        
        printf("CHUNK @ %p | Raw pointer @ %p | Size: %d | STATUS: %s | " , (void*)pointer, (void*)pointer + sizeof(heap_chunk_t), (int)pointer->size, IS_FLAG(pointer->flags, CHUNK_USE_MASK) == 1 ? "INUSE" : "FREE" );
        pointer = (void*)pointer + sizeof(heap_chunk_t) + pointer->size;
        printf("Next Chunk @ %p\n", (void*)pointer);

    }
}