#include <stdint.h>

#define EXT_SIZE_TOO_LOW 1
#define EXT_MMAP_FAILURE 2

struct heap_chunk_t
{
    struct heap_chunk_t* next;
    uint64_t size;
    char inuse;
};

struct heap_t
{
    struct heap_chunk_t* first;
};


struct heap_t heap_init(uint64_t size);
