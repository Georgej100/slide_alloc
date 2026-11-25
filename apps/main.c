
#include "stress.h"

int main(void)
{
    stress();

    return 0;    
}


/*
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "libslide.h"

void* worker(void* arg)
{
    
    table_id_t id = salloc((rand() % (100 - 3 + 1) + 3));
    if(id < 0)
    {
        printf("FUCK\n");
    }
    sfree(id);

    id = id;    
    arg = arg;

    return NULL;
}

int main(void)
{
    heap_init(sysconf(_SC_PAGE_SIZE) * 10);

    pthread_t thread[260];

    for (int i = 0; i < 260; i++)
    {
        pthread_create(&thread[i], NULL, worker, NULL);
    }

    for (int i = 0; i < 260; i++)
    {
        pthread_join(thread[i], NULL);
    }

    print_heap();
    
    return 0;    
}
    */