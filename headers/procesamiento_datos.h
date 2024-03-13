#ifndef PROCESAMIENTO_DATOS_H
#define PROCESAMIENTO_DATOS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "./gestion_datos_climaticos.h"
#include "./structs_datos_climaticos.h"
void procesamientoDatos();
void completadoRegDatos();
void eliminarRegDuplicados();
void eliminarRegAtipicos();
void imprimirArrayDatosClimaticos(datoClimatico *pDatosClimaticos);
/** 
 * Llama a la función que retorna un array con los structs dato climatico  
 * desde los registros en el archivo json y los imprime
*/
void imprimirArrayDatosClimaticos(datoClimatico *pDatosClimaticos){
    if(pDatosClimaticos == NULL){printf("\nNo hay registros\n"); return;}
    printf("\nLista de registros\n");
    for (datoClimatico *datoClimatico = pDatosClimaticos; datoClimatico != NULL; datoClimatico++ ){
        if(datoClimatico->id == 0){
            return;
        }
        printf("\nID de registro = [%d]\n",datoClimatico->id);
        printf("Region = [%s]\n",datoClimatico->region);
        printf("Fecha = [%s] ",datoClimatico->fecha);
        printf("| hora = [%s]\n",datoClimatico->hora);
        printf("Temperatura = [%1f]\n",datoClimatico->temperatura);
        printf("Humedad = [%1f]\n",datoClimatico->humedad);
        printf("Presion = [%1f]\n",datoClimatico->presion);
        printf("Velocidad del viento = [%d]\n",datoClimatico->velcdViento);
        printf("Direccion del viento = [%s]\n",datoClimatico->dirViento);
        printf("Precipitacion = [%d]\n",datoClimatico->precipitacion);
    }
    
}
/** 
 * Se encarga de buscar los registros del archivo json para completar los datos 
 * mediante alguno de los tres algoritmos propuestos
*/
void completadoRegDatos(){
    //PARA EJEMPLO DE USO DE EXTRAER E IMPRIMIR LOS DATOS
    datoClimatico *datosCl = getArrayDatosClimaticos();
    imprimirArrayDatosClimaticos(datosCl);
}
/** 
 * Se encarga de analizar los registros del archivo json en búsqueda de registros  
 * con la TOTALIDAD de sus datos iguales
*/
void eliminarRegDuplicados(){
    
}
/** 
 * Se encarga de acceder a los registros del archivo json y analizar los datos atípicos
 * que se hayan introducido en el sistema con base en los algoritmos propuestos
*/
void eliminarRegAtipicos(){

}

void procesamientoDatos(){    
    bool salir = false;
    while(!salir){
        char entrada;
        printf("\nMenu de procesamiento de datos\n");
        printf("A. Completado de datos\nB. Eliminar datos duplicados\nC. Eliminacion de registros atipicos\nS. Salir\n\n");
        printf("->Seleccione una opcion: ");
        scanf(" %[^\t\n]c",&entrada);        
        switch (entrada)
        {
        case 'A':            
            completadoRegDatos();
            
            break;
        case 'B':
            eliminarRegDuplicados();

            break;
        case 'C':
            eliminarRegAtipicos();
            break;   
        case 'S':
            salir = true;
            break;     
        default:
            printf("\nError, opcion no existe, intente de nuevo\n");
            break;
        }
    }
    
}
#endif