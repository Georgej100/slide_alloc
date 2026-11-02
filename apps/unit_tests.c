#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "../src/alloc.h"
#include "../src/master_table.h"
#include "unit_tests.h"
#include "../src/debug.h"
#include "../src/compact.h"

void test_heap_init()
{
    void* memory = malloc(4096);
    table_entry_t* entries = (table_entry_t*)malloc(20 * sizeof(table_entry_t));
    memset(memory, 0, 4096);
    memset(entries, 0, 20);

    master_table_t table = {entries, NULL, 20};
    heap_t heap;

    initialize_master_table(&table);
    initialize_heap(&heap, memory, 4096);
    heap.master_table = &table;

    print_heap(&heap);

    table_id_t b1 = salloc(&heap, 5);
    strcpy(table.entries[b1].raw, "AAAAB");

    table_id_t b2 = salloc(&heap, 5);
    strcpy(table.entries[b2].raw, "CCCCD");
    
    print_heap(&heap);

    sfree(&heap, b1);

    print_heap(&heap);

    bubble_compact(&heap, 3);

    print_heap(&heap);

    free(memory);

}


