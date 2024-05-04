#ifndef FUNCIONES_ESTUDIANTE_H_INCLUDED
#define FUNCIONES_ESTUDIANTE_H_INCLUDED
#include "estructuras.h"

void solucion(int argc, char* argv[]);
int openBmpFile(char* argv[], t_pixel *img, t_metadata *header);
int escalaDeGrises(  t_pixel *imagen, t_metadata *header, char nombre[]);
int imgNegativa(t_pixel *imagen, t_metadata *header, char nombre[]);
int crearBmpSalida(t_pixel *imagen, t_metadata *header,char nombre[]);
int aumentar25Contraste(t_pixel *imagen, t_metadata *header, char nombre[]);
int reducir25Contraste(t_pixel *imagen, t_metadata *header, char nombre[]);
int aumentar50red(t_pixel *imagen, t_metadata *header, char nombre[]);
int aumentar50blue(t_pixel *imagen, t_metadata *header, char nombre[]);
int aumentar50green(t_pixel *imagen, t_metadata *header, char nombre[]);
int recortar50(t_pixel *imagen, t_metadata *header, char nombre[]);
int rotar90izquierda(t_pixel *imagen, t_metadata *header, char nombre[]);
int rotar90derecha(t_pixel *imagen, t_metadata *header, char nombre[]);
int combinarImagenes(t_pixel *imagen1, t_metadata *header1, char nombre[]);
void definirEfecto(char* argv[],t_metadata* header,t_pixel* img);
#endif // FUNCIONES_ESTUDIANTE_H_INCLUDED
