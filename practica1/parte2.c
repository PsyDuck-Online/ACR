#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<dirent.h>

typedef struct param{
    char *nombre;
}param;

typedef struct respuesta
{
    char *nombre;
    int total;

    int n_casa;
    int n_jardin;
    int n_pelota;
    int n_juego;
    int n_amor;
    int n_enojo;

    float p_casa;
    float p_jardin;
    float p_pelota;
    float p_juego;
    float p_amor;
    float p_enojo; 
}respuesta;

void* analizarTexto(void*);
int contarArchivos(char*,char*);

int main(int argc, char const *argv[])
{
    DIR *folder;
    struct dirent *entry;
    pthread_t *hilos;
    param *p;
    char **nombres;
    int n_hilos;
    int i = 0;

    folder = opendir(".");
    if(folder == NULL)
    {
        puts("Error al leer el directorio");
        return(1);
    }

    n_hilos = contarArchivos(".", ".txt");

    folder = opendir(".");
    hilos = (pthread_t*)malloc(n_hilos * sizeof(pthread_t));

    nombres = (char**)malloc(n_hilos * sizeof(char*));

    while((entry = readdir(folder)))
    {
        if(strstr(entry->d_name, ".txt") != NULL)
        {
            nombres[i] = malloc(strlen(entry->d_name) * sizeof(char*));

            strcpy(nombres[i], entry->d_name);

            //printf("%ld | %ld\n", &nombres[i],&entry->d_name);

            pthread_create(&hilos[i], NULL, &analizarTexto, (void*)nombres[i]);
            i++;
        }
    }

    

    // respuesta *r[n_hilos];

    for(i = 0 ; i < n_hilos ; i++)
    {
        pthread_join(hilos[i], NULL);
    }

    // for(i = 0; i < n_hilos ; i++)
    // {
    //     printf("Nombre: %s\n", (char*)r[i]->nombre);
    //     printf("Palabras: %d\n", (int)r[i]->total);
    //     printf("Casa: %d\n", (int)r[i]->n_casa);
    //     printf("------------\n");
    // }

    closedir(folder);
    free(hilos);

    return 0;
}


int contarArchivos(char*direccion, char*tipo)
{
    DIR *folder;
    int n_hilos = 0;
    struct dirent *entry;

    folder = opendir(direccion);
    if(folder == NULL)
    {
        puts("Error al leer el directorio");
        return -1;
    }

    while((entry = readdir(folder)))
    {
        if(strstr(entry->d_name, tipo) != NULL)
        {
            n_hilos++;
        }
    }

    closedir(folder);
    free(entry);

    return n_hilos;
}

void* analizarTexto(void*arg)
{
     char *nombre = (char*)arg;
    //param *p = (param*)arg;
    FILE *archivo = fopen(nombre, "r");
    char palabra_actual[50];
    int n_palabras = 0;
    respuesta *r = (respuesta*)malloc(sizeof(respuesta));

    r->nombre = nombre;
    r->n_casa = 0;
    r->n_jardin = 0;
    r->n_pelota = 0;
    r->n_juego = 0;
    r->n_amor = 0;
    r->n_enojo = 0;

    int n_casa = 0;

    if(archivo != NULL)
    {
        while(!feof(archivo))
        {
            fscanf(archivo, "%s", palabra_actual);
            n_palabras++;

            if(strcasecmp(palabra_actual, "casa") || strcasecmp(palabra_actual, "casa,") || strcasecmp(palabra_actual,"casa."))
            {
                r->n_casa++;
                n_casa++;
            }
        }
    }

    printf("%ld \n", &nombre);
    //printf("%s : %d : %d\n", nombre, n_palabras,n_casa);
    r->total = n_palabras;
    fclose(archivo);

    pthread_exit(NULL);
}