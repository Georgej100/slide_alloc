#include <memory.h>
#include "alloc.h"
#include "masks.h"
#include "master_table.h"

int bubble_compact(heap_t* heap, unsigned int n)
{
    size_t total_free = 0;
    void* heap_pointer = heap->memory;

    while(n != 0)
    {
        heap_chunk_t* current = (heap_chunk_t*)heap_pointer;
        heap_pointer = heap_pointer + current->size + sizeof(heap_chunk_t);

        if(IS_FLAG(current->flags, CHUNK_USE_MASK) && total_free > 0)
        {
            heap_chunk_t* new = (void*)current - total_free;

            memmove(new, current, sizeof(heap_chunk_t) + current->size);

            CLEAR_FLAG(current->flags, CHUNK_USE_MASK);

            table_id_t id = current->id;
            current->id = -1;

            heap->master_table->entries[id].raw = new + sizeof(heap_chunk_t);
        }

        if(!IS_FLAG(current->flags, CHUNK_USE_MASK))
        {
            total_free += current->size + sizeof(heap_chunk_t);
        }

        n--;
    }

    //heap->head = (heap_chunk_t *)((unsigned char*)heap->head - total_free);

    return total_free;
}