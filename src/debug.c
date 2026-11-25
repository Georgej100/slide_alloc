#include <stdio.h>
#include "debug.h"
#include "alloc.h"
#include "masks.h"
#include "globals.h"



void print_heap()
{    
#ifdef DEBUG
    heap_chunk_t* pointer = (void*)global_heap.memory;
    int n = 101;

    while(n != 0)
    {
        if(pointer == atomic_load(&global_heap.state).head) // Unsafe mabye
        {
            
            printf("CHUNK @ %p | Raw pointer @ %p \nID: %d | Size: %d | STATUS: %d \nEND OF HEAP | HEAP->Head @ %p\n" , 
                (void*)pointer, (void*)pointer + sizeof(heap_chunk_t), -1,  atomic_load(&global_heap.state).size, pointer->flags, 
                (void*)atomic_load(&global_heap.state).head );
            break;
        }
        
        printf("CHUNK @ %p | Raw pointer @ %p \nID: %d | Size: %d | STATUS: %d " , (void*)pointer, (void*)pointer + sizeof(heap_chunk_t), 
            pointer->id,  (int)pointer->size,  pointer->flags);
        pointer = (void*)pointer + sizeof(heap_chunk_t) + pointer->size;
        printf("Next Chunk @ %p\n", (void*)pointer);
        n--;
    }

#endif
}

void print_master_table()
{
#ifdef DEBUG

    for(uint32_t id = 0; id < global_table_state.amount_entries; id++)
    {
        controls_t controls = atomic_load(&global_table[id].controls);
        printf("ID: %d, STATUS: %s, REFs: %d\n", id, IS_FLAG(controls.flags, ENTRY_USE_MASK) == 1 ? "INUSE" : "FREE", controls.reference_count);
    }

#endif
}