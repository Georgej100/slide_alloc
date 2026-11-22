#include <stdint.h>
#include <stddef.h>
#include <threads.h>
#include "alloc.h"
#include "assert.h"
#include "masks.h"
#include "master_table.h"


int heap_init(heap_t* heap, void* memory, uint16_t size)
{
    ASSERT_RET(memory != NULL, HEAP_INIT_INVALID_MEMORY);
    ASSERT_RET(size > sizeof(heap_chunk_t) + 1, HEAP_INIT_LOW_MEMORY);
    ASSERT_RET(heap != NULL, HEAP_INIT_INVALID_HEAP);

    heap_chunk_t* first = (heap_chunk_t*)memory;
    first->size = (size - sizeof(heap_chunk_t));
    CLEAR_FLAG(first->flags, CHUNK_USE_MASK);


    heap->head = first;
    heap->memory = memory;
    heap->size = size - sizeof(heap_chunk_t);

    return HEAP_INIT_SUCCESS;
}

table_id_t salloc(heap_t* heap, uint16_t size)
{
    ASSERT_RET(heap != NULL, HEAP_ALLOC_INVALID_HEAP);
    ASSERT_RET(size > 0, HEAP_ALLOC_TOO_SMALL);
    
    table_id_t id;
    ASSERT_RET(get_next_id(heap->master_table, &id) == MASTER_GET_SUCCESS, 
        HEAP_ALLOC_NO_IDS);

    heap_t current_state = *heap;
    heap_t new_state = current_state;
    
    do
    {
        ASSERT_RET(current_state.size > sizeof(heap_chunk_t) + size, -1)

        new_state.head = (void*)new_state.head + sizeof(heap_chunk_t) + size;
        new_state.size = new_state.size - sizeof(heap_chunk_t) - size;    
    } while (!atomic_compare_exchange_weak(heap, &current_state, new_state));

    setup_chunk(current_state.head, size, id, CHUNK_USE_MASK);

    heap->master_table->entries[id].raw = (void*)current_state.head + sizeof(heap_chunk_t);

    return id;
}

void setup_chunk(heap_chunk_t* chunk, uint16_t size, table_id_t id, uint8_t flags)
{
    chunk->flags = flags;
    chunk->size = size;
    chunk->id = id;
    
}

int sfree(heap_t* heap, table_id_t id)
{
    controls_t old_master = atomic_load(&heap->master_table->entries[id].controls);
    controls_t new_master = old_master;

    do {
        
        if (!IS_FLAG(old_master.flags, ENTRY_USE_MASK)) {
            return 0; 
        }
        
        if(old_master.reference_count != 0)
        {
            return -1;
        }
        
        CLEAR_FLAG(new_master.flags, ENTRY_USE_MASK);
        new_master.reference_count = 0;

    } while (!atomic_compare_exchange_weak(&heap->master_table->entries[id].controls, &old_master, new_master));

    heap_chunk_t* entry = (void*)heap->master_table->entries[id].raw - sizeof(heap_chunk_t);
    CLEAR_FLAG(entry->flags, CHUNK_USE_MASK);
    entry->flags = 0;

    ASSERT_RET(return_id(heap->master_table, id) != MASTER_RETURN_UNUSED, -2);

    return 0;
}

void* dereference(heap_t* heap, table_id_t id)
{
    controls_t current = atomic_load(&heap->master_table->entries[id].controls);
    controls_t new = current;

    do
    {
        if(!IS_FLAG(current.flags, ENTRY_USE_MASK))
        {
            return NULL;
        }
        new.reference_count += 1;
    } while (!atomic_compare_exchange_weak(&heap->master_table->entries[id].controls, &current, new));

    return heap->master_table->entries[id].raw;
    
}

int exit_dereference(heap_t* heap, table_id_t id)
{
    controls_t current = atomic_load(&heap->master_table->entries[id].controls);
    controls_t new = current;

    do
    {
        if((current.reference_count < 1) || (!IS_FLAG(current.flags, ENTRY_USE_MASK)))
        {
            return -1;
        }
        new.reference_count -= 1;
    } while (!atomic_compare_exchange_weak(&heap->master_table->entries[id].controls, &current, new));

    return 0;
    
}