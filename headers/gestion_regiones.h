#ifndef GESTION_REGIONES_H
#define GESTION_REGIONES_H

#include <stdio.h>

// Estructura para almacenar datos de una región
typedef struct Region {
    char nombre[50];
    float x;
    float y;
    char descripcion[200];
} Region;

// Prototipos de funciones
void crearRegion(Region *region);
void mostrarRegiones(void);
void eliminarRegion(char *nombre);

#endif // GESTION_REGIONES_H
