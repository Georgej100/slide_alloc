#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../src/alloc.h"

int main(void)
{
    void* memory = malloc(4096);
    memset(memory, 0, 4096);

    heap_t heap = heap_init(memory, 4096);

    entry_id_t block = heap_alloc(&heap, 10);

    void* raw = dereference(block);    

    char* string = "AAAAAAAAB\0";

    snprintf(raw, 10, "%s", string);

    entry_id_t block2 = heap_alloc(&heap, 10);

    void* raw2 = dereference(block2);    

    snprintf(raw2, 10, "%s", string);



    free(memory);
    return EXIT_SUCCESS;
}
