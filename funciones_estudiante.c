#include <stdio.h>
#include <stdlib.h>
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

void solucion(int argc, char* argv[]){
    // agregar lectura de argumentos.
    t_pixel img;
    t_metadata header;



    //declarar y que la funcion retorne header e imagen
    openBmpFile(&img, &header);



    //free(img);
    /*
        Aquí deben hacer el código que solucione lo solicitado.
        Todas las funciones utilizadas deben estar declaradas en este archivo, y en su respectivo .h
    */
}

void openBmpFile(t_pixel *img, t_metadata *header){
    FILE *pf;
    //abro el archivo
    pf = fopen("unlam.bmp", "rb");

    if (pf == NULL) {
        printf("No se pudo abrir el archivo.");
        return;
    }

    if(getc(pf) == 'B' && getc(pf) == 'M'){ // aca estoy parado en el byte 2
        fread(&header->tamArchivo, sizeof(unsigned int), 1, pf);
        fseek(pf, 4, SEEK_CUR); // me muevo 4 bytes, para posicionarme en el byte 10
                                // este byte nro 10 contiene el inicio de la imagen
        fread(&header->comienzoImagen, sizeof(unsigned int), 1, pf);// hago el fread, como es unsigned int ocupa 4bytes,
                                                                    // y el puntero queda en el byte 14, donde se encuentra el tamanio del header
        fread(&header->tamEncabezado, sizeof(unsigned int), 1, pf);
        fread(&header->ancho, sizeof(unsigned int), 1, pf);
        fread(&header->alto, sizeof(unsigned int), 1, pf);
        fseek(pf, 2, SEEK_CUR);
        fread(&header->profundidad, sizeof(unsigned short), 1, pf);

        printf("\n tam arch: %d \n comienzo: %d \n tam enca: %d \n ancho: %d \n alto: %d \n prof %d ",
               header->tamArchivo, header->comienzoImagen, header->tamEncabezado, header->ancho,
               header->alto, header->profundidad
               );

        // leo la imagen
        img = (t_pixel *)malloc(header->alto * header->ancho * sizeof(t_pixel));
        t_pixel *ini = img;
        if(img == NULL){
            printf("\n No se pudo reservar memoria para la imagen.");
            return;
        }

        fseek(pf, header->comienzoImagen, SEEK_SET);
        //fread(img->pixel, sizeof(img->pixel),header->alto * header->ancho,pf);
        for( int i=0; i< header->alto; i++){
            for(int j=0; j<header->ancho; j++){
                fread(img[i*header->ancho+j].pixel, sizeof(unsigned char), 3, pf);
            }
        }

//        unsigned int padding = (4 - (header->ancho * sizeof(img->pixel)) % 4) % 4; // Calcula el padding
//        for (int i = 0; i < header->alto; i++) {
//            fread(img + i * header->ancho, sizeof(img->pixel),header->ancho, pf);
//            fseek(pf, padding, SEEK_CUR); // Salta los bytes de relleno
//        }

          printf("Primer pixel: R=%u, G=%u, B=%u\n", img[0].pixel[0], img[0].pixel[1], img[0].pixel[2]);




//        // mostrar imagen


        // archivo de salida
        //crearBmpSalida(img, header);
        //escalaDeGrises(img, header->alto, header->ancho);
        crearBmpSalida(img, header, "estudianteV2.bmp");
        img = ini;
        //escalaDeGrises(img, header);
        //imgNegativa(img, header);
        //aumentar25Contraste(img, header);
        //reducir25Contraste(img,header);
        //aumentar50red(img, header);
        //aumentar50blue(img, header);
        //aumentar50green(img, header);
        //recortar50(img, header);
        rotar90derecha(img, header);
        fclose(pf);

    }else{
        printf("\n El archivo indicado no es de tipo bit map.");
    }



    fclose(pf);

}

// funcion que convierte la imagen a escala de grises
int escalaDeGrises(t_pixel *imagen, t_metadata *header) {
    for (int i = 0; i < header->alto * header->ancho; i++) {
        unsigned char promedio = (imagen[i].pixel[0] + imagen[i].pixel[1] + imagen[i].pixel[2]) / 3;
        for (int j = 0; j < 8; j++) {
            imagen[i].pixel[j] = promedio;
        }
    }

    crearBmpSalida(imagen, header, "escala_grises.bmp");

    return 0;
}

int imgNegativa(t_pixel *imagen, t_metadata * header){

     for (int i = 0; i < header->alto * header->ancho; i++) {
        for (int j = 0; j < 8; j++) {
            imagen[i].pixel[j] = 255 - imagen[i].pixel[j];
        }
    }

    crearBmpSalida(imagen, header, "negativo.bmp");
    return 0;
}

int aumentar25Contraste(t_pixel *imagen, t_metadata *header){
    float porcentaje = 1.25;
    float valor_original = 0;
    float valor_nuevo = 0;

    for (int i = 0; i < header->alto * header->ancho; i++) {
        for (int j = 0; j < 3; j++) {
            valor_original = imagen[i].pixel[j];
            valor_nuevo = valor_original * porcentaje;
            if(valor_nuevo < 0){
                valor_nuevo = 0;
            }else if(valor_nuevo > 255){
                valor_nuevo = 255;
            }
            imagen[i].pixel[j] = (unsigned char)valor_nuevo;
        }
    }
    crearBmpSalida(imagen, header, "contraste25aum.bmp");
    return 0;
}

int reducir25Contraste(t_pixel *imagen, t_metadata *header){
    float porcentaje = 0.25;
    float valor_original = 0;
    float valor_nuevo = 0;

    for (int i = 0; i < header->alto * header->ancho; i++) {
        for (int j = 0; j < 3; j++) {
            valor_original = imagen[i].pixel[j];
            valor_nuevo = valor_original * porcentaje;
            valor_nuevo = valor_original - valor_nuevo;
            if(valor_nuevo < 0){
                valor_nuevo = 0;
            }else if(valor_nuevo > 255){
                valor_nuevo = 255;
            }
            imagen[i].pixel[j] = (unsigned char)valor_nuevo;
        }
    }
    crearBmpSalida(imagen, header, "contraste25redu.bmp");
    return 0;
}

int aumentar50red(t_pixel *imagen, t_metadata *header){
    int valor = 0;
    for (int i = 0; i < header->alto * header->ancho; i++) {
        valor = imagen[i].pixel[2] + (imagen[i].pixel[2] / 2);
        imagen[i].pixel[2] = (valor > 255) ? 255 : valor;
    }

    crearBmpSalida(imagen, header, "aumentar50red.bmp");
    return 0;
}

int aumentar50blue(t_pixel *imagen, t_metadata *header){
    int valor = 0;
    for (int i = 0; i < header->alto * header->ancho; i++) {
        valor = imagen[i].pixel[0] + (imagen[i].pixel[0] / 2);
        imagen[i].pixel[0] = (valor > 255) ? 255 : valor;
    }

    crearBmpSalida(imagen, header, "aumentar50blue.bmp");
    return 0;
}

int aumentar50green(t_pixel *imagen, t_metadata *header){
      int valor = 0;
    for (int i = 0; i < header->alto * header->ancho; i++) {
        valor = imagen[i].pixel[1] + (imagen[i].pixel[1] / 2);
        imagen[i].pixel[1] = (valor > 255) ? 255 : valor;
    }

    crearBmpSalida(imagen, header, "aumentar50green.bmp");
    return 0;

}

int recortar50(t_pixel *imagen, t_metadata *header){
    // definimos el nuevo tamaño de la imagen
    int ancho = header->ancho / 2;
    int alto = header->alto / 2;

    // reservamos memoria para la nueva imagen
    t_pixel *imgReco = malloc(ancho * alto * sizeof(t_pixel));

    // copiamos los pixeles de la imagen original a la nueva imagen
    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            // copiamos los pixeles de la imagen original a la nueva imagen
            imgReco[i * ancho + j] = imagen[i * header->ancho + j];
        }
    }

    //modificamos el header de la imagen
    header->alto = alto;
    header->ancho = ancho;

    //creamos la imagen de salida
    crearBmpSalida(imgReco, header, "recortada.bmp");

    free(imgReco);

    return 0;
}

int rotar90derecha(t_pixel *imagen, t_metadata *header ){

    // definimos el nuevo tamaño de la imagen
    int ancho = header->alto;
    int alto = header->ancho;

    // reservamos memoria para la nueva imagen
    t_pixel *imgRot = malloc(ancho * alto * sizeof(t_pixel));

    // copiamos los pixeles de la imagen original a la nueva imagen
    for (int i = 0; i < header->alto; i++) {
        for (int j = 0; j < header->ancho; j++) {
            // copiamos los pixeles de la imagen original a la nueva imagen
            imgRot[j * ancho + (header->alto - i - 1)] = imagen[i * header->ancho + j];
        }
    }

    //modificamos el header de la imagen
    header->alto = alto;
    header->ancho = ancho;

    //creamos la imagen de salida
    crearBmpSalida(imgRot, header, "rotada90derecha.bmp");

    free(imgRot);

    return 0;

}

// funcion que cree un archivo de salida
int crearBmpSalida(t_pixel *imagen, t_metadata *header, char nombre[20]){
    FILE *pf;
    pf = fopen(nombre, "wb");

    if (pf == NULL) {
        printf("No se pudo abrir el archivo.");
        return 1;
    }

    fwrite("BM", sizeof(char), 2, pf);
    fwrite(&header->tamArchivo, sizeof(unsigned int), 1, pf);
    fseek(pf, 4, SEEK_CUR); // me muevo 4 bytes, para posicionarme en el byte 10
                            // este byte nro 10 contiene el inicio de la imagen
    fwrite(&header->comienzoImagen, sizeof(unsigned int), 1, pf);// hago el fread, como es unsigned int ocupa 4bytes,
                                                                // y el puntero queda en el byte 14, donde se encuentra el tamanio del header
    fwrite(&header->tamEncabezado, sizeof(unsigned int), 1, pf);
    fwrite(&header->ancho, sizeof(unsigned int), 1, pf);
    fwrite(&header->alto, sizeof(unsigned int), 1, pf);
    fseek(pf, 2, SEEK_CUR);
    fwrite(&header->profundidad, sizeof(unsigned short), 1, pf);
    fseek(pf, header->comienzoImagen, SEEK_SET);
    for( int i=0; i< header->alto; i++){
            for(int j=0; j<header->ancho; j++){
                fwrite(imagen[i*header->ancho+j].pixel, sizeof(unsigned char), 3, pf);
            }
        }

    //fwrite(imagen, sizeof(imagen), header->alto * header->ancho, pf);

    fclose(pf);

    return TODO_OK;
}

