#include "headers/gestion_regiones.h"

// Función para mostrar el menú de gestión de regiones
void mostrarMenuGestionRegiones(void) {
    int opcion;

    do {
        printf("\n===== Menu Gestion de Regiones =====\n");
        printf("1. Crear region\n");
        printf("2. Eliminar region\n");
        printf("3. Mostrar regiones\n");
        printf("4. Salir\n");
        printf("Ingrese la opcion deseada: ");
        scanf("%d", &opcion);

        switch (opcion) {
        case 1:
            // Crear una nueva región
            char nombre[50];
            float coordenadaX, coordenadaY;
            char descripcion[200];
            printf("Ingrese el nombre de la region: ");
            scanf("%s", nombre);
            printf("Ingrese la coordenada X: ");
            scanf("%f", &coordenadaX);
            printf("Ingrese la coordenada Y: ");
            scanf("%f", &coordenadaY);
            printf("Ingrese la descripcion: ");
            scanf(" %[^\n]%*c", descripcion); // Limpiar buffer
            //Region nuevaRegion = crearRegion(nombre, coordenadaX, coordenadaY, descripcion);
            // TODO: Almacenar la nueva región en la estructura de datos
            break;
        case 2:
            // Eliminar una región
            char nombreRegionEliminar[50];
            printf("Ingrese el nombre de la region a eliminar: ");
            scanf("%s", nombreRegionEliminar);
            //if (eliminarRegion(nombreRegionEliminar)) {
            //printf("Region eliminada correctamente.\n");
            //} else {
            //printf("No se pudo eliminar la region.\n");
            //}
            break;
        case 3:
            // Mostrar todas las regiones
            mostrarRegiones();
            break;
        case 4:
            // Salir del menú
            break;
        default:
            printf("Opcion invalida. Por favor, seleccione una opcion valida.\n");
            break;
        }
    } while (opcion != 4);
}

// Función para crear una nueva región
void crearRegion(Region *region) {
  // Solicitar datos al usuario
  // ...

  // Almacenar la información en un archivo o estructura de datos
  // ...

    printf("Región creada exitosamente.\n");
}

// Función para mostrar todas las regiones
void mostrarRegiones(void) {
  // Leer las regiones del archivo o estructura de datos
  // ...

  // Imprimir la información de cada región
  // ...

    printf("Se han mostrado todas las regiones.\n");
}

// Función para eliminar una región
void eliminarRegion(char *nombre) {
  // Validar que la región no esté relacionada con otros datos
  // ...

  // Eliminar la región del archivo o estructura de datos
  // ...

    printf("Región eliminada exitosamente.\n");
}
