#include <stdint.h>
#include "alloc.h"

#ifdef USE_STD_SYS

#include <unistd.h>

void* change_heap_memory(uint16_t size)
{
    return sbrk(size);
} 

#endif

#ifdef USE_EMBEDDED_SYS

void* change_heap_memory(uint16_t size)
{
    return 0;
}

#endif

