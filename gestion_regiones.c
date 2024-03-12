#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/gestion_regiones.h"

// Definir un arreglo para almacenar las regiones
#define MAX_REGIONES 100
Region regiones[MAX_REGIONES];
int num_regiones = 0;

// Función para crear una nueva región
void crearRegion(Region region) {
    // Verificar si ya existe una región con el mismo nombre
    for (int i = 0; i < num_regiones; i++) {
        if (strcasecmp(region.nombre, regiones[i].nombre) == 0) {
            printf("Ya existe una región con el nombre \"%s\".\n", region.nombre);
            return;
        }
    }

    // Agregar la nueva región al arreglo
    if (num_regiones < MAX_REGIONES) {
        regiones[num_regiones] = region;
        num_regiones++;
        printf("Región \"%s\" creada exitosamente.\n", region.nombre);
    } else {
        printf("Se ha alcanzado el máximo de regiones que se pueden almacenar.\n");
    }
}

// Función para mostrar todas las regiones
void mostrarRegiones() {
    printf("\n=== Lista de Regiones ===\n");
    for (int i = 0; i < num_regiones; i++) {
        printf("Nombre: %s\n", regiones[i].nombre);
        printf("Ubicación: (%d, %d)\n", regiones[i].ubicacion_x, regiones[i].ubicacion_y);
        printf("Descripción: %s\n", regiones[i].descripcion);
        printf("\n");
    }
}

// Función para eliminar una región
void eliminarRegion(char nombre[]) {
    int indice = -1;
    // Buscar la región por su nombre (case insensitive)
    for (int i = 0; i < num_regiones; i++) {
        if (strcasecmp(nombre, regiones[i].nombre) == 0) {
            indice = i;
            break;
        }
    }

    if (indice != -1) {
        // Aquí puedes implementar la lógica para validar y eliminar la región
        // Por ahora, simplemente la eliminaremos del arreglo
        for (int i = indice; i < num_regiones - 1; i++) {
            regiones[i] = regiones[i + 1];
        }
        num_regiones--;
        printf("Región \"%s\" eliminada correctamente.\n", nombre);
    } else {
        printf("No se encontró una región con el nombre \"%s\".\n", nombre);
    }
}


// Implementación de la función para mostrar el menú de gestión de regiones
void mostrarMenuGestionRegiones() {
    int opcion;
    do {
        printf("\n===== Menú de Gestión de Regiones =====\n");
        printf("1. Crear región\n");
        printf("2. Mostrar regiones\n");
        printf("3. Eliminar región\n");
        printf("4. Volver al menú principal\n");
        printf("Ingrese la opción deseada: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                // Aquí puedes solicitar al usuario los datos de la región y llamar a la función crearRegion
                break;
            case 2:
                mostrarRegiones();
                break;
            case 3: {
                char nombre[50];
                printf("Ingrese el nombre de la región a eliminar: ");
                scanf("%s", nombre);
                eliminarRegion(nombre);
                break;
            }
            case 4:
                printf("Volviendo al menú principal...\n");
                break;
            default:
                printf("Opción no válida. Por favor, seleccione una opción válida.\n");
                break;
        }
    } while (opcion != 4);
}