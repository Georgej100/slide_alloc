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
    table_id_t ids[100];
    
    for (int i = 0; i < 100; i++)
    {
        ids[i] = salloc(&heap, 10);
        if(ids[i] < 0)
        {
            //printf("A\n");
        }
        printf("%d\n", ids[i]);
        //sfree(&heap, ids[i]);
    }
    printf("Done\n");

    arg = arg;

    *ids = *ids;

    return NULL;
}

int main(void)
{
    heap_init(&heap, 100 * 4096);

    table_entry_t* table_mem = malloc(15000 * sizeof(table_entry_t));
    master_table_init(&master_table, table_mem, 15000);

    heap.master_table = &master_table;

    pthread_t thread[100];

    for (int i = 0; i < 10; i++)
    {
        pthread_create(&thread[i], NULL, worker, NULL);
    }

    for (int i = 0; i < 10; i++)
    {
        pthread_join(thread[i], NULL);
    }

    //print_heap(&heap);
    //print_master_table(&master_table);
    printf("__________________________\n");

    table_id_t id = salloc(&heap, 13);
    id = id;

    //bubble_compact(&heap);
    
    //print_heap(&heap);

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