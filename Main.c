//Proyecto programado 1 - Lenguajes de Programación
//Gestor de datos climáticos
//Estudiantes:
//Duan Espinoza
//Andy Cruz
#include "headers/gestion_datos_climaticos.h"
#include <stdio.h>


// Función para mostrar el menú y obtener la opción del usuario
int mostrarMenu() {
    int opcion;
    printf("\n===== Menu =====\n");
    printf("1. Gestion de regiones\n");
    printf("2. Gestión de datos climaticos\n");
    printf("3. Procesamiento de datos\n");
    printf("4. Analisis de datos\n");
    printf("5. Análisis de correlacion\n");
    printf("6. Visualizacion de datos\n");
    printf("7. Busqueda de datos\n");
    printf("8. Estadisticas\n");
    printf("9. Salir\n");
    printf("Ingrese la opcion deseada: ");
    scanf("%d", &opcion);
    return opcion;
}

int main() {
    
    int opcion;
    do {
        opcion = mostrarMenu();
        switch (opcion) {
            case 1:
                printf("Ha seleccionado Gestion de regiones\n");
                // Aquí puedes llamar a la función correspondiente en tu archivo de header
                break;
            case 2:
                printf("Ha seleccionado Gestion de datos climaticos\n");
                gestionDatosClimaticos();
                // Aquí puedes llamar a la función correspondiente en tu archivo de header
                break;
            case 3:
                printf("Ha seleccionado Procesamiento de datos\n");
                // Aquí puedes llamar a la función correspondiente en tu archivo de header
                break;
            case 4:
                printf("Ha seleccionado Analisis de datos\n");
                // Aquí puedes llamar a la función correspondiente en tu archivo de header
                break;
            case 5:
                printf("Ha seleccionado Analisis de correlacion\n");
                // Aquí puedes llamar a la función correspondiente en tu archivo de header
                break;
            case 6:
                printf("Ha seleccionado Visualización de datos\n");
                // Aquí puedes llamar a la función correspondiente en tu archivo de header
                break;
            case 7:
                printf("Ha seleccionado Busqueda de datos\n");
                // Aquí puedes llamar a la función correspondiente en tu archivo de header
                break;
            case 8:
                printf("Ha seleccionado Estadisticas\n");
                // Aquí puedes llamar a la función correspondiente en tu archivo de header
                break;
            case 9:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion invalida. Por favor, seleccione una opcion valida.\n");
                break;
        }
    } while (opcion != 9);

    return 0;
}
