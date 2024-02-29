//Proyecto programado 1 - Lenguajes de Programación
//Gestor de datos climáticos
//Estudiantes:
//Duan Espinoza

#include <stdio.h>

#include <stdio.h>

// Función para mostrar el menú y obtener la opción del usuario
int mostrarMenu() {
    int opcion;
    printf("\n===== Menú =====\n");
    printf("1. Gestión de regiones\n");
    printf("2. Gestión de datos climáticos\n");
    printf("3. Procesamiento de datos\n");
    printf("4. Análisis de datos\n");
    printf("5. Análisis de correlación\n");
    printf("6. Visualización de datos\n");
    printf("7. Búsqueda de datos\n");
    printf("8. Estadísticas\n");
    printf("9. Salir\n");
    printf("Ingrese la opción deseada: ");
    scanf("%d", &opcion);
    return opcion;
}

int main() {
    int opcion;
    do {
        opcion = mostrarMenu();
        switch (opcion) {
            case 1:
                printf("Ha seleccionado Gestión de regiones\n");
                // Aquí puedes llamar a la función correspondiente en tu archivo de header
                break;
            case 2:
                printf("Ha seleccionado Gestión de datos climáticos\n");
                // Aquí puedes llamar a la función correspondiente en tu archivo de header
                break;
            case 3:
                printf("Ha seleccionado Procesamiento de datos\n");
                // Aquí puedes llamar a la función correspondiente en tu archivo de header
                break;
            case 4:
                printf("Ha seleccionado Análisis de datos\n");
                // Aquí puedes llamar a la función correspondiente en tu archivo de header
                break;
            case 5:
                printf("Ha seleccionado Análisis de correlación\n");
                // Aquí puedes llamar a la función correspondiente en tu archivo de header
                break;
            case 6:
                printf("Ha seleccionado Visualización de datos\n");
                // Aquí puedes llamar a la función correspondiente en tu archivo de header
                break;
            case 7:
                printf("Ha seleccionado Búsqueda de datos\n");
                // Aquí puedes llamar a la función correspondiente en tu archivo de header
                break;
            case 8:
                printf("Ha seleccionado Estadísticas\n");
                // Aquí puedes llamar a la función correspondiente en tu archivo de header
                break;
            case 9:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción inválida. Por favor, seleccione una opción válida.\n");
                break;
        }
    } while (opcion != 9);

    return 0;
}
