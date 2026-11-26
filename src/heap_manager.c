#include <stdint.h>
#include "alloc.h"

#ifdef USE_STD_SYS

#include <unistd.h>

void* change_heap_memory(heap_t* heap, uint16_t size)
{
    void* got = sbrk(size);
    if (got == -1)
    {
        return -1;
    }
    
    return got;
} 

#endif

#ifdef USE_EMBEDDED_SYS

void* change_heap_memory(heap_t* heap, uint16_t size)
{
    return 0;
}

#endif

