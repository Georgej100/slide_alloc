#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "libslide.h"

heap_t heap;
master_table_t master_table;

void* worker(void* arg)
{
    
    table_id_t id = salloc(&heap, (rand() % (100 - 3 + 1) + 3));
    
    id = id;    
    arg = arg;

    return NULL;
}

int main(void)
{
    void* heap_memory = mmap(NULL, sysconf(_SC_PAGESIZE) * 4, (PROT_READ | PROT_WRITE),
        (MAP_PRIVATE | MAP_ANONYMOUS), -1, 0);
    
    if(heap_memory == MAP_FAILED)
    {
        exit(EXIT_FAILURE);
    }
    
    heap_init(&heap, heap_memory, sysconf(_SC_PAGESIZE) * 4);

    void* table_memory = mmap(NULL, sizeof(table_entry_t) * 400, (PROT_READ | PROT_WRITE),
        (MAP_PRIVATE | MAP_ANONYMOUS), -1, 0);
    
    if (table_memory == MAP_FAILED)
    {
        exit(EXIT_FAILURE);
    }

    
    master_table_init(&master_table, table_memory, 400);

    heap.master_table = &master_table;


    pthread_t thread[10];

    for (int i = 0; i < 10; i++)
    {
        pthread_create(&thread[i], NULL, worker, NULL);
    }

    for (int i = 0; i < 10; i++)
    {
        pthread_join(thread[i], NULL);
    }

    print_heap(&heap);
    print_master_table(&master_table);
    
    return 0;    
}



/*
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
    table_id_t b2 = salloc(&heap, 3);
    table_id_t b3 = salloc(&heap, 10);

    b1 = b1;
    b2 = b2;
    b3 = b3;

    print_master_table(&table);

    char* x = "AAAAAAAAA\0";
    memmove(dereference(&heap, b1), x, 10);

    char* y = "BB\0";
    memmove(dereference(&heap, b2), y, 3);

    char* z = "CCCCCCCCC\0";
    memmove(dereference(&heap, b3), z, 10);

    print_heap(&heap);

    exit_dereference(&heap, b1);
    exit_dereference(&heap, b2);
    exit_dereference(&heap, b3);

    printf("%d\n", sfree(&heap, b2));

    print_heap(&heap);

    bubble_compact(&heap, 3);

    print_heap(&heap);

    printf("%s", (char*)(dereference(&heap, b1)));
    printf("%s", (char*)(dereference(&heap, b3)));

    free(memory);


    return 0;    
}
*/