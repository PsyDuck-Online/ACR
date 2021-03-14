#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

/* VARIABLES GLOBALES */
int columnasA, columnasB;
int **matriz_A;
int **matriz_B;
int **matriz_C;

/* ESTRUCTURA */ 
typedef struct parametros {
    int id;
    int indice_I;
    int indice_F;
}parametros;


void imprimirMatriz(int**, int , int );
void* resolverHilo(void *);
int **crearMatriz(int , int ); 
void llenarMatriz(int** , int , int );


int main(int argc, char const *argv[])
{
    if(argc != 5){printf("Deben ser 5 parametros\n"); exit(0);}

    srand(time(NULL));

    // MEDICION DE TIEMPO
    time_t begin, end;
    time(&begin);

    // DEFINICION DE VARIABLES 
    int filasA, num_hilos; // PARAMETROS MAIN
    int i, j, k; // CONTADORES
    int cant_filas, filas_restantes;
    pthread_t *hilos; //HILOS
    parametros *p; // PARAMETROS A ENVIAR AL HILO

    filasA = atoi(argv[1]);
    columnasA = atoi(argv[2]);
    columnasB = atoi(argv[3]);
    num_hilos = atoi(argv[4]);

    hilos = (pthread_t*)malloc(num_hilos * sizeof(pthread_t));
    
    

    // CREACION DE MATRICE
    matriz_A = crearMatriz(filasA, columnasA);
    matriz_B = crearMatriz(columnasA, columnasB);
    matriz_C = crearMatriz(filasA, columnasB);

    // LLENADO DE MATRICES
    llenarMatriz(matriz_A, filasA, columnasA);
    llenarMatriz(matriz_B, columnasA, columnasB);

    // printf("MATRIZ A:\n");
    // imprimirMatriz(matriz_A,filasA,columnasA);   

    // printf("---------------\nMATRIZ B:\n");
    // imprimirMatriz(matriz_B, columnasA, columnasB);
    // printf("---------------\n");
    
    // CREACION DE HILOS
    cant_filas = filasA / num_hilos;
    filas_restantes = filasA % num_hilos;

    for(i = 0; i < num_hilos ; i++)
    {
        p = (parametros*)malloc(sizeof(parametros));
        p->id = i; // NUMERO DEL HILO

        p->indice_I = (i * (cant_filas - 1) + i); 
        p->indice_F = p->indice_I + (cant_filas - 1);

        if(i == num_hilos-1)
        {
            p->indice_F = p->indice_F + filas_restantes;
        }

        pthread_create(&hilos[i], NULL, &resolverHilo, (void*)p);
    }

    for(i = 0; i < num_hilos ; i++)
    {
        pthread_join(hilos[i], NULL);
    }

    // printf("---------------\nMATRIZ C:\n");
    // imprimirMatriz(matriz_C, filasA, columnasB);

    time(&end);
    time_t elapsed = end - begin;

    printf("Tiempo medido: %ld segundos\n", elapsed);

    free(hilos);
    return 0;
}

/* FUNCIONES */
void *resolverHilo(void *args)
{
    parametros *p = (parametros*)args;
    int i, j, k;
    int aux;

    

    // printf("(%d) Inicio: %d |Fin: %d\n",p->id, p->indice_I, p->indice_F);
    for(i = p->indice_I ; i <= p->indice_F ; i++)
    {
        for(j = 0 ; j < columnasA ; j++)
        {
            matriz_C[i][j] = 0;
            for(k = 0; k < columnasB; k++)
            {
                matriz_C[i][j] += matriz_A[i][k] * matriz_B[k][j];
            }
        }
    }

    free(args);
    pthread_exit(NULL);
}

int** crearMatriz(int filas, int col) 
{
    int i, j;
    int **matriz_aux = malloc(filas * sizeof(int*));

    for(i = 0; i < filas; i++)
    {
        matriz_aux[i] = malloc(col * sizeof(int));
    }

    return matriz_aux;
}

void llenarMatriz(int** matriz, int filas, int col)
{
    int i, j;
    for(i = 0 ; i < filas ; i++)
    {
        for(j = 0 ; j < col ; j++)
        {
            matriz[i][j] = rand() % 6;
        }
    }
}

void imprimirMatriz(int **matriz, int filas, int col)
{
    
    for(int i = 0; i < filas; i++)
    {
        for(int j = 0; j < col; j++)
        {
            printf("%d | ", matriz[i][j]);
        }
        printf("\n");
    }
}