#include <stdio.h>
#include "debug.h"
#include "alloc.h"
#include "masks.h"



void print_heap( __attribute__ ((unused)) heap_t* heap)
{    
#ifdef DEBUG
    heap_chunk_t* pointer = heap->memory;
    int n = 7;

    while(n != 0)
    {
        if(pointer == heap->head)
        {
            
            printf("CHUNK @ %p | Raw pointer @ %p | ID: %d | Size: %d | STATUS: %s\nEND OF HEAP | HEAP->Head @ %p\n" , (void*)pointer, (void*)pointer + sizeof(heap_chunk_t), pointer->id,  (int)pointer->size, IS_FLAG(pointer->flags, CHUNK_USE_MASK) == 1 ? "INUSE" : "FREE", (void*)heap->head );
            break;
        }
        
        printf("CHUNK @ %p | Raw pointer @ %p | ID: %d | Size: %d | STATUS: %s | " , (void*)pointer, (void*)pointer + sizeof(heap_chunk_t), pointer->id,  (int)pointer->size,  IS_FLAG(pointer->flags, CHUNK_USE_MASK) == 1 ? "INUSE" : "FREE" );
        pointer = (void*)pointer + sizeof(heap_chunk_t) + pointer->size;
        printf("Next Chunk @ %p\n", (void*)pointer);
        n--;
    }

#endif
}

