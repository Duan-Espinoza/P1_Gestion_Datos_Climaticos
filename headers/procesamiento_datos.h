#ifndef PROCESAMIENTO_DATOS_H
#define PROCESAMIENTO_DATOS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "./gestion_datos_climaticos.h"
#include "./structs_datos_climaticos.h"
#include "cjson/cJSON.h"
void procesamientoDatos();
void completadoRegDatos();
void eliminarRegDuplicados();
void eliminarRegAtipicos();
void imprimirArrayDatosClimaticos(datoClimatico *pDatosClimaticos);
bool sonDuplicados(datoClimatico *datoCmp1, datoClimatico *datoCmp2);
void imprimirDatoClimatico (datoClimatico *pDatoClimatico);
bool estaEnArreglo (int pElemento, int *pArreglo, int pCantElementos);
/** 
 * Llama a la función que retorna un array con los structs dato climatico  
 * desde los registros en el archivo json e imprime todos los registros
*/
void imprimirArrayDatosClimaticos(datoClimatico *pDatosClimaticos){
    if(pDatosClimaticos == NULL){printf("\nNo hay registros\n"); return;}
    printf("\nLista de registros\n");

    for (datoClimatico *datoClimatico = pDatosClimaticos; datoClimatico->id != 0; datoClimatico++ ){       
        imprimirDatoClimatico(datoClimatico);
    }
    
}
/** 
 * Imprime en la consola los elementos de un datoClimatico
*/
void imprimirDatoClimatico (datoClimatico *pDatoClimatico){
    printf("\nID de registro = [%d]\n",pDatoClimatico->id);
    printf("Region = [%s]\n",pDatoClimatico->region);
    printf("Fecha = [%s] ",pDatoClimatico->fecha);
    printf("| hora = [%s]\n",pDatoClimatico->hora);
    printf("Temperatura = [%1f]\n",pDatoClimatico->temperatura);
    printf("Humedad = [%1f]\n",pDatoClimatico->humedad);
    printf("Presion = [%1f]\n",pDatoClimatico->presion);
    printf("Velocidad del viento = [%d]\n",pDatoClimatico->velcdViento);
    printf("Direccion del viento = [%s]\n",pDatoClimatico->dirViento);
    printf("Precipitacion = [%d]\n",pDatoClimatico->precipitacion);
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
    //se extraen los structs
    datoClimatico *datosClimaticos = getArrayDatosClimaticos();
    int cantIdsPorBorrar = 0;
    int *idsPorBorrar = malloc(1* sizeof(int));
    //se hace busqueda a través de ciclos anidados y comparaciones
    for(datoClimatico *actual_datoClimatico = datosClimaticos; actual_datoClimatico->id != 0; actual_datoClimatico++ ){
        //Verifica que el elemento actual no sea uno que anteriormente haya sido marcado como por eliminar
        bool datoBorrado = estaEnArreglo (actual_datoClimatico->id, idsPorBorrar, cantIdsPorBorrar);         
        if (!datoBorrado){            
            for(datoClimatico *comparado_datoClimatico = actual_datoClimatico+1; comparado_datoClimatico->id != 0; comparado_datoClimatico++){                
                if( sonDuplicados(actual_datoClimatico, comparado_datoClimatico) ){
                    cantIdsPorBorrar++;
                    idsPorBorrar = realloc(idsPorBorrar,cantIdsPorBorrar * sizeof(int));
                    idsPorBorrar[cantIdsPorBorrar-1] = comparado_datoClimatico->id;
                    printf("\nRegistros duplicados encontrados\n");
                    printf("- Primer registro:\n");
                    imprimirDatoClimatico(actual_datoClimatico);                    
                    printf("- Registro duplicado:\n");
                    imprimirDatoClimatico(comparado_datoClimatico);    
                }
            }
        }
    }
    char *contenidoJson = getContenido(path_JSONDatosClimaticos);
    cJSON *json_obj = cJSON_Parse(contenidoJson);
    if(cJSON_GetArraySize(json_obj) == 0){
        return ;
    }
    //Se eliman del json
    cJSON* element;
    for(int i = 0; i < cJSON_GetArraySize(json_obj); i++){
        element = cJSON_GetArrayItem(json_obj,i);
        int idElementoActual = (cJSON_GetObjectItemCaseSensitive(element,"id"))->valueint;
        if(estaEnArreglo(idElementoActual,idsPorBorrar, cantIdsPorBorrar)){
            cJSON_DeleteItemFromArray(json_obj,i);
            printf("\n *Registro [id=%d] eliminado*\n",idElementoActual);
            i--;
        }
    }
    FILE* archivoJSON = fopen(path_JSONDatosClimaticos,"w");
    char* str_datos_climaticos = cJSON_Print(json_obj);
    fprintf(archivoJSON, "%s",str_datos_climaticos);
    fclose(archivoJSON);
    //Se limpia la memoria 

    free(str_datos_climaticos);
    
    
}

/** 
 * Recibe dos enteros y un arreglo de enteros (un id, los ids por borrar y la cantidad de ids)
 * retorna true o false si el elemento se encuentra
*/
bool estaEnArreglo (int pElemento, int *pArreglo, int pCantElementos){
    for(int indexId = 0; indexId < pCantElementos; indexId++){
        if( pElemento == pArreglo[indexId]  ) {return true; }
    }
    return false;
 }
/** 
 * Recibe dos structs como parámetro, debe retornar true o false 
 * si los dos registros cuentan con todos sus atributos iguales a excepecion del id 
*/
bool sonDuplicados(datoClimatico *datoCmp1, datoClimatico *datoCmp2){
    //Se comparan sus atributos de manera paralela
    if( strcmp( datoCmp1->region, datoCmp2->region) != 0 ){return false; }
    if( strcmp( datoCmp1->fecha, datoCmp2->fecha) != 0){return false; }
    if( strcmp( datoCmp1->hora, datoCmp2->hora) != 0){return false; }
    if( datoCmp1->temperatura != datoCmp2->temperatura ){return false; }
    if( datoCmp1->humedad != datoCmp2->humedad ){return false; }
    if( datoCmp1->presion != datoCmp2->presion ){return false; }
    if( datoCmp1->velcdViento != datoCmp2->velcdViento ){return false; }
    if( strcmp( datoCmp1->dirViento, datoCmp2->dirViento)!=0 ){return false; }
    if( datoCmp1->precipitacion != datoCmp2->precipitacion ){return false; }
    return true;
}
/** 
 * Se encarga de acceder a los registros del archivo json y analizar los datos atípicos
 * que se hayan introducido en el sistema con base en los algoritmos propuestos
*/
void eliminarRegAtipicos(){

}
/** 
 * Se encarga de gestionar el menú de procesamiento de datos del sistema
 * Atiende a las entradas de texto del usuario 
*/
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