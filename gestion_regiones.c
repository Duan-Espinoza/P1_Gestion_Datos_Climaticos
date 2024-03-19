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


/**
 * 
 * 
 * ESTA EXISTE EN OTRO HEADER .H
 * 
 *  
 * Se encarga de seguir la ruta para parsear el archivo 
 * y retornar el contenido del archivo 
*/
char* getContenido1(const char *pathCsv){

    FILE* archivo = fopen(pathCsv,"rb");
    if(archivo == NULL){
        printf("\n-No se pudo abrir el archivo\n");
        return NULL;
    }
    
    fseek(archivo,0, SEEK_END);
    long size_archivo = ftell(archivo);
    fseek(archivo,0,SEEK_SET);

    char *contenido = (char*)malloc(size_archivo+1);
    if( contenido == NULL){
        printf("\nError de lectura\n");
        fclose(archivo);
        return NULL;
    }
    fread(contenido,1, size_archivo, archivo);
    fclose(archivo);
    contenido[size_archivo] = '\0';
    
    return contenido;
}


// Función para cargar el contenido del archivo JSON en un objeto cJSON
cJSON *cargarJSON() {
    FILE *archivo = fopen(path_JSONDatosClimaticos, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo JSON.\n");
        return NULL;
    }

    fseek(archivo, 0, SEEK_END);
    long tamaño_archivo = ftell(archivo);
    fseek(archivo, 0, SEEK_SET);

    char *contenido_json = (char *)malloc(tamaño_archivo + 1);
    fread(contenido_json, 1, tamaño_archivo, archivo);
    fclose(archivo);

    contenido_json[tamaño_archivo] = '\0';

    cJSON *json = cJSON_Parse(contenido_json);
    free(contenido_json);

    return json;
}

// Función para guardar el objeto cJSON de vuelta al archivo JSON
void guardarJSON(cJSON *json) {
    FILE *archivo = fopen(path_JSONDatosClimaticos, "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo JSON para escritura.\n");
        return;
    }

    char *json_str = cJSON_Print(json);
    fprintf(archivo, "%s\n", json_str);
    fclose(archivo);
    free(json_str);
}


// Función para crear una nueva región
void crearRegion(Region region) {
    cJSON *json = cargarJSON();
    if (json == NULL) {
        printf("Error al cargar el JSON.\n");
        return;
    }

    cJSON *regiones_json = cJSON_GetObjectItem(json, "regiones");
    if (regiones_json == NULL) {
        printf("Error: No se encontró el arreglo 'regiones' en el JSON.\n");
        cJSON_Delete(json);
        return;
    }

    // Verificar si ya existe una región con el mismo nombre
    cJSON *region_json = NULL;
    cJSON_ArrayForEach(region_json, regiones_json) {
        char *nombre = cJSON_GetObjectItem(region_json, "nombre")->valuestring;
        if (strcasecmp(region.nombre, nombre) == 0) {
            printf("Ya existe una región con el nombre \"%s\".\n", region.nombre);
            cJSON_Delete(json);
            return;
        }
    }

    cJSON *nueva_region_json = cJSON_CreateObject();
    cJSON_AddStringToObject(nueva_region_json, "nombre", region.nombre);
    cJSON_AddNumberToObject(nueva_region_json, "ubicacion_x", region.ubicacion_x);
    cJSON_AddNumberToObject(nueva_region_json, "ubicacion_y", region.ubicacion_y);
    cJSON_AddStringToObject(nueva_region_json, "descripcion", region.descripcion);

    cJSON_AddItemToArray(regiones_json, nueva_region_json);

    guardarJSON(json);
    cJSON_Delete(json);

    printf("Región \"%s\" creada exitosamente.\n", region.nombre);
}

// 
// 
// Función para mostrar todas las regiones
void mostrarRegiones() {
    cJSON *json = cargarJSON();
    if (json == NULL) {
        printf("Error al cargar el JSON.\n");
        return;
    }

    cJSON *regiones_json = cJSON_GetObjectItem(json, "regiones");
    if (regiones_json == NULL) {
        printf("Error: No se encontró el arreglo 'regiones' en el JSON.\n");
        cJSON_Delete(json);
        return;
    }

    printf("\n=== Lista de Regiones ===\n");
    cJSON *region_json = NULL;
    cJSON_ArrayForEach(region_json, regiones_json) {
        char *nombre = cJSON_GetObjectItem(region_json, "nombre")->valuestring;
        int ubicacion_x = cJSON_GetObjectItem(region_json, "ubicacion_x")->valueint;
        int ubicacion_y = cJSON_GetObjectItem(region_json, "ubicacion_y")->valueint;
        char *descripcion = cJSON_GetObjectItem(region_json, "descripcion")->valuestring;

        printf("Nombre: %s\n", nombre);
        printf("Ubicación: (%d, %d)\n", ubicacion_x, ubicacion_y);
        printf("Descripción: %s\n", descripcion);
        printf("\n");
    }

    cJSON_Delete(json);
}


/// Función para eliminar una región
void eliminarRegion(char nombre[]) {
    cJSON *json = cargarJSON();
    if (json == NULL) {
        printf("Error al cargar el JSON.\n");
        return;
    }

    cJSON *regiones_json = cJSON_GetObjectItem(json, "regiones");
    if (regiones_json == NULL) {
        printf("Error: No se encontró el arreglo 'regiones' en el JSON.\n");
        cJSON_Delete(json);
        return;
    }

    cJSON *region_json = NULL;
    int indice = -1;
    int i = 0;
    cJSON_ArrayForEach(region_json, regiones_json) {
        char *nombre_region = cJSON_GetObjectItem(region_json, "nombre")->valuestring;
        if (strcasecmp(nombre, nombre_region) == 0) {
            indice = i;
            break;
        }
        i++;
    }

    if (indice != -1) {
        cJSON_DeleteItemFromArray(regiones_json, indice);
        guardarJSON(json);
        cJSON_Delete(json);
        printf("Región \"%s\" eliminada correctamente.\n", nombre);
    } else {
        printf("No se encontró una región con el nombre \"%s\".\n", nombre);
        cJSON_Delete(json);
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