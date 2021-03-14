#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include <pthread.h>

typedef struct res{
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
}res;

void *buscarPalabras(void*);

int main(int argc, char const *argv[])
{
    DIR *folder;
    struct dirent *entry;
    pthread_t *hilos;
    int num_hilos = 0;
    int i;
    
    

    folder = opendir(".");
    if(folder == NULL)
    {
        puts("Error al leer el directorio");
        return(1);
    }
    
    while((entry = readdir(folder)))
    {
        if(strstr(entry->d_name,".txt") != NULL)
        {            
            num_hilos++;
        }   
    }
    closedir(folder);

    folder = opendir(".");
    hilos = (pthread_t*)malloc(num_hilos * sizeof(pthread_t));

    i = 0;
    while((entry = readdir(folder)))
    {
        if(strstr(entry->d_name,".txt") != NULL)
        {
            entry->d_name;

            pthread_create(&hilos[i],NULL,&buscarPalabras,(void*)entry->d_name);
            
            i++;
        }   
    }

    res *resp[num_hilos];
    for(i = 0 ; i < num_hilos ; i++)
    {
        pthread_join(hilos[i],(void**)&resp[i]);
    }


    for(i = 0; i < num_hilos ; i++)
    {
        printf("NOMBRE: %s\n", (char*)resp[i]->nombre);
        printf("TOTAL DE PALABRAS: %d palabras\n", (int)resp[i]->total);
        printf("Casa: %d veces => %f %%\n", (int)resp[i]->n_casa, (float)resp[i]->p_casa);
        printf("Jardin: %d veces => %f %%\n", (int)resp[i]->n_jardin, (float)resp[i]->p_jardin);
        printf("Pelota: %d veces => %f %%\n", (int)resp[i]->n_pelota, (float)resp[i]->p_pelota);
        printf("Juego: %d veces => %f %%\n", (int)resp[i]->n_juego, (float)resp[i]->p_juego);
        printf("Amor: %d veces => %f %%\n", (int)resp[i]->n_amor, (float)resp[i]->p_amor);
        printf("Enojo: %d veces => %f %%\n", (int)resp[i]->n_enojo, (float)resp[i]->p_enojo);
        printf("-------------\n\n");
    }


    free(hilos);
    closedir(folder);
    
    return 0;
}

void* buscarPalabras(void*args)
{

    char *nombre = (char*)args;

    FILE *archivo = fopen(nombre, "r");
    char palabra_actual[50];
    int n_palabras = 0;
    res *resp = (res*)malloc(sizeof(res));

    resp->n_casa = 0;
    resp->n_jardin = 0;
    resp->n_pelota = 0;
    resp->n_juego = 0;
    resp->n_amor = 0;
    resp->n_enojo = 0;

    if(archivo != NULL)
    {
        while(!feof(archivo))
        {
            fscanf(archivo, "%s", palabra_actual);
            n_palabras++;

            if(strcasecmp(palabra_actual, "casa") == 0 || strcasecmp(palabra_actual, "casa,") == 0 || strcasecmp(palabra_actual, "casa."))
            {
                resp->n_casa++;
            }

            if(strcasecmp(palabra_actual, "jardin") == 0 || strcasecmp(palabra_actual, "jardin,") == 0 || strcasecmp(palabra_actual, "jardin.") == 0)
            {
                resp->n_jardin++;
            }

            if(strcasecmp(palabra_actual, "pelota") == 0 || strcasecmp(palabra_actual, "pelota,") == 0 || strcasecmp(palabra_actual, "pelota.") == 0)
            {
                resp->n_pelota++;
            }

            if(strcasecmp(palabra_actual, "juego") == 0 || strcasecmp(palabra_actual, "juego,") == 0 || strcasecmp(palabra_actual, "juego.") == 0)
            {
                resp->n_juego++;
            }

            if(strcasecmp(palabra_actual, "amor") == 0 || strcasecmp(palabra_actual, "amor,") == 0 || strcasecmp(palabra_actual, "amo.") == 0)
            {
                resp->n_amor++;
            }

            if(strcasecmp(palabra_actual, "enojo") == 0 || strcasecmp(palabra_actual, "enojo,") == 0 || strcasecmp(palabra_actual, "enojo.") == 0)
            {
                resp->n_enojo++;
            }
        }
    }
    resp->nombre = nombre;
    resp->total = n_palabras;

    resp->p_casa = resp->n_casa*100;
    resp->p_casa = resp->p_casa / n_palabras;


    resp->p_jardin = resp->n_jardin * 100;
    resp->p_jardin = resp->p_jardin / n_palabras;

    resp->p_pelota = resp->n_pelota * 100;
    resp->p_pelota = resp->p_pelota / n_palabras;

    resp->p_juego = resp->n_juego * 100;
    resp->p_juego = resp->p_juego / n_palabras;

    resp->p_amor = resp->n_amor * 100;
    resp->p_amor = resp->p_amor / n_palabras;

    resp->p_enojo = resp->n_enojo * 100;
    resp->p_enojo = resp->p_enojo / n_palabras;

    fclose(archivo);
    pthread_exit((void*)resp);
}