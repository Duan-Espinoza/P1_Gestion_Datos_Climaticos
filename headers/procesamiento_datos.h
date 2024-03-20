#ifndef PROCESAMIENTO_DATOS_H
#define PROCESAMIENTO_DATOS_H
#include <math.h>
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
float getPromedioAtributosFloat(char *pNombreAtributo);
int getPromedioAtributosInt(char *pNombreAtributo);
void determinarNulosRegistro(datoClimatico *pDatoClimatico);
bool determinarRegistroAtipico(datoClimatico *pDatoClimatico);
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
    printf("\n->Los elementos con valor N/I corresponde a los que no fueron ingresados al registro\n");
    free(pDatosClimaticos);
}
/** 
 * Imprime en la consola los elementos de un datoClimatico
*/
void imprimirDatoClimatico (datoClimatico *pDatoClimatico){
    printf("\nID de registro = [%d]\n",pDatoClimatico->id);
    printf("Region = [%s]\n",pDatoClimatico->region);
    printf("Fecha = [%s] ",pDatoClimatico->fecha);
    printf("| hora = [%s]\n",pDatoClimatico->hora);
    
    if(!isnan(pDatoClimatico->temperatura )){        
        printf("Temperatura = [%f%cC]\n",pDatoClimatico->temperatura,0xF8);    
    }else{ printf("Temperatura = [N/I]\n"); }

    if(!isnan(pDatoClimatico->humedad)){
        printf("Humedad = [%f%]\n",pDatoClimatico->humedad);
    }else{ printf("Humedad = [N/I]\n"); }
    if(!isnan(pDatoClimatico->presion)){
        printf("Presion = [%fhPa]\n",pDatoClimatico->presion);
    }else{ printf("Presion = [N/I]\n"); }
    if(pDatoClimatico->velcdViento != -1){
        printf("Velocidad del viento = [%dkm/h]\n",pDatoClimatico->velcdViento);
    }else{ printf("Velocidad del viento = [N/I]\n"); }
    printf("Direccion del viento = [%s]\n",pDatoClimatico->dirViento);
    
    if( pDatoClimatico->precipitacion != -1){
        printf("Precipitacion = [%dmm]\n",pDatoClimatico->precipitacion);
    }else{ printf("Precipitacion = [N/I]\n"); }
    
    
}
/** 
 * Se encarga de buscar los registros del archivo json para completar los datos 
 * mediante alguno de los tres algoritmos propuestos
*/
void completadoRegDatos(){
    // Se extraen los datos
    datoClimatico *datosClimaticos = getArrayDatosClimaticos();
    // Recorrer arreglo de datosClimaticos
    for (datoClimatico *datoClimatico = datosClimaticos; datoClimatico->id != 0; datoClimatico++ ){       
        determinarNulosRegistro(datoClimatico); 
        
        
    }
                        
}

/** 
 * Se encarga de extraer un dato específico (indicado por parámetro) de cada uno de los registros que se 
 * encuentran en el archivo.json para calcular el promedio con base en la cantidad de datos dividido entre la cantidad de ellos
*/
float getPromedioAtributosFloat(char *pNombreAtributo){
    float promedio = 0.0;
    char *contenidoArchivoJson = getContenido(path_JSONDatosClimaticos);
    cJSON* json_objeto = cJSON_Parse(contenidoArchivoJson);
    cJSON* element;
    int cantidadElementos = 0;    
    cJSON_ArrayForEach(element, json_objeto){
        cJSON *atributoSolicitado = cJSON_GetObjectItemCaseSensitive(element,pNombreAtributo);
        if( strcmp(cJSON_Print(atributoSolicitado),"null") != 0){
            float atributoFloat = atributoSolicitado->valuedouble;
            promedio += atributoFloat;
            cantidadElementos++;
        }
    }
    //El promedio es la suma de los elementos de un conjunto dividido entre la cantidad de elementos   
    if (cantidadElementos == 0 ){return 0.0; }    
    return promedio/cantidadElementos;
}
/** 
 * Se encarga de extraer un dato específico (indicado por parámetro) de cada uno de los registros que se 
 * encuentran en el archivo.json para calcular el promedio con base en la cantidad de datos dividido entre la cantidad de ellos
*/
int getPromedioAtributosInt(char *pNombreAtributo){
    int promedio = 0;
    char *contenidoArchivoJson = getContenido(path_JSONDatosClimaticos);
    cJSON* json_objeto = cJSON_Parse(contenidoArchivoJson);
    cJSON* element;
    int cantidadElementos = 0;    
    cJSON_ArrayForEach(element, json_objeto){
        cJSON *atributoSolicitado = cJSON_GetObjectItemCaseSensitive(element,pNombreAtributo);
        if( strcmp(cJSON_Print(atributoSolicitado),"null") != 0){
            int atributoInt = atributoSolicitado->valueint;
            promedio += atributoInt;
            cantidadElementos++;
        }
    }
    //El promedio es la suma de los elementos de un conjunto dividido entre la cantidad de elementos       
    if (cantidadElementos == 0 ){return 0; }
    return promedio/cantidadElementos;
}
/** 
 * Se encarga de analizar los datos del archivo json en búsqueda de registros
 * con atributos vacíos para realizar la completación de los datos según la fórmula
*/
void determinarNulosRegistro(datoClimatico *pDatoClimatico){
    int cantidadCambios = 0;
    bool cambioRealizado = false;
    char **msj = (char**)malloc(sizeof(char*));
    //temperatura
    if(isnan(pDatoClimatico->temperatura)){
        //operacion
        float atributoTempGen = getPromedioAtributosFloat("temperatura");        
        pDatoClimatico->temperatura = atributoTempGen;
        
        char *ch_atributoTempGen = (char*) malloc ( 10 * sizeof(char));        
        sprintf(ch_atributoTempGen,"%1f",atributoTempGen);        
        char *lineaTemp = (char*) malloc ( (strlen(ch_atributoTempGen) + 50) * sizeof(char));

        strcat(lineaTemp, "\nTemperatura = [Valor promedio de temperatura] => ");
        strcat(lineaTemp, ch_atributoTempGen);
        msj[cantidadCambios] = strdup(lineaTemp);
        
        cantidadCambios++;
        cambioRealizado = true;
        free(ch_atributoTempGen);
    }
    //humedad
    if(isnan(pDatoClimatico->humedad)){        
        //operacion
        float atributoHumedadGen = getPromedioAtributosFloat("humedad");        
        pDatoClimatico->humedad = atributoHumedadGen;
        
        char *ch_atributoHumedadGen = (char*) malloc (10 * sizeof(char));
        sprintf(ch_atributoHumedadGen,"%1f",atributoHumedadGen);        
        char* lineaHumedad = (char*)malloc ( (strlen(ch_atributoHumedadGen) + 50) * sizeof(char));
        
        strcat(lineaHumedad, "\nHumedad = [Valor promedio de humedad] => ");
        strcat(lineaHumedad,ch_atributoHumedadGen); 
        msj[cantidadCambios] = strdup(lineaHumedad);
        
        cantidadCambios++;
        cambioRealizado = true;
        free(ch_atributoHumedadGen);
    }
    //presion
    if(isnan(pDatoClimatico->presion)){        
        //operacion
        float atributoPresionGen = getPromedioAtributosFloat("presion");        
        pDatoClimatico->presion = atributoPresionGen;
        
        char *ch_atributoPresionGen = (char*) malloc ( 10 * sizeof(char));
        sprintf(ch_atributoPresionGen,"%1f",atributoPresionGen);        
        char* lineaPresion = (char*) malloc ((strlen(ch_atributoPresionGen) + 50) * sizeof(char));

        strcat(lineaPresion, "\nPresion = [Valor promedio de presion] => ");
        strcat(lineaPresion,ch_atributoPresionGen); 
        msj[cantidadCambios] = strdup(lineaPresion);
        
        cantidadCambios++;
        cambioRealizado = true;
        free(ch_atributoPresionGen);
       
    }    
    //vecldViento
    if(pDatoClimatico->velcdViento == - 1){
        //operacion
        int atributoVelcdVientoGen = getPromedioAtributosInt("velcdViento");
        pDatoClimatico->velcdViento = atributoVelcdVientoGen;

        char *ch_atributoVelcdVientoGen = (char*)malloc(10*sizeof(char));
        sprintf(ch_atributoVelcdVientoGen,"%d",atributoVelcdVientoGen);
        char *lineaVelcdViento = (char*) malloc ((strlen(ch_atributoVelcdVientoGen) + 50) * sizeof(char));
        
        strcat(lineaVelcdViento, "\nVelocidad del viento = [Valor promedio de velocidad del viento] => ");
        strcat(lineaVelcdViento,ch_atributoVelcdVientoGen);
        msj[cantidadCambios] = strdup(lineaVelcdViento);
        
        cantidadCambios++;
        cambioRealizado = true;
        free(ch_atributoVelcdVientoGen);
        
    }
    //precipitacion
    if(pDatoClimatico->precipitacion == - 1){
        //operacion
        int atributoPrecipitacionGen = getPromedioAtributosInt("precipitacion");
        pDatoClimatico->precipitacion = atributoPrecipitacionGen;

        char *ch_atributoPrecipitacionGen = (char*)malloc(10*sizeof(char));
        sprintf(ch_atributoPrecipitacionGen,"%d",atributoPrecipitacionGen);
        char *lineaPrecipitacion = (char*) malloc ((strlen(ch_atributoPrecipitacionGen)+ 50) * sizeof(char));
        
        strcat(lineaPrecipitacion, "\nPrecipitacion = [Valor promedio de precipitacion] => ");
        strcat(lineaPrecipitacion,ch_atributoPrecipitacionGen);
        msj[cantidadCambios] = strdup(lineaPrecipitacion);
        
        cantidadCambios++;
        cambioRealizado = true;
        free(ch_atributoPrecipitacionGen);
    }
    if (cambioRealizado){
        printf("\nAl registro de dato climatico con el id = %d se le han completado los siguientes datos: ",pDatoClimatico->id);       
        for(int i = 0; i<cantidadCambios; i++){printf("\n%s",msj[i]);}              
        printf("\nRegistro modificado: \n");
        imprimirDatoClimatico(pDatoClimatico);
        free(msj); 
        char *contenidoJson = getContenido(path_JSONDatosClimaticos);
        cJSON *json_obj = cJSON_Parse(contenidoJson);
        cJSON *element;        
        for(int i = 0;i< cJSON_GetArraySize(json_obj);i++){
            element = cJSON_GetArrayItem(json_obj,i);
            //se verifica que sea el datoClimático actual y se corrobora con cada atributo que no este nulo
            if( pDatoClimatico->id == cJSON_GetObjectItemCaseSensitive(element,"id")->valueint){
                //temperatura
                cJSON * cjson_atributoTemperatura = cJSON_GetObjectItemCaseSensitive(element,"temperatura");         
                if(strcmp(cJSON_Print(cjson_atributoTemperatura),"null") == 0){                    
                    cJSON_DetachItemFromObject(element,"temperatura");
                    cJSON_AddNumberToObject(element,"temperatura", cjson_atributoTemperatura->valuedouble = pDatoClimatico->temperatura);
                    //cJSON_ReplaceItemInObjectCaseSensitive  (element,"temperatura",cjson_atributoTemperatura);
                    
                }
                //humedad
                cJSON * cjson_atributoHumedad = cJSON_GetObjectItemCaseSensitive(element,"humedad");         
                if(strcmp(cJSON_Print(cjson_atributoHumedad),"null") == 0){
                    cJSON_DetachItemFromObject(element,"humedad");
                    cJSON_AddNumberToObject(element,"humedad",pDatoClimatico->humedad);                 
                    //cJSON_ReplaceItemInObjectCaseSensitive  (element,"humedad",cjson_atributoHumedad);                    
                }
                //presion
                cJSON * cjson_atributoPresion = cJSON_GetObjectItemCaseSensitive(element,"presion");         
                if(strcmp(cJSON_Print(cjson_atributoPresion),"null") == 0){ 
                    cJSON_DetachItemFromObject(element,"presion");
                    cJSON_AddNumberToObject(element,"presion", pDatoClimatico->presion);                  
                    //cJSON_ReplaceItemInObjectCaseSensitive  (element,"presion",cjson_atributoPresion);                    
                }
                //velcdViento
                cJSON * cjson_atributoVelcdViento = cJSON_GetObjectItemCaseSensitive(element,"velcdViento");         
                if(strcmp(cJSON_Print(cjson_atributoVelcdViento),"null") == 0){
                    cJSON_DetachItemFromObject(element,"velcdViento");
                    cJSON_AddNumberToObject(element,"velcdViento",pDatoClimatico->velcdViento);                    
                    //cJSON_ReplaceItemInObjectCaseSensitive   (element,"velcdViento",cjson_atributoVelcdViento);                    
                }
                //precipitacion
                cJSON * cjson_atributoPrecipitacion = cJSON_GetObjectItemCaseSensitive(element,"precipitacion");                 
                if(strcmp(cJSON_Print(cjson_atributoPrecipitacion),"null") == 0){ 
                    cJSON_DetachItemFromObject(element,"precipitacion");
                    cJSON_AddNumberToObject(element,"precipitacion",pDatoClimatico->precipitacion); 
                    //cjson_atributoPrecipitacion->valueint  = 
                    //cJSON_ReplaceItemInObjectCaseSensitive   (element,"precipitacion",cjson_atributoPrecipitacion);
                }
                
                cJSON_ReplaceItemInArray(json_obj,i,element);
                break;
            }
        }
        FILE* archivoJSON = fopen(path_JSONDatosClimaticos,"w");
        char* str_datos_climaticos = cJSON_Print(json_obj);
        fprintf(archivoJSON, "%s",str_datos_climaticos);
        fclose(archivoJSON);
        //Se limpia la memoria         
        free(str_datos_climaticos); 
        

    }
    
}
/** 
 * Se encarga de analizar los datos del archivo json en búsqueda de registros  
 * con la TOTALIDAD de sus datos iguales para eliminar el duplicado
*/
void eliminarRegDuplicados(){
    //se extraen los structs
    
    datoClimatico *datosClimaticos = getArrayDatosClimaticos();
    if (datosClimaticos == NULL){
        printf("\nNo hay registros guardados actualmente\n");
        return;
    }
    int cantIdsPorBorrar = 0;
    int *idsPorBorrar = malloc(1* sizeof(int));
    //se hace busqueda a través de ciclos anidados y comparaciones
    for(datoClimatico *actual_datoClimatico = datosClimaticos; actual_datoClimatico->id != 0; actual_datoClimatico++ ){
        //Verifica que el elemento actual no sea uno que anteriormente haya sido marcado como por eliminar
        
        bool datoBorrado = estaEnArreglo (actual_datoClimatico->id, idsPorBorrar, cantIdsPorBorrar);         
        if (!datoBorrado){            
            for(datoClimatico *comparado_datoClimatico = actual_datoClimatico+1; comparado_datoClimatico->id != 0; comparado_datoClimatico++){                
                if( sonDuplicados(actual_datoClimatico, comparado_datoClimatico) ){
                    
                    idsPorBorrar = (int*) realloc(idsPorBorrar,(cantIdsPorBorrar+1) * sizeof(int));
                    idsPorBorrar[cantIdsPorBorrar] = comparado_datoClimatico->id;
                    cantIdsPorBorrar++;
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
    if(cantIdsPorBorrar==0){
        printf("\nNo hay registros por borrar actualmente\n");
        return;
    }
    //Se eliminan del json
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
 * que se hayan introducido en el sistema y con base en los algoritmos propuestos eliminarlos
*/
void eliminarRegAtipicos(){
    datoClimatico *datosClimaticos = getArrayDatosClimaticos();
     if (datosClimaticos == NULL){
        printf("\nNo hay registros guardados actualmente\n");
        return;
    }
    int cantIdsPorBorrar = 0;
    int *idsPorBorrar = malloc(1* sizeof(int));
    printf("\nSe va a analizar los registros en busqueda de datos atipicos para su eliminacion\n");
    for(datoClimatico *actual_datoClimatico = datosClimaticos; actual_datoClimatico->id != 0; actual_datoClimatico++ ){
        if (determinarRegistroAtipico(actual_datoClimatico)){            
            idsPorBorrar = (int*) realloc(idsPorBorrar,(cantIdsPorBorrar+1) * sizeof(int));
            idsPorBorrar[cantIdsPorBorrar] = actual_datoClimatico->id;
            cantIdsPorBorrar++;
        }
    }
    //imprimirArrayDatosClimaticos(datosClimaticos);
    char *contenidoJson = getContenido(path_JSONDatosClimaticos);
    cJSON *json_obj = cJSON_Parse(contenidoJson);
    if(cJSON_GetArraySize(json_obj) == 0){
        return ;
    }
    if(cantIdsPorBorrar==0){
        printf("\nNo hay registros por borrar actualmente\n");
        return;
    }

    //Se eliminan del json
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
 * Identifica un registro de dato climatico con atributos atípicos y retorna su correspondiente en 
 * valor de tipo bool
*/
bool determinarRegistroAtipico(datoClimatico *pDatoClimatico){
    //temperatura
    float promedioTemp = getPromedioAtributosFloat("temperatura");
    float resultadoTemp = abs(promedioTemp - (pDatoClimatico->temperatura) );
    if(resultadoTemp > 100.0){
        printf("\nAtributo temperatura:[%f] del registro id:[%d] atipico\n",pDatoClimatico->temperatura,pDatoClimatico->id);
        return true;
    }    
    //humedad
    float promedioHumedad = getPromedioAtributosFloat("humedad");
    float resultadoHumedad = abs(promedioHumedad - (pDatoClimatico->humedad) );
    if(resultadoHumedad > 100.0){
        printf("\nAtributo humedad:[%f] del registro id:[%d] atipico\n",pDatoClimatico->humedad,pDatoClimatico->id);
        return true;
    }    
    //presion
    float promedioPresion = getPromedioAtributosFloat("presion");
    float resultadoPresion = abs(promedioPresion - (pDatoClimatico->presion) );
    if(resultadoPresion > 100.0){
        printf("\nAtributo presion:[%f] del registro id:[%d] atipico\n",pDatoClimatico->presion,pDatoClimatico->id);
        return true;
    }

    //velocidad del viento 
    int promedioVelcdViento = getPromedioAtributosInt("velcdViento");
    int resultadoVelcdViento = abs(promedioVelcdViento - (pDatoClimatico->velcdViento) );
    if(resultadoVelcdViento > 100){
        printf("\nAtributo velocidad del viento:[%d] del registro id:[%d] atipico\n",pDatoClimatico->velcdViento,pDatoClimatico->id);
        return true;
    } 
    //precipitacion
    int promedioPrecipitacion = getPromedioAtributosInt("precipitacion");
    int resultadoPrecipitacion = abs(promedioPrecipitacion - (pDatoClimatico->precipitacion) );
    if(resultadoPrecipitacion > 100){
        printf("\nAtributo precipitacion:[%d] del registro id:[%d] atipico\n",pDatoClimatico->precipitacion,pDatoClimatico->id);
        return true;
    } 
    return false;
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