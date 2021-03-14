#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<sys/time.h>

int columnasA, columnasB;

typedef struct parametros {
    int id;
    int **A;
    int **B;
    int **C;
    int inicio_fila;
    int fin_fila;
}parametros;

/* IMPRIME UNA MATRIZ RECIBIENDO COMO PARAMETRO LA MATRIZ, SUS FILAS Y COLUMNAS */
void imprimirMatriz(int **matriz, int filas, int col)
{
    
    for(int i = 0; i < filas; i++)
    {
        for(int j = 0; j < col; j++)
        {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}

void* resolverHilo(void *args)
{
    parametros *p = (parametros*)args;
    int i, j, k;
    int aux;

    printf("(%d)inicio: %d |fin: %d |columnasA: %d |columnasB: %d\n",p->id, p->inicio_fila, p->fin_fila, columnasA, columnasB);

    for(i = p->inicio_fila; i <= p->fin_fila ; i++)
    {
        for(j = 0 ; j < columnasA ; j++)
        {
            aux = 0;
            for(k = 0; k < columnasB; k++)
            {
                aux += p->A[i][k] * p->B[k][j];
            }

            p->C[i][j] = aux;

            // printf("C[%d][%d] = %d\n", i,j,aux);
        }
    }
    free(args);
}

/* DEVUELVE UN APUNTADOR A UNA MATRIZ RECIBE LAS FILAS Y COLUMNAS COMO PARAMETRO*/
int **crearMatriz(int filas, int col) 
{
    int i, j;
    int **matriz_aux = (int**)malloc(filas * sizeof(int*));

    for(i = 0; i < filas; i++)
    {
        matriz_aux[i] = (int*)malloc(col * sizeof(int));
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


int main(int argc, char const *argv[])
{
    if(argc != 5){printf("Deben ser 5 parametros\n"); exit(0);}

    srand(time(NULL));
    //MEDICION DE TIEMPO
    struct timeval ti, tf;
    double tiempo;

    gettimeofday(&ti, NULL);

    /* DEFINICION DE VARIABLES */
    int filasA, num_hilos; // PARAMETROS MAIN
    int **A, **B, **C; // MATRICES
    int i, j, k; // CONTADORES
    int indice_filas = 0, cant_filas, filas_restantes;
    pthread_t *hilos; //HILOS
    parametros *p; // PARAMETROS A ENVIAR AL HILO

    filasA = filas_restantes = atoi(argv[1]);
    columnasA = atoi(argv[2]);
    columnasB = atoi(argv[3]);
    num_hilos = atoi(argv[4]);

    hilos = (pthread_t*)malloc(num_hilos * sizeof(pthread_t));
    
    

    /* CREACION DE MATRICES */
    A = crearMatriz(filasA, columnasA);
    B = crearMatriz(columnasA, columnasB);
    C = crearMatriz(filasA, columnasB);

    /* LLENADO DE MATRICES */

    llenarMatriz(A, filasA, columnasA);

    printf("MATRIZ A:\n");
    imprimirMatriz(A,filasA,columnasA);

    llenarMatriz(B, columnasA, columnasB);

    printf("---------------\nMATRIZ B:\n");
    imprimirMatriz(B, columnasA, columnasB);
    
    // CREACION DE HILOS
    for(i = 0 ; i < num_hilos ; i++)
    {
        p = (parametros*)malloc(sizeof(parametros));
        p->A = A;
        p->B = B;
        p->C = C;
        p->id = i;

        cant_filas = 0;
        cant_filas = filas_restantes / (num_hilos - i); // filas restantes - hilos restantes
        filas_restantes -= cant_filas;

        p->inicio_fila = indice_filas;
        indice_filas += cant_filas - 1;
        p->fin_fila = indice_filas;
        
        indice_filas++;

        pthread_create(&hilos[i], NULL, &resolverHilo, (void*)p);
    }

    for(i = 0; i < num_hilos; i++)
    {
        pthread_join(hilos[i], NULL);
    }

    printf("-------------\nMATRIZ C:\n");
    imprimirMatriz(C, filasA, columnasB);
    

    free(hilos);

    gettimeofday(&tf, NULL);
    tiempo = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000.0;

    printf("Duracion: %g milisegundos\n", tiempo);
    return 0;
}

// CREAR MATRIZ DE MANERA DINAMICA A,B,C
// MATRIZ A SE LLENA CON VALORES ALEATORIOS ENTRE 0 Y 5
// CREACION DE HILOS
// QUE INFORMACION DEBEMOS ENVIARLE A CADA HILO PARA QUE PUEDA RESOLVER UNA PARTE DE LA MATRIZ