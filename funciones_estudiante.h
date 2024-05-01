#ifndef FUNCIONES_ESTUDIANTE_H_INCLUDED
#define FUNCIONES_ESTUDIANTE_H_INCLUDED
#include "estructuras.h"

void solucion(int argc, char* argv[]);
void openBmpFile(char* argv[], t_pixel *img, t_metadata *header);
int escalaDeGrises(  t_pixel *imagen, t_metadata *header, char nombre[]);
int imgNegativa(t_pixel *imagen, t_metadata *header, char nombre[]);
int crearBmpSalida(t_pixel *imagen, t_metadata *header,char nombre[]);
int aumentar25Contraste(t_pixel *imagen, t_metadata *header, char nombre[]);
int reducir25Contraste(t_pixel *imagen, t_metadata *header, char nombre[]);
void definirEfecto(char* argv[],t_metadata* header,t_pixel* img);
#endif // FUNCIONES_ESTUDIANTE_H_INCLUDED
