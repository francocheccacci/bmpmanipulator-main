#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
//        // mostrar imagen
        // archivo de salida
        //crearBmpSalida(img, header);
        //escalaDeGrises(img, header->alto, header->ancho);
        //crearBmpSalida(&img, &header, "estudianteV2.bmp");
        //escalaDeGrises(img, header);
        //imgNegativa(img, header);
        //aumentar25Contraste(img, header);
        //reducir25Contraste(img,header);
        //aumentar50red(img, header);
        //aumentar50blue(img, header);
        //aumentar50green(img, header);
        //recortar50(img, header);

        //morphing(img, header, 2);

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

        // printf("\n tam arch: %d \n comienzo: %d \n tam enca: %d \n ancho: %d \n alto: %d \n prof %d ",
        //        header->tamArchivo, header->comienzoImagen, header->tamEncabezado, header->ancho,
        //        header->alto, header->profundidad
        //        );

        // leo la imagen
        img = (t_pixel *)malloc(header->alto * header->ancho * sizeof(t_pixel));
        if(img == NULL){
            printf("No se pudo reservar memoria para la imagen. \n");
            return;
        }

        fseek(pf, header->comienzoImagen, SEEK_SET);
        for( int i=0; i< header->alto; i++){
            for(int j=0; j<header->ancho; j++){
                fread(img[i*header->ancho+j].pixel, sizeof(unsigned char), 3, pf);
            }
        }

        //imgNegativa(img, header);
        //aumentar25Contraste(img, header);
        //reducir25Contraste(img,header);
        //aumentar50red(img, header);
        //aumentar50blue(img, header);
        //aumentar50green(img, header);
        //recortar50(img, header);
        escalaDeGrises(img, header);
        rotar90izquierda(img, header);
       // rotar90derecha(img, header);
        return 0;

        fclose(pf);

    }else{
        printf("\n El archivo indicado no es de tipo bit map.");
    }

    fclose(pf);

}

// funcion que convierte la imagen a escala de grises
int escalaDeGrises(t_pixel *imagen, t_metadata *header) {

    for (int i = 0; i < header->alto * header->ancho; i++) {
        unsigned char promedio = (imgCopy[i].pixel[0] + imgCopy[i].pixel[1] + imgCopy[i].pixel[2]) / 3;
        for (int j = 0; j < 8; j++) {
            imgCopy[i].pixel[j] = promedio;
        }
    }

    crearBmpSalida(imgCopy, header, "imagenesAlumno/escala_grises.bmp");
    free(imgCopy);
    return 0;
}

int imgNegativa(t_pixel *imagen, t_metadata * header){

     for (int i = 0; i < header->alto * header->ancho; i++) {
        for (int j = 0; j < 8; j++) {
            imagen[i].pixel[j] = 255 - imagen[i].pixel[j];
        }
    }

    crearBmpSalida(imagen, header, "imagenesAlumno/negativo.bmp");
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
    crearBmpSalida(imagen, header, "imagenesAlumno/contraste25aum.bmp");
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
    crearBmpSalida(imagen, header, "imagenesAlumno/contraste25redu.bmp");
    return 0;
}

int aumentar50red(t_pixel *imagen, t_metadata *header){
    int valor = 0;
    for (int i = 0; i < header->alto * header->ancho; i++) {
        valor = imagen[i].pixel[2] + (imagen[i].pixel[2] / 2);
        imagen[i].pixel[2] = (valor > 255) ? 255 : valor;
    }

    crearBmpSalida(imagen, header, "imagenesAlumno/aumentar50red.bmp");
    return 0;
}

int aumentar50blue(t_pixel *imagen, t_metadata *header){
    int valor = 0;
    for (int i = 0; i < header->alto * header->ancho; i++) {
        valor = imagen[i].pixel[0] + (imagen[i].pixel[0] / 2);
        imagen[i].pixel[0] = (valor > 255) ? 255 : valor;
    }

    crearBmpSalida(imagen, header, "imagenesAlumno/aumentar50blue.bmp");
    return 0;
}

int aumentar50green(t_pixel *imagen, t_metadata *header){
      int valor = 0;
    for (int i = 0; i < header->alto * header->ancho; i++) {
        valor = imagen[i].pixel[1] + (imagen[i].pixel[1] / 2);
        imagen[i].pixel[1] = (valor > 255) ? 255 : valor;
    }

    crearBmpSalida(imagen, header, "imagenesAlumno/aumentar50green.bmp");
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
    crearBmpSalida(imgReco, header, "imagenesAlumno/recortada.bmp");

    free(imgReco);

    return 0;
}

int rotar90izquierda(t_pixel *imagen, t_metadata *header ){

    // definimos el nuevo tamaño de la imagen
    int ancho = header->alto;
    int alto = header->ancho;

    // reservamos memoria para la nueva imagen
    t_pixel *imgRot = malloc(ancho * alto * sizeof(t_pixel));

    // copiamos los pixeles de la imagen original a la nueva imagen
    for (int i = 0; i < header->alto; i++) {
        for (int j = 0; j < header->ancho; j++) {
            //j * ancho avanza en las columnas
            //header->alto - i - 1 | calcula la fila en la imagen rotada
            //me posiciono en la columna con j * ancho
            imgRot[j * ancho + (header->alto - i - 1)] = imagen[i * header->ancho + j];
        }
    }

    //modificamos el header de la imagen
    header->alto = alto;
    header->ancho = ancho;

    //creamos la imagen de salida
    crearBmpSalida(imgRot, header, "imagenesAlumno/rotada90izquierda.bmp");

    free(imgRot);

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
            // imagen[i * header->ancho + j] es la posicion (i,j) de la imagen original.
            // header->ancho es la altura de la imagen y j representa la coordenada Y
            // en la imagen rotada i es mi eje X

            imgRot[ (header->ancho - j - 1) * ancho + i] = imagen[i * header->ancho + j];
            // a la altura de la imagen le resto el valor de j para posicionarme en el pixel de la imagen rotada
        }
    }

    //modificamos el header de la imagen
    header->alto = alto;
    header->ancho = ancho;

    //creamos la imagen de salida
    crearBmpSalida(imgRot, header, "imagenesAlumno/rotada90derecha.bmp");

    free(imgRot);

    return 0;

}

int combinarImagenes(t_pixel *imagen1, t_metadata *header1, float variador){

    t_pixel *img2 = NULL;
    t_metadata *header2 = NULL;
    header2 = malloc(sizeof(t_metadata));
            printf("hola.\n");

    printf("Ingrese nombre del segundo archivo para combinar: (sin la extenstion .bmp) \n");
    char nombre[50];
    scanf("%s", nombre);

    FILE *pf_img2 = fopen(strcat(nombre, ".bmp"), "rb");
    if (!pf_img2) {
        printf("No se pudo abrir el archivo.\n");
        return 1;
    }
    if(getc(pf_img2) == 'B' && getc(pf_img2) == 'M'){ // aca estoy parado en el byte 2
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
        if(img2 == NULL){
            printf("\n No se pudo reservar memoria para la imagen.");
            return 1;
        }

        fseek(pf_img2, header2->comienzoImagen, SEEK_SET);
        for( int i=0; i< header2->alto; i++){
            for(int j=0; j<header2->ancho; j++){
                fread(img2[i*header2->ancho+j].pixel, sizeof(unsigned char), 3, pf_img2);
            }
        }

        fclose(pf_img2);
        unsigned char r, g, b;
        t_pixel *imgCombinada = malloc(header1->alto * header1->ancho * sizeof(t_pixel));
        for (int i = 0; i < header1->alto * header1->ancho; i++) {
            // copiamos los pixeles de la imagen original a la nueva imagen
            b = ( ((1 - variador) * imagen1[i].pixel[0]) + img2[i].pixel[0] ) / 2;
            g = ( ((1 - variador) * imagen1[i].pixel[1]) + img2[i].pixel[1] ) / 2;
            r = ( ((1 - variador) * imagen1[i].pixel[2]) + img2[i].pixel[2] ) / 2;
//            imgCombinada[i].pixel[0] = (1 - variador)*imagen1[i].pixel[0] + img2[i].pixel[0];
//            imgCombinada[i].pixel[1] = (1 - variador)*imagen1[i].pixel[1] + img2[i].pixel[1];
//            imgCombinada[i].pixel[2] = (1 - variador)*imagen1[i].pixel[2] + img2[i].pixel[2];
            imgCombinada[i].pixel[0] = b;
            imgCombinada[i].pixel[1] = g;
            imgCombinada[i].pixel[2] = r;

        }

        crearBmpSalida(imgCombinada, header1, "imagenesAlumno/combinada.bmp");

        printf("hola");
    }else{
        printf("\n El archivo indicado no es de tipo bit map.");
    }

    free(img2);
    free(header2);

    return 0;
}

// funcion que cree un archivo de salida
int crearBmpSalida(t_pixel *imagen, t_metadata *header, char nombre[]){
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

