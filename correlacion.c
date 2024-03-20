#include "headers/correlacion.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "cJSON.h"
#include <math.h>


// Rutas de los archivos JSON
const char *path_JSONDatosClimaticos = "datos\\datos_climaticos.json";
const char *path_JSONDatosRegiones = "datos\\datos_regiones.json";

// Función para cargar el contenido del archivo JSON
cJSON *cargarJSON(const char *path) {
    FILE *archivo = fopen(path, "r");
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


// Función para verificar si una región existe en el archivo JSON de regiones
bool existeRegion(const char *nombre_region) {
    cJSON *json = cargarJSON(path_JSONDatosRegiones);
    if (json == NULL) {
        printf("Error al cargar el JSON de regiones.\n");
        return false;
    }

    cJSON *regiones_json = cJSON_GetObjectItem(json, "regiones");
    if (regiones_json == NULL) {
        printf("Error: No se encontró el arreglo 'regiones' en el JSON de regiones.\n");
        cJSON_Delete(json);
        return false;
    }

    cJSON *region_json = NULL;
    cJSON_ArrayForEach(region_json, regiones_json) {
        char *nombre = cJSON_GetObjectItem(region_json, "nombre")->valuestring;
        if (strcasecmp(nombre_region, nombre) == 0) {
            cJSON_Delete(json);
            return true;
        }
    }

    cJSON_Delete(json);
    return false;
}


// Función para filtrar los datos climáticos por región
cJSON *filtrarDatosClimaticosPorRegion(const char *nombre_region) {
    cJSON *json = cargarJSON(path_JSONDatosClimaticos);
    if (json == NULL) {
        printf("Error al cargar el JSON de datos climáticos.\n");
        return NULL;
    }

    cJSON *datos_filtrados = cJSON_CreateArray();
    cJSON *datos_climaticos_json = cJSON_GetArrayItem(json, 0); // El archivo tiene una raíz de tipo array
    cJSON *dato_json = NULL;
    cJSON_ArrayForEach(dato_json, datos_climaticos_json) {
        char *region = cJSON_GetObjectItem(dato_json, "region")->valuestring;
        if (strcasecmp(nombre_region, region) == 0) {
            cJSON_AddItemToArray(datos_filtrados, cJSON_Duplicate(dato_json, true));
        }
    }

    cJSON_Delete(json);
    return datos_filtrados;
}

// Función para seleccionar los parámetros a correlacionar
void seleccionarParametrosCorrelacion() {
    char nombre_region[50];
    printf("Ingrese el nombre de la región: ");
    scanf("%s", nombre_region);

    if (!existeRegion(nombre_region)) {
        printf("La región ingresada no existe.\n");
        return;
    }

    cJSON *datos_filtrados = filtrarDatosClimaticosPorRegion(nombre_region);
    if (datos_filtrados == NULL) {
        printf("No se pudieron obtener los datos climáticos para la región especificada.\n");
        return;
    }

    // Obtener la cantidad de datos disponibles para correlacionar
    int num_datos = cJSON_GetArraySize(datos_filtrados);
    if (num_datos < 2) {
        printf("No hay suficientes datos para realizar la correlación.\n");
        cJSON_Delete(datos_filtrados);
        return;
    }

    // Seleccionar los parámetros a correlacionar
    printf("Parámetros disponibles para correlacionar:\n");
    printf("1. Temperatura\n");
    printf("2. Humedad\n");
    printf("3. Presión\n");
    printf("4. Velocidad del viento\n");
    printf("5. Precipitación\n");

    int parametro1, parametro2;
    printf("Seleccione el primer parámetro (1-5): ");
    scanf("%d", &parametro1);

    printf("Seleccione el segundo parámetro (1-5, diferente al primero): ");
    scanf("%d", &parametro2);

    // Verificar que los parámetros seleccionados sean válidos
    if (parametro1 < 1 || parametro1 > 5 || parametro2 < 1 || parametro2 > 5 || parametro1 == parametro2) {
        printf("Selección de parámetros inválida.\n");
        cJSON_Delete(datos_filtrados);
        return;
    }

    // Obtener los arreglos de datos para los parámetros seleccionados
    double *x = (double *)malloc(num_datos * sizeof(double));
    double *y = (double *)malloc(num_datos * sizeof(double));

    cJSON *dato_json = NULL;
    int i = 0;
    cJSON_ArrayForEach(dato_json, datos_filtrados) {
        switch (parametro1) {
            case 1:
                x[i] = cJSON_GetObjectItem(dato_json, "temperatura")->valuedouble;
                break;
            case 2:
                x[i] = cJSON_GetObjectItem(dato_json, "humedad")->valuedouble;
                break;
            case 3:
                x[i] = cJSON_GetObjectItem(dato_json, "presion")->valuedouble;
                break;
            case 4:
                x[i] = cJSON_GetObjectItem(dato_json, "velcdViento")->valuedouble;
                break;
            case 5:
                x[i] = cJSON_GetObjectItem(dato_json, "precipitacion")->valuedouble;
                break;
        }

        switch (parametro2) {
            case 1:
                y[i] = cJSON_GetObjectItem(dato_json, "temperatura")->valuedouble;
                break;
            case 2:
                y[i] = cJSON_GetObjectItem(dato_json, "humedad")->valuedouble;
                break;
            case 3:
                y[i] = cJSON_GetObjectItem(dato_json, "presion")->valuedouble;
                break;
            case 4:
                y[i] = cJSON_GetObjectItem(dato_json, "velcdViento")->valuedouble;
                break;
            case 5:
                y[i] = cJSON_GetObjectItem(dato_json, "precipitacion")->valuedouble;
                break;
        }

        i++;
    }

    // Calcular la correlación de Pearson
    double correlacion = calcularCorrelacionPearson(x, y, num_datos);

    // Mostrar el resultado al usuario
    mostrarResultadoCorrelacion(correlacion);

    // Liberar la memoria asignada para los arreglos
    free(x);
    free(y);
    cJSON_Delete(datos_filtrados);
}



// Función para mostrar el resultado de la correlación al usuario
void mostrarResultadoCorrelacion(double correlacion) {
    printf("El coeficiente de correlación de Pearson entre los dos conjuntos de datos es: %lf\n", correlacion);
}


// Función para mostrar el menú de correlación
void menuCorrelacion() {
    int opcion;
    do {
        printf("\n===== Menú de Correlación =====\n");
        printf("1. Seleccionar parámetros de correlación\n");
        printf("2. Salir al menú principal\n");
        printf("Ingrese la opción deseada: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                seleccionarParametrosCorrelacion();
                break;
            case 2:
                printf("Volviendo al menú principal...\n");
                break;
            default:
                printf("Opción no válida. Por favor, seleccione una opción válida.\n");
                break;
        }
    } while (opcion != 2);
}

//Se requiere una funcionalidad previa que filtre los datos obtenidos de daatos_climaticos,json

double calcularCorrelacionPearson(double *x, double *y, int n) {
    double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0;
    double sum_x_squared = 0.0, sum_y_squared = 0.0;

    for (int i = 0; i < n; i++) {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_x_squared += x[i] * x[i];
        sum_y_squared += y[i] * y[i];
    }

    double correlacion_numerador = n * sum_xy - sum_x * sum_y;
    double correlacion_denominador = sqrt((n * sum_x_squared - sum_x * sum_x) * (n * sum_y_squared - sum_y * sum_y));

    if (correlacion_denominador == 0) {
        return 0; // Evitar división por cero
    }

    return correlacion_numerador / correlacion_denominador;
}
