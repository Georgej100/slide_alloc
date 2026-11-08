#include <pthread.h>
#include <stdlib.h>
#include "debug.h"
#include "alloc.h"
#include "master_table.h"
#include "memory.h"
#include "compact.h"

int main(void)
{
   

    void* memory = malloc(4096);
    table_entry_t* entries = (table_entry_t*)malloc(20 * sizeof(table_entry_t));
    memset(memory, 0, 4096);
    memset(entries, 0, 20);

    master_table_t table;
    heap_t heap;

    

    master_table_init(&table, entries, 20);
    heap_init(&heap, memory, 4096);
    heap.master_table = &table;

    table_id_t b1 = salloc(&heap, 10);
    table_id_t b2 = salloc(&heap, 10);
    table_id_t b3 = salloc(&heap, 10);

    b1 = b1;
    b2 = b2;
    b3 = b3;


    print_heap(&heap);

    sfree(&heap, b2);

    print_heap(&heap);

    bubble_compact(&heap, 3);

    print_heap(&heap);

    free(memory);


    return 0;    
}


