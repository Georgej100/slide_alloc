#include <stdint.h>
#include <stddef.h>
#include <threads.h>
#include <stdatomic.h>
#include <unistd.h>
#include <sys/mman.h>
#include "alloc.h"
#include "assert.h"
#include "masks.h"
#include "master_table.h"
#include "globals.h"


heap_t global_heap;
master_table_t global_table_state;
table_entry_t* global_table;

int heap_init(uint16_t size)
{
    ASSERT_RET(size >= sizeof(heap_chunk_t) + 1, HEAP_INIT_LOW_MEMORY);

    void* heap_memory = mmap(NULL, size, (PROT_READ | PROT_WRITE),
        (MAP_PRIVATE | MAP_ANONYMOUS), -1, 0);
    
    if(heap_memory == MAP_FAILED)
    {
        return -1;
    }

    void* table_memory = mmap(NULL, sizeof(table_entry_t) * MASTER_TABLE_SIZE, (PROT_READ | PROT_WRITE),
        (MAP_PRIVATE | MAP_ANONYMOUS), -1, 0);
    
    if (table_memory == MAP_FAILED)
    {
        return -1;
    }

    
    master_table_init(&global_table_state, table_memory, MASTER_TABLE_SIZE);
    global_heap.master_table = &global_table_state;
    global_table = table_memory;

    global_heap.memory = heap_memory;
    

    heap_chunk_t* first = (heap_chunk_t*)heap_memory;
    first->size = (size - sizeof(heap_chunk_t));
    CLEAR_FLAG(first->flags, CHUNK_USE_MASK);


    heap_state_t tmp = {
        .head = first,
        .size = size,
    };

    atomic_store(&global_heap.state, tmp);

    return HEAP_INIT_SUCCESS;
}

table_id_t salloc(uint16_t size)
{
    ASSERT_RET(size > 0, HEAP_ALLOC_TOO_SMALL);

    table_id_t id;
    get_next_id(&id);

    if (id < 0)
        return HEAP_ALLOC_NO_IDS;

    heap_state_t current = atomic_load(&global_heap.state);
    heap_state_t next;

    uint16_t required = sizeof(heap_chunk_t) + size;

    do
    {
        if (current.size < required)
            return HEAP_ALLOC_NO_IDS;

        next = current;

        next.head = (void*)current.head + required;

        next.size -= required;

    } while (!atomic_compare_exchange_weak(
        &global_heap.state,
        &current,
        next
    ));

    heap_chunk_t* chunk = current.head;

    setup_chunk(chunk, size, id, CHUNK_USE_MASK);

    global_table[id].raw = (void*)chunk + sizeof(heap_chunk_t);

    return id;
}

void setup_chunk(heap_chunk_t* chunk, uint16_t size, table_id_t id, uint8_t flags)
{
    chunk->flags = flags;
    chunk->size = size;
    chunk->id = id;
    
}

int sfree(table_id_t id)
{
    controls_t old_master = atomic_load(&global_table[id].controls);
    controls_t new_master = old_master;

    do {
        
        if (!IS_FLAG(old_master.flags, ENTRY_USE_MASK)) {
            return 0; 
        }
        
        if(old_master.reference_count != 0)
        {
            return -1;
        }
        
        new_master = old_master;

        CLEAR_FLAG(new_master.flags, ENTRY_USE_MASK);
        new_master.reference_count = 0;

    } while (!atomic_compare_exchange_weak(&global_table[id].controls, &old_master, new_master));

    heap_chunk_t* entry = (void*)global_table[id].raw - sizeof(heap_chunk_t);
    CLEAR_FLAG(entry->flags, CHUNK_USE_MASK);
    entry->flags = 0;
    entry->id = -1;

    ASSERT_RET(return_id(id) != MASTER_RETURN_UNUSED, -2);

    return 0;
}

void* dereference(table_id_t id)
{
    controls_t current = atomic_load(&global_table[id].controls);
    controls_t new = current;

    do
    {
        if(!IS_FLAG(current.flags, ENTRY_USE_MASK))
        {
            return NULL;
        }
        

        while(current.reference_count == -1);
        new.reference_count += 1;
    } while (!atomic_compare_exchange_weak(&global_table[id].controls, &current, new));

    return global_table[id].raw;
    
}

int exit_dereference(table_id_t id)
{
    controls_t current = atomic_load(&global_table[id].controls);
    controls_t new = current;

    do
    {
        if((current.reference_count < 1) || (!IS_FLAG(current.flags, ENTRY_USE_MASK)))
        {
            return -1;
        }
        new.reference_count -= 1;
    } while (!atomic_compare_exchange_weak(&global_table[id].controls, &current, new));

    return 0;
    
}