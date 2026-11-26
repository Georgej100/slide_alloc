#include <stdint.h>
#include <stddef.h>
#include <threads.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include "alloc.h"
#include "assert.h"
#include "masks.h"
#include "master_table.h"
#include "heap_manager.h"


int heap_init(heap_t* heap, size_t initial_size)
{
    ASSERT_RET(initial_size > sizeof(heap_chunk_t) + 1, HEAP_INIT_LOW_MEMORY);
    ASSERT_RET(heap != NULL, HEAP_INIT_INVALID_HEAP);

    void* heap_memory = mmap(NULL, initial_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(heap_memory == NULL)
    {
        return -1;
    }

    heap_chunk_t* first = (heap_chunk_t*)heap_memory;
    first->size = (initial_size) - sizeof(heap_chunk_t);
    CLEAR_FLAG(first->flags, CHUNK_USE_MASK);


    heap_state_t tmp = {
        .head = first,
        .size = initial_size,
    };

    atomic_store(&heap->state, tmp);
    heap->memory = heap_memory;

    return HEAP_INIT_SUCCESS;
}

table_id_t salloc(heap_t* heap, size_t size)
{
    ASSERT_RET(heap != NULL, HEAP_ALLOC_INVALID_HEAP);
    ASSERT_RET(size > 0, HEAP_ALLOC_TOO_SMALL);

    table_id_t id;
    get_next_id(heap->master_table, &id);

    if (id < 0)
        return HEAP_ALLOC_NO_IDS;

    heap_state_t current = atomic_load(&heap->state);
    heap_state_t next;

    size_t required = sizeof(heap_chunk_t) + size;

    do
    {
        
        if(current.size < required)
        {
            //if(change_heap_memory(required + INC_BUFFER) == NULL)
            //{
                return HEAP_ALLOC_TOO_SMALL;
            //}
            //printf("INC\n");
        }
        next = current;

        next.head = (void*)current.head + required;

        next.size = (next.size - required); //+ INC_BUFFER;

    } while (!atomic_compare_exchange_weak(
        &heap->state,
        &current,
        next
    ));

    heap_chunk_t* chunk = current.head;

    setup_chunk((void*)chunk, size, id, CHUNK_USE_MASK);

    heap->master_table->entries[id].raw = (void*)chunk + sizeof(heap_chunk_t);

    return id;
}

void setup_chunk(heap_chunk_t* chunk, size_t size, table_id_t id, uint8_t flags)
{
    ATOMIC_SET_FLAG(&chunk->flags, flags);
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
    entry->id = -1;

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
        

        while(current.reference_count == -1);
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