#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

typedef struct efs{
    int entero;
    float flotante;
    char *cadena;
}efs;

void *funcion(void *arg){
    efs *dato = (efs*)arg;

    int res = 21;
    
    printf("(%d)Entero: %d\n", dato->entero, dato->entero);
    printf("(%d)Flotante: %f\n", dato->entero, dato->flotante);
    printf("(%d)Cadena %s\n", dato->entero, dato->cadena);
    

    free(arg);
    pthread_exit((void*)res);
}

int main(int argc, char const *argv[])
{
    int i;
    efs *datos;
    pthread_t *hilos;

    hilos = (pthread_t*)malloc(10 * sizeof(pthread_t));
    
    for(i = 0; i < 10; i++)
    {
        datos = (efs*)malloc(10 * sizeof(efs));
        datos->cadena = "Soy una cadena";
        datos->entero = i;
        datos->flotante = i * .23;
        pthread_create(&hilos[i], NULL, &funcion, (void*)datos);
    }

    void *res[10] ;
    for(i = 0; i < 10; i++)
    {
        pthread_join(hilos[i], &res[0]);
        printf("Retorno : %d\n", (int**)res[0]);
        
    }

    free(hilos);

    return 0;
}

/* 
TAREAS: 
1) Como trabajar con archivos(hilos con archivos y lectura)
2) Como construir una multiplicación de matrices con hilos
*/