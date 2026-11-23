#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include "libslide.h"

int main(void)
{
    void* memory = mmap(NULL, sysconf(_SC_PAGESIZE) * 4, (PROT_READ | PROT_WRITE),
        (MAP_PRIVATE | MAP_ANONYMOUS), -1, 0);
    
    if(memory == MAP_FAILED)
    {
        exit(EXIT_FAILURE);
    }
    
    heap_t heap;
    heap_init(&heap, memory, sysconf(_SC_PAGESIZE) * 4);

    

    
    return 0;    
}


