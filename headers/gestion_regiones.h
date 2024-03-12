#ifndef GESTION_REGIONES_H
#define GESTION_REGIONES_H

#include <stdio.h>

// Estructura para representar una región
typedef struct {
    char nombre[50];
    int ubicacion_x;
    int ubicacion_y;
    char descripcion[100];
} Region;

// Función para crear una nueva región
void crearRegion(Region region);

// Función para mostrar todas las regiones
void mostrarRegiones();

// Función para eliminar una región
void eliminarRegion(char nombre[]);

// Función para mostrar el menú de gestión de regiones
void mostrarMenuGestionRegiones();

#endif /* REGIONES_H */