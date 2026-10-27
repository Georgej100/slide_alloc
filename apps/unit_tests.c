#include <stdlib.h>
#include <memory.h>
#include "../src/alloc.h"
#include "unit_tests.h"

void test_heap_init()
{
    void* memory = malloc(4096);
    memset(memory, 0, 4096);

    heap_t heap;

    TEST(initialize_heap (&heap, NULL, 4096), HEAP_INIT_INVALID_MEMORY, "Heap Invalid Memory");
    TEST(initialize_heap(&heap, memory, 0), HEAP_INIT_LOW_MEMORY, "Heap Low Memory");
    TEST(initialize_heap(NULL, memory, 4096), HEAP_INIT_INVALID_HEAP, "Heap Invalid Heap");


    free(memory);

}


