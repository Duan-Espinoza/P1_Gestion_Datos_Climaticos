#ifndef GESTION_DATOS_CLIMATICOS_H
#define GESTION_DATOS_CLIMATICOS_H

#include "cjson/cJSON.h"
#include "struct_dato_climatico.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//ruta al archivo json
const char *path_JSONDatosClimaticos = "datos\\datos_climaticos.json";

void extraerCsv(const char *path);
bool determinarLinea(char *pLinea);
char* getContenido(const char *pathCsv);
void insertarDatoClimatico(datoClimatico pDatoClimatico);
int getID_UltimoIdDatoClimatico();
/** 
 * Se encarga de solicitar la ruta del archivo por medio de la consola
 * Esta ruta se envía como un const char* 
*/
int gestionDatosClimaticos(){
    char path[200];
    printf("-Por favor, ingrese la direccion del archivo.csv: ");
    scanf("%s", &path);
    printf("\n A continuación se hace lectura de los datos del archivo ingresado \n");
    const char* pathArchivo = path;
    extraerCsv(pathArchivo);
    return 0;
}
/** 
 * Se encarga de seguir la ruta para parsear el archivo 
 * y retornar el contenido del archivo 
*/
char* getContenido(const char *pathCsv){

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
/** 
 * Encargada de recibir el struct datoClimatico y
 * y agregarlo como un objeto JSON al archivo de persistencia de datos
*/
int getID_UltimoIdDatoClimatico(){
    char* archivoJson = getContenido(path_JSONDatosClimaticos);
    //printf("%s",archivoJson);
    cJSON* json_obj = cJSON_Parse(archivoJson);
    cJSON* element;
    int id = 0;
    cJSON_ArrayForEach(element,json_obj){
        cJSON* json_id = cJSON_GetObjectItemCaseSensitive(element,"id");
        id = json_id->valueint;
    };
    
    return id;
}
/** 
 * Encargada de recibir el struct datoClimatico y
 * y agregarlo como un objeto JSON al archivo de persistencia de datos
*/
void insertarDatoClimatico(datoClimatico pDatoClimatico){
    //Se crea un objeto JSON con los datos del struct
    cJSON* json_objClima = cJSON_CreateObject();
    int ultimoId = getID_UltimoIdDatoClimatico() + 1;
    cJSON_AddNumberToObject(json_objClima,"id",ultimoId);
    cJSON_AddStringToObject(json_objClima,"region",pDatoClimatico.region);
    cJSON_AddStringToObject(json_objClima,"fecha",pDatoClimatico.fecha);
    cJSON_AddStringToObject(json_objClima,"hora",pDatoClimatico.hora);
    cJSON_AddNumberToObject(json_objClima,"temperatura",pDatoClimatico.temperatura);
    cJSON_AddNumberToObject(json_objClima,"humedad",pDatoClimatico.humedad);
    cJSON_AddNumberToObject(json_objClima,"presion",pDatoClimatico.presion);
    cJSON_AddNumberToObject(json_objClima,"velcdViento",pDatoClimatico.velcdViento);
    cJSON_AddStringToObject(json_objClima,"dirViento",pDatoClimatico.dirViento);
    cJSON_AddNumberToObject(json_objClima,"precipitacion",pDatoClimatico.precipitacion);
    //Se obtiene el contenido de la data JSON para adjuntar el nuevo
    char* contenidoJSON = getContenido(path_JSONDatosClimaticos);
    cJSON* objetoJSON = cJSON_Parse(contenidoJSON);
    cJSON_AddItemToArray(objetoJSON,json_objClima);
    //Escritura sobre el archivo con el obj agregado 
    FILE* archivoJSON = fopen(path_JSONDatosClimaticos,"w");
    char* str_datos_climaticos = cJSON_Print(objetoJSON);
    fprintf(archivoJSON, "%s",str_datos_climaticos);
    fclose(archivoJSON);
    //Se limpia la memoria 
    free(str_datos_climaticos);
    cJSON_Delete(json_objClima);
    printf("[Agregado] = Si  \n");
}
/** 
 * recibe una linea que contiene los valores de gestion de datos 
 * climáticos, retorna un valor booleano según si se agrega una linea o no  
*/
bool determinarLinea(char* pLinea){
    printf("\n->[Nueva Linea] = %s ",pLinea);
    datoClimatico nuevoDatoClimatico;
    int indiceAtributo = 1;
    char *atributo = NULL;
    size_t tamanoAtributo = 0;

    int largo = strlen(pLinea)+1;
    for(char *chr = pLinea; largo !=0; *chr++){
        tamanoAtributo++;        
        
        if(*chr == ','|| *chr == '\0'){
            atributo = (char*)realloc(atributo,tamanoAtributo);
            atributo[tamanoAtributo-1] = '\0';
            
            switch (indiceAtributo){        
                
                case 1: //region
                    nuevoDatoClimatico.region = strdup(atributo);//copia dinamicamente una cadena
                    printf("\n[Region] = ok");
                    break;
                
                case 2: //fecha
                    /*validacion pendiente*/
                    nuevoDatoClimatico.fecha = strdup(atributo);//copia dinamicamente una cadena            
                    printf("\n[Fecha] = ok");
                    break;
                
                case 3://hora
                    /*validacion pendiente*/
                    nuevoDatoClimatico.hora = strdup(atributo);//copia dinamicamente una cadena            
                    printf("\n[Hora] = ok");
                    break;
                
                case 4: //temperatura   puede ir vacía
                    /*validacion pendiente*/
                    float v_temperatura = atof(atributo); //array to float
                    nuevoDatoClimatico.temperatura = v_temperatura;
                    printf("\n[Temperatura] = ok");
                    break;
                
                case 5: //humedad       puede ir vacía
                    /*validacion pendiente*/
                    float v_humedad = atof(atributo); //array to float
                    nuevoDatoClimatico.humedad = v_humedad;
                    printf("\n[Humedad] = ok");
                    break;
                
                case 6: //presion       puede ir vacía
                    /*validacion pendiente*/
                    float v_presion = atof(atributo); //array to float
                    nuevoDatoClimatico.presion = v_presion;
                    printf("\n[Presion]= ok");
                    break;
                
                case 7: //velcdViento   puede ir vacía
                    /*validacion pendiente*/
                    int v_velcdViento = atoi(atributo); //array to int
                    nuevoDatoClimatico.velcdViento = v_velcdViento;
                    printf("\n[Velocidad del viento] = ok");
                    break;
                
                case 8: //dirViento
                    /*validacion pendiente*/
                    nuevoDatoClimatico.dirViento = strdup(atributo);//copia dinamicamente una cadena   
                    printf("\n[Direccion del viento] = ok");         
                    break;
                
                case 9: //precipitacion puede ir vacía
                    /*validacion pendiente*/
                    int v_precipitacion = atoi(atributo); //array to int
                    nuevoDatoClimatico.precipitacion = v_precipitacion;
                    printf("\n[Precipitacion] = ok");
                    break;
                default:
                    break;
            }
            free(atributo);
            indiceAtributo++;
            atributo = NULL;
            tamanoAtributo = 0; 
        }else{ // se concatena al atributo
            atributo = (char*)realloc(atributo,tamanoAtributo);
            atributo[tamanoAtributo - 1] = *chr;
        }
        largo--;
    }
    printf("\n\n-> Datos de struct: \n");
    printf("- Region: %s\n",nuevoDatoClimatico.region);
    printf("- Fecha: %s\n",nuevoDatoClimatico.fecha);
    printf("- Hora: %s\n",nuevoDatoClimatico.hora);
    printf("- Temperatura: %f\n",nuevoDatoClimatico.temperatura);
    printf("- humedad: %f\n",nuevoDatoClimatico.humedad);
    printf("- Presion: %f\n",nuevoDatoClimatico.presion);
    printf("- Velocidad del viento: %d\n",nuevoDatoClimatico.velcdViento);
    printf("- Direccion del viento: %s\n",nuevoDatoClimatico.dirViento);
    printf("- Precipitacion: %d\n",nuevoDatoClimatico.precipitacion);
    insertarDatoClimatico(nuevoDatoClimatico);
    return true;
}  
/** 
 * Función que recibe el path correspondiente al archivo
 * csv para la gestión de datos climáticos
*/
void extraerCsv(const char* pathCsv){
    char* contenido =  getContenido(pathCsv);
    if(contenido == NULL){
        //Si estuviera vacío
        fprintf(stderr,"Error al leer el archivo.\n");
        return;
    }
    char* linea = NULL;
    size_t tamanoLinea = 0;
    bool finDelBucle = false;
    for (char* chr = contenido; finDelBucle != true; chr++){
        tamanoLinea++;
        
        if(*chr == '\n'|| *chr == '\0'){ 
            //Final de  linea
            linea = (char*) realloc(linea,tamanoLinea);
            linea[tamanoLinea-1] = '\0';
            
            bool ret = determinarLinea(linea);
            free(linea);
            linea = NULL;
            tamanoLinea = 0;
            if( *chr == '\0'){
                finDelBucle = true;
            }
        }else{
            
            linea = (char*)realloc(linea,tamanoLinea);
            linea[tamanoLinea-1] = *chr;
        }
    }
    free(contenido);
    free(linea);
}

#endif