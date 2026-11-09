#include <stdint.h>
#include <stddef.h>
#include "alloc.h"
#include "assert.h"
#include "masks.h"
#include "master_table.h"

int heap_init(heap_t* heap, void* memory, size_t size)
{
    ASSERT_RET(memory != NULL, HEAP_INIT_INVALID_MEMORY);
    ASSERT_RET(size > sizeof(heap_chunk_t) + 1, HEAP_INIT_LOW_MEMORY);
    ASSERT_RET(heap != NULL, HEAP_INIT_INVALID_HEAP);

    heap_chunk_t* first = (heap_chunk_t*)memory;
    first->size = (size - sizeof(heap_chunk_t));
    CLEAR_FLAG(first->flags, CHUNK_USE_MASK);


    heap->head = first;
    heap->memory = memory;
    heap->size = size;

    return HEAP_INIT_SUCCESS;
}

table_id_t salloc(heap_t* heap, size_t size)
{
    ASSERT_RET(heap != NULL, HEAP_ALLOC_INVALID_HEAP);
    ASSERT_RET(size > 0, HEAP_ALLOC_TOO_SMALL);

    size_t prev_size = heap->head->size;

    // Get ID
    table_id_t id;
    ASSERT_RET(get_next_id(heap->master_table, &id) == MASTER_GET_SUCCESS, 
        HEAP_ALLOC_NO_IDS);
    table_entry_t* entry = &heap->master_table->entries[id];

    // Setup user chunk
    heap_chunk_t* chunk = heap->head;
    SET_FLAG(chunk->flags, CHUNK_USE_MASK);
    chunk->size = size;
    chunk->id = id;

    entry->raw = (void*)heap->head + sizeof(heap_chunk_t);

    // Setup next chunk
    heap->head = (void*)heap->head + sizeof(heap_chunk_t) + size;
    heap->head->size = prev_size - sizeof(heap_chunk_t) - size;
    heap->head->id = -1;
    CLEAR_FLAG(heap->head->flags, CHUNK_USE_MASK);

    return id;
}


int sfree(heap_t* heap, table_id_t id)
{
    uint16_t old_master_flags = atomic_load(&heap->master_table->entries[id].flags);
    uint16_t new_master_flags;

    do {
        
        if (!(old_master_flags & ENTRY_USE_MASK)) {
            return 0; 
        }
        
        
        new_master_flags = old_master_flags & ~ENTRY_USE_MASK;

    } while (!atomic_compare_exchange_weak(&heap->master_table->entries[id].flags, &old_master_flags, new_master_flags));

    heap_chunk_t* entry = (void*)heap->master_table->entries[id].raw - sizeof(heap_chunk_t);
    ATOMIC_CLEAR_FLAG(&entry->flags, CHUNK_USE_MASK);

    ASSERT_RET(return_id(heap->master_table, id) != MASTER_RETURN_UNUSED, -1);

    return 0;
}
