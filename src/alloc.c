#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "alloc.h"

struct heap_t heap_init(uint64_t size)
{
    if (size <= 0)
    {
        fprintf(stderr, "Size too low\n");
        exit(EXT_SIZE_TOO_LOW);
    }

    void *memory = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (memory == (void*)-1)
    {
        fprintf(stderr, "MMAP failed...");
        exit(EXT_MMAP_FAILURE);
    }


    struct heap_t heap;
    
    heap.first = (struct heap_chunk_t*)memory;
    heap.first->size = size;
    heap.first->inuse = 0;
    heap.first->next = NULL;

    return heap;
}

