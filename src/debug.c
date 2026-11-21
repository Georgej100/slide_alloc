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
            
            printf("CHUNK @ %p | Raw pointer @ %p | ID: %d | Size: %d | STATUS: %d \nEND OF HEAP | HEAP->Head @ %p\n" , (void*)pointer, (void*)pointer + sizeof(heap_chunk_t), pointer->id,  (int)pointer->size, pointer->flags, (void*)heap->head );
            break;
        }
        
        printf("CHUNK @ %p | Raw pointer @ %p | ID: %d | Size: %d | STATUS: %d " , (void*)pointer, (void*)pointer + sizeof(heap_chunk_t), pointer->id,  (int)pointer->size,  pointer->flags);
        pointer = (void*)pointer + sizeof(heap_chunk_t) + pointer->size;
        printf("Next Chunk @ %p\n", (void*)pointer);
        n--;
    }

#endif
}

void print_master_table(master_table_t* master)
{
#ifdef DEBUG

    for(table_id_t id = 0; id < master->amount_entries; id++)
    {
        printf("ID: %d, STATUS: %s, REFs: %d\n", id, ATOMIC_IS_FLAG(&master->entries[id].controls.flags, ENTRY_USE_MASK) == 1 ? "INUSE" : "FREE", master->entries[id].controls.reference_count);
    }

#endif
}