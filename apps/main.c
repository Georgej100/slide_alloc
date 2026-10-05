#include <stdlib.h>
#include <stdio.h>
#include "../src/alloc.h"

int main(void)
{
    void* memory = malloc(4096);
    
    heap_t heap = heap_init(memory, 4096);
    
    char* block = heap_alloc(&heap, 10);

    char* string = "AAAAAAAAB\0";    
    snprintf(block, 10, "%s", string);
    
    char* second = heap_alloc(&heap, 10);
    snprintf(second, 10,"%s", string);

    free(memory);
    return EXIT_SUCCESS;
}
