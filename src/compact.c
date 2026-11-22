#include <memory.h>
#include <stdatomic.h>
#include "alloc.h"
#include "masks.h"
#include "master_table.h"

enum State
{
    FREE,
    UNOBSERVED,
    OBSERVED,
};

int bubble_compact(heap_t* heap)
{
    uint16_t free_behind = 0;
    heap_chunk_t* current_chunk = heap->memory;
    heap_chunk_t* next_chunk;
    
    while (current_chunk != heap->head)
    {
        enum State current_state = FREE;
        next_chunk = (void*)current_chunk + sizeof(heap_chunk_t) + current_chunk->size;
        table_entry_t* entry = &heap->master_table->entries[current_chunk->id];
        
        if (!IS_FLAG(current_chunk->flags, CHUNK_USE_MASK))
        {
            current_state = FREE;
        }
        else
        {
            controls_t expected_entry = atomic_load(&entry->controls);
            controls_t new_entry = expected_entry;

            do
            {
                if (expected_entry.reference_count > 0)
                {
                    current_state = OBSERVED;
                    break;
                }
                
                new_entry.reference_count = -1;
                current_state = UNOBSERVED;
            } while (!atomic_compare_exchange_weak(&entry->controls, &expected_entry, new_entry));

        }


        switch (current_state)
        {
        case FREE:
            free_behind += sizeof(heap_chunk_t) + current_chunk->size;
            break;
        
        case OBSERVED:
            free_behind = 0;
            if(free_behind != 0)
            {
                heap_chunk_t* behind = (void*)current_chunk - free_behind;
                behind->size = free_behind - sizeof(heap_chunk_t);
                behind->id = -1;
                behind->flags = 0;
            }
            break;
        
        case UNOBSERVED:
            heap_chunk_t* new_spot = (void*)current_chunk - free_behind;
            memmove(new_spot, current_chunk, (sizeof(heap_chunk_t) + current_chunk->size));
            entry->raw = (void*)new_spot + sizeof(heap_chunk_t);
        }

        current_chunk = next_chunk;
        
    }
    
    heap->head = (void*)heap->head - free_behind; //UNSAFE FIX PLZ
    heap->size = heap->size + free_behind;
    return free_behind;
    
}