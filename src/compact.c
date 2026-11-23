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

    heap_state_t state = atomic_load(&heap->state);
    
    while (current_chunk != (heap_chunk_t*)state.head) // Mabye unsafe
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
            controls_t tmp = atomic_load(&entry->controls);
            tmp.reference_count = 0;
            atomic_store(&entry->controls, tmp);
        }

        current_chunk = next_chunk;
        
    }
    

    heap_state_t current_state = atomic_load(&heap->state);
    
    
    heap_state_t new_state = {
        .head = (void*)current_state.head + free_behind,
        .size = current_state.size + free_behind,
    };

    if(!atomic_compare_exchange_weak(&heap->state, &current_state, new_state))
    {
        new_state.head->size = free_behind - sizeof(heap_chunk_t);
        new_state.head->id = -1;
        new_state.head->flags = 0;
    }
    

    return free_behind;
    
}