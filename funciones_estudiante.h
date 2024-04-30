#ifndef FUNCIONES_ESTUDIANTE_H_INCLUDED
#define FUNCIONES_ESTUDIANTE_H_INCLUDED
#include "estructuras.h"

void solucion(int argc, char* argv[]);
void openBmpFile(t_pixel *img, t_metadata *header);
int escalaDeGrises(  t_pixel *imagen, t_metadata *header);
int imgNegativa(t_pixel *imagen, t_metadata *header);
int crearBmpSalida(t_pixel *imagen, t_metadata *header,char nombre[20]);
int aumentar25Contraste(t_pixel *imagen, t_metadata *header);
int reducir25Contraste(t_pixel *imagen, t_metadata *header);
int aumentar50red(t_pixel *imagen, t_metadata *header);
int aumentar50blue(t_pixel *imagen, t_metadata *header);
int aumentar50green(t_pixel *imagen, t_metadata *header);
int recortar50(t_pixel *imagen, t_metadata *header);
#endif // FUNCIONES_ESTUDIANTE_H_INCLUDED
