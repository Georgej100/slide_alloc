#include <pthread.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "stress.h"
#include "libslide.h"

/*
https://www.geeksforgeeks.org/c/time-delay-c/
*/
void delay(int number_of_seconds)
{
    // Storing start time
    clock_t start_time = clock();

    // Looping till required time is not achieved
    while (clock() - start_time < number_of_seconds * CLOCKS_PER_SEC);
}

/*
Derived from 
https://6it.dev/blog/testing-memory-allocators-ptmalloc2-vs-tcmalloc-vs-hoard-vs-jemalloc-while-trying-to-simulate-real-world-loads-776
*/
size_t random_allocation_size(size_t min_size, size_t max_size) {
    // Generate a uniform random double in [0.0, 1.0]
    double u = (double)rand() / (double)RAND_MAX;
    
    // Map uniformly in log-space, then exponentiate
    double log_min = log((double)min_size);
    double log_max = log((double)max_size);
    double size = exp(log_min + u * (log_max - log_min));
    
    return (size_t)size;
}

void* worker(void* arg)
{
    arg = arg;
    
    UNIT slots[(TOTAL_ALLOCATIONS / THREAD_COUNT)];
    uint num_slots = (TOTAL_ALLOCATIONS / THREAD_COUNT);

    while(1)
    {
        int id = rand() % (num_slots - 0 + 1) + 0;

        if(slots[id] == -1)
        {
            slots[id] = ALLOC(random_allocation_size(1, UINT16_MAX));
        }
        else
        {
            FREE(slots[id]);
            slots[id] = -1;
        }
    }

    return NULL;
}

int stress(void)
{
    srand(RNG_SEED);

    if(heap_init(sysconf(_SC_PAGE_SIZE) * 10) == -1)
    {
        exit(EXIT_FAILURE);
    }

    print_master_free();

    

    pthread_t thread[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_create(&thread[i], NULL, worker, NULL);
    }

    while(1)
    {
        delay(2);
        bubble_compact();
    }

    
}