#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "libslide.h"



void* worker(void* arg)
{
    table_id_t ids[10];
    
    for (int i = 0; i < 10; i++)
    {
        ids[i] = salloc(30);
    }
    
    
    for (int i = 0; i < 10; i++)
    {
        //sfree(&heap, ids[i]);
    }
    
    arg = arg;

    *ids = *ids;

    return NULL;
}

int main(void)
{
    heap_init(sysconf(_SC_PAGE_SIZE) * 4);

    table_id_t id = salloc(42);
    id = id;

    pthread_t thread[10];

    for (int i = 0; i < 10; i++)
    {
        pthread_create(&thread[i], NULL, worker, NULL);
    }

    for (int i = 0; i < 10; i++)
    {
        pthread_join(thread[i], NULL);
    }

    print_heap();

    return 0;    
}