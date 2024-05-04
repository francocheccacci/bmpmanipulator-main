#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "estructuras.h"
#include "funciones_estudiante.h"
#include "constantes.h"
#define ERROR_MULT_IMG 30
#define ERROR_MEMORIA 40
#define ERROR_NO_BMP 50
#define ERROR_NO_IMG 60

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

    int resultado = 0;
    resultado = openBmpFile(argv, &img, &header);

    switch (resultado)
    {
    case TODO_OK:
        printf("\n\n *** Ha finalizado el proceso de ejecución. *** \n\n");
        break;
    case ERROR_MULT_IMG:
        printf("\n\nError: Se debe pasar solo una imagen como argumento.\n\n");
        break;
    case ERROR_MEMORIA:
        printf("\n\nError: No se pudo reservar memoria para la imagen.\n\n");
        break;
    case ERROR_NO_BMP:
        printf("\n\nError: El archivo indicado no es de tipo bit map.\n\n");
        break;
    case ERROR_NO_IMG:  
        printf("\n\nError: No se recibe imagen.\n\n");
        break;
    }

    /*
        Aquí deben hacer el código que solucione lo solicitado.
        Todas las funciones utilizadas deben estar declaradas en este archivo, y en su respectivo .h
    */
}

int openBmpFile(char* argv[], t_pixel *img, t_metadata *header)
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
        //printf("\nSe debe pasar solo una imagen como argumento.\n");
        return ERROR_MULT_IMG;
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

                if(strstr(argv[i], ".bmp") == NULL)
                {
                    strcat(argv[i], ".bmp");
                }

                pf = fopen(argv[i], "rb");

                if (pf == NULL)
                {
                    printf("No se pudo abrir el archivo.");
                    return ARCHIVO_NO_ENCONTRADO;
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
                        //printf("\n No se pudo reservar memoria para la imagen.");
                        return ERROR_MEMORIA;
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
                    //printf("\n El archivo indicado no es de tipo bit map.");
                    return ERROR_NO_BMP;
                }
            }

            i++;
        }
    }
    else
    {
        //printf("No recibe imagen");
        return ERROR_NO_IMG;
    }
    return TODO_OK;
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

    return TODO_OK;
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
    return TODO_OK;
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
    return TODO_OK;
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
    return TODO_OK;
}

int aumentar50red(t_pixel *imagen, t_metadata *header, char nombre[])
{

    t_pixel * imgCopy = malloc(sizeof(t_pixel)*header->alto*header->ancho);
    memcpy(imgCopy, imagen, sizeof(t_pixel)*header->alto*header->ancho);
    int valor = 0;
    for (int i = 0; i < header->alto * header->ancho; i++)
    {
        valor = imgCopy[i].pixel[2] + (imgCopy[i].pixel[2] / 2);
        imgCopy[i].pixel[2] = (valor > 255) ? 255 : valor;
    }

    crearBmpSalida(imgCopy, header, nombre);
    free(imgCopy);
    return TODO_OK;
}

int aumentar50blue(t_pixel *imagen, t_metadata *header, char nombre[])
{

    t_pixel * imgCopy = malloc(sizeof(t_pixel)*header->alto*header->ancho);
    memcpy(imgCopy, imagen, sizeof(t_pixel)*header->alto*header->ancho);

    int valor = 0;
    for (int i = 0; i < header->alto * header->ancho; i++)
    {
        valor = imgCopy[i].pixel[0] + (imgCopy[i].pixel[0] / 2);
        imgCopy[i].pixel[0] = (valor > 255) ? 255 : valor;
    }

    crearBmpSalida(imgCopy, header, nombre);
    free(imgCopy);
    return TODO_OK;
}

int aumentar50green(t_pixel *imagen, t_metadata *header, char nombre[])
{

    t_pixel * imgCopy = malloc(sizeof(t_pixel)*header->alto*header->ancho);
    memcpy(imgCopy, imagen, sizeof(t_pixel)*header->alto*header->ancho);
    int valor = 0;
    for (int i = 0; i < header->alto * header->ancho; i++)
    {
        valor = imgCopy[i].pixel[1] + (imgCopy[i].pixel[1] / 2);
        imgCopy[i].pixel[1] = (valor > 255) ? 255 : valor;
    }

    crearBmpSalida(imgCopy, header, nombre);
    free(imgCopy);
    return TODO_OK;

}

int recortar50(t_pixel *imagen, t_metadata *header, char nombre[])
{

    t_pixel * imgCopy = malloc(sizeof(t_pixel)*header->alto*header->ancho);
    memcpy(imgCopy, imagen, sizeof(t_pixel)*header->alto*header->ancho);

    t_metadata * headerCopy = malloc(sizeof(t_metadata));
    memcpy(headerCopy, header, sizeof(t_metadata));

    // definimos el nuevo tamaño de la imagen
    int ancho = header->ancho / 2;
    int alto = header->alto / 2;

    // reservamos memoria para la nueva imagen
    t_pixel *imgReco = malloc(ancho * alto * sizeof(t_pixel));

    // copiamos los pixeles de la imagen original a la nueva imagen
    for (int i = 0; i < alto; i++)
    {
        for (int j = 0; j < ancho; j++)
        {
            // copiamos los pixeles de la imagen original a la nueva imagen
            imgReco[i * ancho + j] = imgCopy[i * header->ancho + j];
        }
    }

    //modificamos el header de la imagen
    headerCopy->alto = alto;
    headerCopy->ancho = ancho;

    //creamos la imagen de salida
    crearBmpSalida(imgReco, headerCopy, nombre);

    free(imgReco);
    free(imgCopy);
    free(headerCopy);

    return TODO_OK;
}

int rotar90izquierda(t_pixel *imagen, t_metadata *header, char nombre[])
{

    t_pixel * imgCopy = malloc(sizeof(t_pixel)*header->alto*header->ancho);
    memcpy(imgCopy, imagen, sizeof(t_pixel)*header->alto*header->ancho);

    t_metadata * headerCopy = malloc(sizeof(t_metadata));
    memcpy(headerCopy, header, sizeof(t_metadata));

// definimos el nuevo tamaño de la imagen
    int ancho = header->alto;
    int alto = header->ancho;
    t_pixel* ini = imagen;
    // reservamos memoria para la nueva imagen
    t_pixel * imgRot = malloc(ancho * alto * sizeof(t_pixel));

    // copiamos los pixeles de la imagen original a la nueva imagen
    for (int i = 0; i < header->alto; i++)
    {
        for (int j = 0; j < header->ancho; j++)
        {
            //j * ancho avanza en las columnas
            //header->alto - i - 1 | calcula la fila en la imagen rotada
            //me posiciono en la columna con j * ancho
            imgRot[j * ancho + (header->alto - i - 1)] = imagen[i * header->ancho + j];
        }
    }

    imagen = ini;
    //modificamos el header de la imagen
    headerCopy->alto = alto;
    headerCopy->ancho = ancho;

    //creamos la imagen de salida
    crearBmpSalida(imgRot, headerCopy, nombre);

    free(imgRot);
    free(imgCopy);
    free(headerCopy);

    return TODO_OK;

}

int rotar90derecha(t_pixel *imagen, t_metadata *header, char nombre[])
{

    t_pixel * imgCopy = malloc(sizeof(t_pixel)*header->alto*header->ancho);
    memcpy(imgCopy, imagen, sizeof(t_pixel)*header->alto*header->ancho);

    t_metadata * headerCopy = malloc(sizeof(t_metadata));
    memcpy(headerCopy, header, sizeof(t_metadata));

    // definimos el nuevo tamaño de la imagen
    int ancho = header->alto;
    int alto = header->ancho;

    // reservamos memoria para la nueva imagen
    t_pixel *imgRot = malloc(ancho * alto * sizeof(t_pixel));

    // copiamos los pixeles de la imagen original a la nueva imagen
    for (int i = 0; i < header->alto; i++)
    {
        for (int j = 0; j < header->ancho; j++)
        {
            // imagen[i * header->ancho + j] es la posicion (i,j) de la imagen original.
            // header->ancho es la altura de la imagen y j representa la coordenada Y
            // en la imagen rotada i es mi eje X

            imgRot[ (header->ancho - j - 1) * ancho + i] = imgCopy[i * header->ancho + j];
            // a la altura de la imagen le resto el valor de j para posicionarme en el pixel de la imagen rotada
        }
    }

    headerCopy->alto = alto;
    headerCopy->ancho = ancho;

    crearBmpSalida(imgRot, headerCopy, nombre);

    free(imgRot);
    free(imgCopy);
    free(headerCopy);
    return TODO_OK;

}

int combinarImagenes(t_pixel *imagen1, t_metadata *header1, char nombre[])
{

    t_pixel *img2 = NULL;
    t_metadata *header2 = NULL;
    header2 = malloc(sizeof(t_metadata));
    printf("hola.\n");

    printf("Ingrese nombre del segundo archivo para combinar: (sin la extenstion .bmp) \n");
    char nombreArchivo[50];
    scanf("%s", nombreArchivo);

    FILE *pf_img2 = fopen(strcat(nombreArchivo, ".bmp"), "rb");
    if (!pf_img2)
    {
        printf("No se pudo abrir el archivo.\n");
        return 1;
    }

    float variador = 0;
    do
    {
        printf("Ingrese el numero de variador comprendido entre 0 y 1: \n");
        scanf("%f", &variador);
    }
    while(variador < 0 || variador > 1);


    if(getc(pf_img2) == 'B' && getc(pf_img2) == 'M')  // aca estoy parado en el byte 2
    {
        fread(&header2->tamArchivo, sizeof(unsigned int), 1, pf_img2);
        fseek(pf_img2, 4, SEEK_CUR); // me muevo 4 bytes, para posicionarme en el byte 10
        // este byte nro 10 contiene el inicio de la imagen
        fread(&header2->comienzoImagen, sizeof(unsigned int), 1, pf_img2);// hago el fread, como es unsigned int ocupa 4bytes,
        // y el puntero queda en el byte 14, donde se encuentra el tamanio del header
        fread(&header2->tamEncabezado, sizeof(unsigned int), 1, pf_img2);
        fread(&header2->ancho, sizeof(unsigned int), 1, pf_img2);
        fread(&header2->alto, sizeof(unsigned int), 1, pf_img2);

        if (header1->alto != header2->alto || header1->ancho != header2->ancho)
        {
            printf("Las imagenes no tienen el mismo tamaño, por lo tanto, no se pueden combinar.\n");
            free(img2);
            free(header2);
            fclose(pf_img2);
            return 1;

        }

        fseek(pf_img2, 2, SEEK_CUR);
        fread(&header2->profundidad, sizeof(unsigned short), 1, pf_img2);

        // leo la imagen
        img2 = (t_pixel *)malloc(header2->alto * header2->ancho * sizeof(t_pixel));
        if(img2 == NULL)
        {
            printf("\n No se pudo reservar memoria para la imagen.");
            return 1;
        }

        fseek(pf_img2, header2->comienzoImagen, SEEK_SET);
        for( int i=0; i< header2->alto; i++)
        {
            for(int j=0; j<header2->ancho; j++)
            {
                fread(img2[i*header2->ancho+j].pixel, sizeof(unsigned char), 3, pf_img2);
            }
        }

        fclose(pf_img2);
        unsigned char r, g, b;
        t_pixel *imgCombinada = malloc(header1->alto * header1->ancho * sizeof(t_pixel));
        for (int i = 0; i < header1->alto * header1->ancho; i++)
        {
            // copiamos los pixeles de la imagen original a la nueva imagen
            b = ( ((1 - variador) * imagen1[i].pixel[0]) + img2[i].pixel[0] ) / 2;
            g = ( ((1 - variador) * imagen1[i].pixel[1]) + img2[i].pixel[1] ) / 2;
            r = ( ((1 - variador) * imagen1[i].pixel[2]) + img2[i].pixel[2] ) / 2;
            imgCombinada[i].pixel[0] = b;
            imgCombinada[i].pixel[1] = g;
            imgCombinada[i].pixel[2] = r;

        }
        crearBmpSalida(imgCombinada, header1, nombre);
    }
    else
    {
        printf("\n El archivo indicado no es de tipo bit map.");
    }

    free(img2);
    free(header2);

    return TODO_OK;
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
        else if (strcmp(argv[i], "--tonalidad-roja") == 0)
        {
            aumentar50red(img, header, "tonalidad-roja.bmp");
        }
        else if (strcmp(argv[i], "--tonalidad-verde") == 0)
        {
            aumentar50green(img, header, "tonalidad-verde.bmp");
        }
        else if (strcmp(argv[i], "--tonalidad-azul") == 0)
        {
            aumentar50blue(img, header, "tonalidad-azul.bmp");
        }
        else if (strcmp(argv[i], "--recortar") == 0)
        {
            recortar50(img, header, "recortar.bmp");
        }
        else if (strcmp(argv[i], "--rotar-derecha") == 0)
        {
            rotar90derecha(img, header, "rotar-derecha.bmp");
        }
        else if (strcmp(argv[i], "--rotar-izquierda") == 0)
        {
            rotar90izquierda(img, header, "rotar-izquierda.bmp");
        }
        else if (strcmp(argv[i], "--comodin") == 0)
        {
            combinarImagenes(img, header, "comodin.bmp");
        }
        else if(strstr(argv[i], "--") != 0)
        {
            printf("\nArgumento no reconocido: %s\n\n", argv[i]);
        }
        i++;
    }
}

