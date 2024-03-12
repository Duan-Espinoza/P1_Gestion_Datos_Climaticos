#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "headers/gestion_regiones.h"

//path con el json
const char *path_JSONDatosClimaticos = "datos\\datos_regiones.json";


// Definir un arreglo para almacenar las regiones
#define MAX_REGIONES 100
Region regiones[MAX_REGIONES];
int num_regiones = 0;

// Función para crear una nueva región
void crearRegion(Region region) {
    // Abrir el archivo JSON de regiones en modo lectura
    FILE *archivo = fopen(path_JSONDatosClimaticos, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    // Leer el contenido del archivo JSON
    fseek(archivo, 0, SEEK_END);
    long tamaño_archivo = ftell(archivo);
    fseek(archivo, 0, SEEK_SET);
    char *contenido_json = malloc(tamaño_archivo + 1);
    fread(contenido_json, 1, tamaño_archivo, archivo);
    contenido_json[tamaño_archivo] = '\0';
    fclose(archivo);

    // Convertir el contenido del archivo a un objeto cJSON
    cJSON *json = cJSON_Parse(contenido_json);
    free(contenido_json);

    // Verificar si ya existe una región con el mismo nombre
    cJSON *regiones_json = cJSON_GetObjectItem(json, "regiones");
    cJSON *region_json = NULL;
    cJSON_ArrayForEach(region_json, regiones_json) {
        char *nombre = cJSON_GetObjectItem(region_json, "nombre")->valuestring;
        if (strcasecmp(region.nombre, nombre) == 0) {
            printf("Ya existe una región con el nombre \"%s\".\n", region.nombre);
            cJSON_Delete(json);
            return;
        }
    }
    cJSON_Delete(json);

    // Agregar la nueva región al arreglo
    if (num_regiones < MAX_REGIONES) {
        regiones[num_regiones] = region;
        num_regiones++;
        printf("Región \"%s\" creada exitosamente.\n", region.nombre);

        // Abrir el archivo JSON de regiones en modo escritura (agregar al final)
        archivo = fopen(path_JSONDatosClimaticos, "a");
        if (archivo == NULL) {
            printf("Error al abrir el archivo.\n");
            return;
        }

        // Crear un objeto cJSON para la nueva región
        cJSON *nueva_region_json = cJSON_CreateObject();
        cJSON_AddStringToObject(nueva_region_json, "nombre", region.nombre);
        cJSON_AddNumberToObject(nueva_region_json, "ubicacion_x", region.ubicacion_x);
        cJSON_AddNumberToObject(nueva_region_json, "ubicacion_y", region.ubicacion_y);
        cJSON_AddStringToObject(nueva_region_json, "descripcion", region.descripcion);

        // Convertir el objeto cJSON a una cadena JSON y escribirlo en el archivo
        char *nueva_region_str = cJSON_PrintUnformatted(nueva_region_json);
        fprintf(archivo, "%s\n", nueva_region_str);

        // Liberar memoria y cerrar el archivo
        fclose(archivo);
        cJSON_Delete(nueva_region_json);
        free(nueva_region_str);
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
    Region nueva_region;
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
                //Region nueva_region;
                printf("Ingrese el nombre de la región: ");
                scanf("%s", nueva_region.nombre);
                printf("Ingrese la ubicación X de la región: ");
                scanf("%d", &nueva_region.ubicacion_x);
                printf("Ingrese la ubicación Y de la región: ");
                scanf("%d", &nueva_region.ubicacion_y);
                printf("Ingrese la descripción de la región: ");
                scanf("%s", nueva_region.descripcion);
                crearRegion(nueva_region);
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