#include <stdlib.h>
#include <stdio.h>
#include "libslide.h"
#include "../src/alloc.h"

int main(void)
{
    struct heap_t heap = heap_init(4096);

    printf("First heap chunk @ %x", heap.first);

    return EXIT_SUCCESS;
}
