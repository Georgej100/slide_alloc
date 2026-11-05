#include <pthread.h>

void* work(void* arg)
{
    arg = arg;
    
    return NULL;
}

int main(void)
{
    pthread_t worker1;

    pthread_create(&worker1, NULL, &work, NULL);
    pthread_join(worker1, NULL);

    


    return 0;    
}


