#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void *nombrar(void *arg)
{
    int *num = (int*)arg;
    printf("Yo soy el hilo numero %d\n", *num);
    
    free(arg);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    int i = 0;
    int *n;
    pthread_t *hilo;
    
    hilo = (pthread_t*)malloc(10 * sizeof(hilo)); 
    

    for (i = 0; i < 10; i++)
    {
        n = (int*)malloc(sizeof(int));
        *n = i;
        pthread_create(&hilo[i],NULL, &nombrar, (void*)n);
    }

    for(i = 0; i < 10; i++){
        pthread_join(hilo[i],NULL);
    }
    
    free(hilo);

    return 0;
}
