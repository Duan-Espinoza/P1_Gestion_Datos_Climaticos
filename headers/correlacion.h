#ifndef CORRELACION_H
#define CORRELACION_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "cJSON.h"
#include <math.h>

// Declaración de la función para cargar el contenido del archivo JSON
cJSON *cargarJSON(const char *path);

// Declaración de la función para verificar si una región existe en el archivo JSON de regiones
bool existeRegion(const char *nombre_region);

// Declaración de la función para filtrar los datos climáticos por región
cJSON *filtrarDatosClimaticosPorRegion(const char *nombre_region);

// Declaración de la función para seleccionar los parámetros a correlacionar
void seleccionarParametrosCorrelacion();

// Declaración de la función para mostrar el resultado de la correlación al usuario
void mostrarResultadoCorrelacion(double correlacion);

// Declaración de la función para mostrar el menú de correlación
void menuCorrelacion();

// Declaración de la función para calcular el coeficiente de correlación de Pearson
double calcularCorrelacionPearson(double *x, double *y, int n);

#endif /* CORRELACION_H */
