#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<dirent.h>

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
    // DEFINICION DE LAS VARIABLES
    DIR *folder;
    struct dirent *entry;
    pthread_t *hilos;
    int n_archivos;
    int i = 0;

    folder = opendir(".");
    if(folder == NULL)
    {
        puts("Error al leer el directorio");
        return(1);
    }

    n_archivos = contarArchivos(".", ".txt"); 

    folder = opendir(".");
    hilos = (pthread_t*)malloc(n_archivos * sizeof(pthread_t));

    
    // CREACION DE HILOS, 1 HILO POR 1 ARCHIVO
    while((entry = readdir(folder)))
    {
        if(strstr(entry->d_name, ".txt") != NULL)
        {
            pthread_create(&hilos[i], NULL, analizarTexto, (void*)entry->d_name);
            i++;
        }
    }

    respuesta *r[n_archivos]; // VARIABLE DE RETORNO DE LOS HILOS

    for(i = 0; i < n_archivos; i++)
    {
        pthread_join(hilos[i], (void*)&r[i]);
    }
    
    // MUESTRA LA INFORMACION QUE RETORNO CADA HILO
    for(i = 0; i < n_archivos; i++)
    {
        printf("%s\n", (char*)r[i]->nombre);
        printf("Total: %d\n", (int)r[i]->total);
        printf("Casa: %d => %f %%\n",(int)r[i]->n_casa, (float)r[i]->p_casa);
        printf("Jardin: %d => %f %%\n", (int)r[i]->n_jardin, (float)r[i]->p_jardin);
        printf("Pelota: %d => %f %%\n",(int)r[i]->n_pelota, (float)r[i]->p_pelota);
        printf("Juego: %d => %f %%\n", (int)r[i]->n_juego, (float)r[i]->p_juego);
        printf("Amor: %d => %f %%\n", (int)r[i]->n_amor, (float)r[i]->p_amor);
        printf("Enojo: %d => %f %%\n", (int)r[i]->n_enojo, (float)r[i]->p_enojo);
        printf("-----------\n");
    }

    closedir(folder);
    free(hilos);

    return 0;
}

// CUENTA LOS ARCHIVOS DEL TIPO ESPECIFICADO EN UNA CARPETA ESPECIFICA
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

// FUNCION LLAMADA EN LOS HILOS
void* analizarTexto(void*arg)
{
    char* nombre_archivo = (char*)arg;
    char pal_actual[50];
    FILE *archivo = fopen(nombre_archivo, "r");
    respuesta *r = (respuesta*)malloc(sizeof(respuesta));
    
    r->nombre = nombre_archivo;
    r->total = 0;
    r->n_amor = 0;
    r->n_enojo = 0;
    r->n_casa = 0;
    r->n_jardin = 0;
    r->n_juego = 0;
    r->n_pelota = 0;

    while(!feof(archivo))
    {
        fscanf(archivo, "%s", pal_actual);
        r->total++;

        if(!strcasecmp(pal_actual, "casa") || !strcasecmp(pal_actual, "casa,") || !strcasecmp(pal_actual, "casa."))
        {
            r->n_casa++;
        }
        if(!strcasecmp(pal_actual, "jardin") || !strcasecmp(pal_actual, "jardin,") || !strcasecmp(pal_actual, "jardin."))
        {
            r->n_jardin++;
        }
        if(!strcasecmp(pal_actual, "pelota") || !strcasecmp(pal_actual, "pelota,") || !strcasecmp(pal_actual, "pelota."))
        {
            r->n_pelota++;
        }
        if(!strcasecmp(pal_actual, "juego") || !strcasecmp(pal_actual, "juego,") || !strcasecmp(pal_actual, "juego."))
        {
            r->n_juego++;
        }
        if(!strcasecmp(pal_actual, "amor") || !strcasecmp(pal_actual, "amor,") || !strcasecmp(pal_actual, "amor."))
        {
            r->n_amor++;
        }
        if(!strcasecmp(pal_actual, "enojo") || !strcasecmp(pal_actual, "enojo,") || !strcasecmp(pal_actual, "enojo."))
        {
            r->n_enojo++;
        }
    }

    

    r->p_casa = r->n_casa * 100;
    r->p_casa /= r->total;

    r->p_jardin = r->n_jardin * 100;
    r->p_jardin /= r->total;

    r->p_pelota = r->n_pelota * 100;
    r->p_pelota /= r->total;
    

    r->p_juego = r->n_juego * 100;
    r->p_juego /= r->total;

    r->p_amor = r->n_amor * 100;
    r->p_amor /= r->total;

    r->p_enojo = r->n_enojo * 100;
    r->p_enojo /= r->total;
    
    fclose(archivo);

    pthread_exit((void*)r);
}