#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "libslide.h"

heap_t heap;
master_table_t master_table;

void* worker(void* arg)
{
    
    table_id_t id = salloc(&heap, (rand() % (100 - 3 + 1) + 3));
    return_id(&master_table, id);
    
    for (int i = 0; i < 40; i++)
    {
        table_id_t id;
        get_next_id(&master_table, &id);
        id = id;
    }


    
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

    print_master_table(&master_table);
    
    return 0;    
}


