#include <memory.h>
#include "alloc.h"
#include "masks.h"
#include "master_table.h"

int bubble_compact(heap_t* heap)
{
    uint16_t total_free = 0;
    void* heap_pointer = heap->memory;

    while(heap_pointer != heap->head)
    {
        heap_chunk_t* current = (heap_chunk_t*)heap_pointer;
        heap_pointer = heap_pointer + current->size + sizeof(heap_chunk_t);

        if(IS_FLAG(current->flags, CHUNK_USE_MASK) && total_free > 0)
        {
            heap_chunk_t* new = (void*)current - total_free;

            memmove(new, current, sizeof(heap_chunk_t) + current->size);

            table_id_t id = current->id;

            

            heap->master_table->entries[id].raw = new + sizeof(heap_chunk_t);
        }

        if(!IS_FLAG(current->flags, CHUNK_USE_MASK))
        {
            total_free += current->size + sizeof(heap_chunk_t);
        }
    }

    heap->head = (heap_chunk_t *)((uintptr_t)heap->head - total_free);

    return total_free;
}