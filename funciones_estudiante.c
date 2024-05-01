#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "estructuras.h"
#include "funciones_estudiante.h"
#include "constantes.h"
/*
    Integrantes del grupo. En caso de ser un grupo de dos integrantes, no completar el último campo.
    Si alguno de los integrantes del grupo dejara la materia, completar de todos modos sus datos, aclarando que no entrega.
    -----------------
    Apellido: Checcacci
    Nombre: Franco
    DNI:43.386.577
    Entrega: SI.
    -----------------
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------
    (Sólo para grupos de tres integrantes)
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------

    Comentarios (opcionales) que deseen hacer al docente sobre el TP:

*/

/* Bitmap file format
 *
 * SECTION
 * Address:Bytes	Name
 *
 * HEADER:
 *	  0:	2		"BM" magic number
 *	  2:	4		file size
 *	  6:	4		junk
 *	 10:	4		Starting address of image data
 * BITMAP HEADER:
 *	 14:	4		header size
 *	 18:	4		width  (signed)
 *	 22:	4		height (signed)
 *	 26:	2		Number of color planes
 *	 28:	2		Bits per pixel
 *	[...]
 * [OPTIONAL COLOR PALETTE, NOT PRESENT IN 32 BIT BITMAPS]
 * BITMAP DATA:
 *	DATA:	X	Pixels
 */

void solucion(int argc, char* argv[])
{
    // agregar lectura de argumentos.
    t_pixel img;
    t_metadata header;

    //declarar y que la funcion retorne header e imagen
    openBmpFile(argv, &img, &header);




    /*
        Aquí deben hacer el código que solucione lo solicitado.
        Todas las funciones utilizadas deben estar declaradas en este archivo, y en su respectivo .h
    */
}

void openBmpFile(char* argv[], t_pixel *img, t_metadata *header)
{
    int i = 1;
    bool contieneImagen = false;
    int contImagenes = 0;
    while(argv[i])
    {
        if(strstr(argv[i], "--") == 0)
        {
            contieneImagen = true;
            contImagenes++;
        }
        i++;
    }

    if(contImagenes > 1)
    {
        printf("\nSe debe pasar solo una imagen como argumento.\n");
        return;
    }

    i = 1;
    if(contieneImagen)
    {
        while(argv[i])
        {
            if(strstr(argv[i], "--") == 0)
            {
                //abro el archivo
                FILE *pf;
                strcat(argv[i], ".bmp");
                pf = fopen(argv[i], "rb");
                if (pf == NULL)
                {
                    printf("No se pudo abrir el archivo.");
                    return;
                }

                if(getc(pf) == 'B' && getc(pf) == 'M')
                {
                    fread(&header->tamArchivo, sizeof(unsigned int), 1, pf);
                    fseek(pf, 4, SEEK_CUR);
                    fread(&header->comienzoImagen, sizeof(unsigned int), 1, pf);

                    fread(&header->tamEncabezado, sizeof(unsigned int), 1, pf);
                    fread(&header->ancho, sizeof(unsigned int), 1, pf);
                    fread(&header->alto, sizeof(unsigned int), 1, pf);
                    fseek(pf, 2, SEEK_CUR);
                    fread(&header->profundidad, sizeof(unsigned short), 1, pf);

                    printf("\n tam arch: %d \n comienzo: %d \n tam enca: %d \n ancho: %d \n alto: %d \n prof %d ",
                           header->tamArchivo, header->comienzoImagen, header->tamEncabezado, header->ancho,
                           header->alto, header->profundidad
                          );

                    img = (t_pixel *)malloc(header->alto * header->ancho * sizeof(t_pixel));
                    t_pixel *ini = img;
                    if(img == NULL)
                    {
                        printf("\n No se pudo reservar memoria para la imagen.");
                        return;
                    }

                    fseek(pf, header->comienzoImagen, SEEK_SET);
                    for( int i=0; i< header->alto; i++)
                    {
                        for(int j=0; j<header->ancho; j++)
                        {
                            fread(img[i*header->ancho+j].pixel, sizeof(unsigned char), 3, pf);
                        }
                    }

                    printf("Primer pixel: R=%u, G=%u, B=%u\n", img[0].pixel[0], img[0].pixel[1], img[0].pixel[2]);

                    definirEfecto(argv, header, img);
                    img = ini;
                    free(img);
                    fclose(pf);
                }
                else
                {
                    printf("\n El archivo indicado no es de tipo bit map.");
                }
            }

            i++;
        }
    }
    else
    {
        printf("No recibe imagen");
        return;
    }
}

// funcion que convierte la imagen a escala de grises
int escalaDeGrises(t_pixel *imagen, t_metadata *header, char nombre[])
{
    t_pixel * imgCopy = malloc(sizeof(t_pixel)*header->alto*header->ancho);
    memcpy(imgCopy, imagen, sizeof(t_pixel)*header->alto*header->ancho);

    for (int i = 0; i < header->alto * header->ancho; i++)
    {
        unsigned char promedio = (imgCopy[i].pixel[0] + imgCopy[i].pixel[1] + imgCopy[i].pixel[2]) / 3;
        for (int j = 0; j < 8; j++)
        {
            imgCopy[i].pixel[j] = promedio;
        }
    }

    crearBmpSalida(imgCopy, header, nombre);
    free(imgCopy);

    return 0;
}

int imgNegativa(t_pixel *imagen, t_metadata * header, char nombre[])
{
    t_pixel * imgCopy = malloc(sizeof(t_pixel)*header->alto*header->ancho);
    memcpy(imgCopy, imagen, sizeof(t_pixel)*header->alto*header->ancho);

    for (int i = 0; i < header->alto * header->ancho; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            imgCopy[i].pixel[j] = 255 - imgCopy[i].pixel[j];
        }
    }

    crearBmpSalida(imgCopy, header, nombre);
    free(imgCopy);
    return 0;
}

int aumentar25Contraste(t_pixel *imagen, t_metadata *header, char nombre[])
{
    float porcentaje = 1.25;
    float valor_original = 0;
    float valor_nuevo = 0;

    t_pixel * imgCopy = malloc(sizeof(t_pixel)*header->alto*header->ancho);
    memcpy(imgCopy, imagen, sizeof(t_pixel)*header->alto*header->ancho);

    for (int i = 0; i < header->alto * header->ancho; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            valor_original = imgCopy[i].pixel[j];
            valor_nuevo = valor_original * porcentaje;
            if(valor_nuevo < 0)
            {
                valor_nuevo = 0;
            }
            else if(valor_nuevo > 255)
            {
                valor_nuevo = 255;
            }
            imgCopy[i].pixel[j] = (unsigned char)valor_nuevo;
        }
    }
    crearBmpSalida(imgCopy, header, nombre);
    free(imgCopy);
    return 0;
}

int reducir25Contraste(t_pixel *imagen, t_metadata *header, char nombre[])
{
    t_pixel * imgCopy = malloc(sizeof(t_pixel)*header->alto*header->ancho);
    memcpy(imgCopy, imagen, sizeof(t_pixel)*header->alto*header->ancho);

    float porcentaje = 0.25;
    float valor_original = 0;
    float valor_nuevo = 0;

    for (int i = 0; i < header->alto * header->ancho; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            valor_original = imgCopy[i].pixel[j];
            valor_nuevo = valor_original * porcentaje;
            valor_nuevo = valor_original - valor_nuevo;
            if(valor_nuevo < 0)
            {
                valor_nuevo = 0;
            }
            else if(valor_nuevo > 255)
            {
                valor_nuevo = 255;
            }
            imgCopy[i].pixel[j] = (unsigned char)valor_nuevo;
        }
    }

    crearBmpSalida(imgCopy, header, nombre);
    free(imgCopy);
    return 0;
}

// funcion que cree un archivo de salida
int crearBmpSalida(t_pixel *imagen, t_metadata *header, char nombre[])
{
    FILE *pf;
    pf = fopen(nombre, "wb");

    if(!nombre)
    {
        printf("Error al leer el argumento");
        return 1;
    }

    if (pf == NULL)
    {
        printf("No se pudo abrir el archivo.");
        return 1;
    }

    fwrite("BM", sizeof(char), 2, pf);
    fwrite(&header->tamArchivo, sizeof(unsigned int), 1, pf);
    fseek(pf, 4, SEEK_CUR);
    fwrite(&header->comienzoImagen, sizeof(unsigned int), 1, pf);
    fwrite(&header->tamEncabezado, sizeof(unsigned int), 1, pf);
    fwrite(&header->ancho, sizeof(unsigned int), 1, pf);
    fwrite(&header->alto, sizeof(unsigned int), 1, pf);
    fseek(pf, 2, SEEK_CUR);
    fwrite(&header->profundidad, sizeof(unsigned short), 1, pf);
    fseek(pf, header->comienzoImagen, SEEK_SET);
    for( int i=0; i< header->alto; i++)
    {
        for(int j=0; j<header->ancho; j++)
        {
            fwrite(imagen[i*header->ancho+j].pixel, sizeof(unsigned char), 3, pf);
        }
    }
    fclose(pf);

    return TODO_OK;
}

void definirEfecto(char* argv[], t_metadata* header, t_pixel* img)
{
    int i = 1;
    while(argv[i])
    {
        if (strcmp(argv[i], "--negativo") == 0)
        {
            imgNegativa(img, header, "negativo.bmp");
        }
        else if (strcmp(argv[i], "--escala-de-grises") == 0)
        {
            escalaDeGrises(img, header, "escala-de-grises.bmp");
        }
        else if (strcmp(argv[i], "--aumentar-contraste") == 0)
        {
            aumentar25Contraste(img, header, "aumentar-contraste.bmp");
        }
        else if (strcmp(argv[i], "--reducir-contraste") == 0)
        {
            reducir25Contraste(img, header, "reducir-contraste.bmp");
        }
        else if(strstr(argv[i], "--") != 0)
        {
            printf("\nArgumento no reconocido: %s\n\n", argv[i]);
        }
        i++;
    }
}

