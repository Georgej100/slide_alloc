#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "../src/alloc.h"
#include "../src/master_table.h"
#include "unit_tests.h"

void test_heap_init()
{
    void* memory = malloc(4096);
    table_entry_t* entries = (table_entry_t*)malloc(20 * sizeof(table_entry_t));
    memset(memory, 0, 4096);
    memset(entries, 0, 20);

    master_table_t table = {entries, NULL, 20};
    heap_t heap;

    TEST(initialize_heap (&heap, NULL, 4096), HEAP_INIT_INVALID_MEMORY, "Heap Invalid Memory");
    TEST(initialize_heap(&heap, memory, 0), HEAP_INIT_LOW_MEMORY, "Heap Low Memory");
    TEST(initialize_heap(NULL, memory, 4096), HEAP_INIT_INVALID_HEAP, "Heap Invalid Heap");

    initialize_master_table(&table);

    TEST(entries[0].next, &entries[1], "Table linking");
    TEST(entries[19].next, NULL, "Table End"); 


    initialize_heap(&heap, memory, 4096);
    heap.master_table = &table;

    table_id_t b1 = alloc(&heap, 5);
    strcpy(table.entries[b1].raw, "AAAAB");

    table_id_t b2 = alloc(&heap, 5);
    strcpy(table.entries[b2].raw, "CCCCD");
    
    free(memory);

}


