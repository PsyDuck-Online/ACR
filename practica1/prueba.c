#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char const *argv[])
{
    FILE *archivo = fopen("quijote.txt","r");
    char palabra_actual[50];
    int cont_casa = 0;

    if(archivo==NULL)
    {  
        printf("Error al abrir el archivo\n");
        exit(1);
    }

    while (!feof(archivo))
    {
        fscanf(archivo, "%s", palabra_actual);

        if(strcmp(palabra_actual,"casa") == 0)
        {
            cont_casa++;
        }
    }
    
    printf("Total de casa: %d\n", cont_casa);

    fclose(archivo);
    return 0;
}
