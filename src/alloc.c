#include <stdint.h>
#include <assert.h>
#include "alloc.h"

table_entry_t master_table[MAX_TABLE_ENTRIES];

void* dereference(entry_id_t id)
{
    return master_table[id].raw;
}

heap_t heap_init(void* memory, uint32_t size)
{
    heap_t heap;

    assert(memory);

    heap.head = memory;
    heap.size = size;

    return heap;
}


entry_id_t heap_alloc(heap_t* heap, uint32_t size)
{
    assert(size < heap->size);

    heap->head += size;
    heap->size -= size;

    void* raw = heap->head - size;
    

    for (int x = 0; x < MAX_TABLE_ENTRIES; x++) {
        if (master_table[x].inuse == 0)
        {
            table_entry_t* entry = &master_table[x];
            
            entry->inuse = 1;
            entry->size = size;
            entry->raw = raw;

            return (entry_id_t)x;
        }
    }

    return (entry_id_t)-1;

}



