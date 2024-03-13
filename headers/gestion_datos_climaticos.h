#ifndef GESTION_DATOS_CLIMATICOS_H
#define GESTION_DATOS_CLIMATICOS_H

#include "cjson/cJSON.h"
#include "structs_datos_climaticos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
//ruta al archivo json
const char *path_JSONDatosClimaticos = "datos\\datos_climaticos.json";
datoClimatico *getArrayDatosClimaticos();
void extraerCsv(const char *path);
bool determinarLinea(char *pLinea);
char* getContenido(const char *pathCsv);
void insertarDatoClimatico(datoClimatico pDatoClimatico);
bool nComasReq(char *pLinea);
int getID_UltimoIdDatoClimatico();
bool cronologiaCorrecta(char *pFecha, char *phora);
bool formatoHoraCorrecto(char *pHora);
bool formatoFechaCorrecto(char *pFecha);
bool esMesLargo(int pMes);
bool esAtributoInvalido(char *pAtributo, bool esFloat);
bool esDirCorrecta(char *atributo);

/** 
 * Accede al archivo Json para parsear los datos existentes
 * y retornar un arreglo con todos los registros de datos climaticos, retorna null si no hay
*/
datoClimatico *getArrayDatosClimaticos(){

    datoClimatico* arrayDatosClimaticos = (datoClimatico*) malloc(sizeof(datoClimatico));
    char *archivoJson = getContenido(path_JSONDatosClimaticos);
    cJSON* json_obj = cJSON_Parse(archivoJson);
    if(cJSON_GetArraySize(json_obj) == 0){
        return NULL;
    }
    cJSON* element;
    int cantDatosClimaticos = 0;
    cJSON_ArrayForEach(element,json_obj){
        cantDatosClimaticos++;
        datoClimatico nuevoDatoClimatico ;
        nuevoDatoClimatico.id = (cJSON_GetObjectItemCaseSensitive(element,"id"))->valueint;
        nuevoDatoClimatico.region = strdup((cJSON_GetObjectItemCaseSensitive(element,"region"))->valuestring);
        nuevoDatoClimatico.fecha = strdup((cJSON_GetObjectItemCaseSensitive(element,"fecha"))->valuestring);
        nuevoDatoClimatico.hora = strdup((cJSON_GetObjectItemCaseSensitive(element,"hora"))->valuestring);
        nuevoDatoClimatico.temperatura = (cJSON_GetObjectItemCaseSensitive(element,"temperatura"))->valuedouble;
        nuevoDatoClimatico.humedad = (cJSON_GetObjectItemCaseSensitive(element,"humedad"))->valuedouble;
        nuevoDatoClimatico.presion = (cJSON_GetObjectItemCaseSensitive(element,"presion"))->valuedouble;
        nuevoDatoClimatico.velcdViento = (cJSON_GetObjectItemCaseSensitive(element,"velcdViento"))->valueint;
        nuevoDatoClimatico.dirViento = strdup((cJSON_GetObjectItemCaseSensitive(element,"dirViento"))->valuestring);
        nuevoDatoClimatico.precipitacion = (cJSON_GetObjectItemCaseSensitive(element,"precipitacion"))->valueint;
        arrayDatosClimaticos = (datoClimatico*)realloc(arrayDatosClimaticos,cantDatosClimaticos * sizeof(datoClimatico));
        arrayDatosClimaticos[cantDatosClimaticos-1] = nuevoDatoClimatico;
        
    };
    return arrayDatosClimaticos;
}
/** 
 * Se encarga de solicitar la ruta del archivo por medio de la consola
 * Esta ruta se envía como un const char* 
*/
void gestionDatosClimaticos(){
    char path[200];
    printf("-Por favor, ingrese la direccion del archivo.csv: ");
    scanf("%s", &path); 
    const char* pathArchivo = path;
    
    extraerCsv(pathArchivo);
    
    
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
 * Se encarga de hacer la consulta en archivo.json y retornar el 
 * id del último elemento. Se utiliza para conocer el último registro agregado 
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
    
}
/** 
 * Recibe una cadena de caracteres e indica si tiene info o no
 * se le indica por parámetro si valida de tipo int o float
*/
bool esAtributoInvalido(char *pAtributo, bool esFloat){
    if(esFloat){        
        if(strspn(pAtributo, ".0123456789") == 0 && strspn(pAtributo, ".0123456789") != strlen(pAtributo)){return true;}
        //validación de punto 
        bool puntoUbicado = false;
        for(char *chr = pAtributo; *chr != '\0' && *chr != '\n'; *chr++){
            if(*chr == '.'){
                if(puntoUbicado){return false;}
                if(*chr == pAtributo[strlen(pAtributo)-1]){return false;}
                puntoUbicado = true;
            }
        }
    }else{               
        if (strchr(pAtributo, '.') != NULL){ return true;}
        if(strspn(pAtributo, "0123456789") == 0 && strspn(pAtributo, "0123456789") != strlen(pAtributo)){return true;}       
    }    
    return false;
}
/** 
 * Recibe un arreglo de caracteres con información que debería ser
 * la del dirección del viento, si esto no es correcto retorna false
*/
bool esDirCorrecta(char *atributo){
    if(strcmp(atributo,"N") == 0 || strcmp(atributo,"S") == 0 || strcmp(atributo,"E") == 0 || strcmp(atributo,"W") == 0 ||
    strcmp(atributo,"NE") == 0 || strcmp(atributo,"NW") == 0 || strcmp(atributo,"SE") == 0 || strcmp(atributo,"SW") == 0){        
        return true;
    }    
    return false;
}

/** 
 * Recibe una línea y comprueba si tiene 8 comas, para hacer 
 * constar de que pueden existir 9 campos 
*/
bool nComasReq(char *pLinea){
    bool resultado = false;
    int contComas = 0;
    
    int largo = strlen(pLinea)+1;
    for(char *chr = pLinea; *chr != '\n' && *chr != '\0'; *chr++){
        if (*chr == ','){contComas++;}
    }
    
    if(contComas == 8){
        
        return true;
    }   
    return false;
        
}
/** 
 * recibe una linea que contiene los valores de gestion de datos 
 * climáticos, retorna un valor booleano según si se agrega una linea o no  
*/
bool determinarLinea(char* pLinea){
    printf("\n->[Nueva Linea] = %s",pLinea);
    // Valida cantidad de comas,suguiere que no hay suficientes datos
    if(!nComasReq(pLinea)){
        return false;
    }
    
    datoClimatico nuevoDatoClimatico;
    int indiceAtributo = 1;
    char *atributo = NULL;
    char *fechaAtributo;
    char *horaAtributo;
    size_t tamanoAtributo = 0;

    int largo = strlen(pLinea)+1;
    for(char *chr = pLinea; largo !=0; *chr++){
        tamanoAtributo++;        
        
        if(*chr == ','|| *chr == '\0'){
            atributo = (char*)realloc(atributo,tamanoAtributo);
            atributo[tamanoAtributo-1] = '\0';
            
            switch (indiceAtributo){        
                
                case 1: //region
                    /*validacion pendiente*/
                    // Buscar existencia en el archivo de regiones
                    nuevoDatoClimatico.region = strdup(atributo);//copia dinamicamente una cadena
                    //printf(" ->[Region] = [ok]");
                    break;
                
                case 2: //fecha
                    //validacion de formato
                    if(! formatoFechaCorrecto(atributo)) { return false;}
                    fechaAtributo = strdup(atributo);
                    nuevoDatoClimatico.fecha = strdup(atributo);//copia dinamicamente una cadena            
                    
                    break;
                
                case 3://hora
                    // validacion de formato
                    if(! formatoHoraCorrecto(atributo)) {  return false;}
                    horaAtributo = strdup(atributo);   
                    // verifica si no es una fecha posterior a la actual
                    if(!cronologiaCorrecta(fechaAtributo, horaAtributo)){ return false;}
                    
                    nuevoDatoClimatico.hora = strdup(atributo);//copia dinamicamente una cadena  
                    break;
                
                case 4: //temperatura   puede ir vacía                    
                    float v_temperatura = atof(atributo); //array to float
                    if(esAtributoInvalido(atributo,true)){return false;}
                    nuevoDatoClimatico.temperatura = v_temperatura;                    
                    
                    break;
                
                case 5: //humedad       puede ir vacía                    
                    float v_humedad = atof(atributo); //array to float
                    if(esAtributoInvalido(atributo,true)){ return false;}
                    nuevoDatoClimatico.humedad = v_humedad;
                    
                    break;
                
                case 6: //presion       puede ir vacía
                    
                    float v_presion = atof(atributo); //array to float
                    if(esAtributoInvalido(atributo,true)){ return false;}
                    nuevoDatoClimatico.presion = v_presion;
                    
                    break;
                
                case 7: //velcdViento   puede ir vacía
                    
                    int v_velcdViento = atoi(atributo); //array to int
                    if(esAtributoInvalido(atributo,false)){ return false;}
                    nuevoDatoClimatico.velcdViento = v_velcdViento;
                    
                    break;
                
                case 8: //dirViento
                    
                    if(!esDirCorrecta(atributo)){ return false;}
                    nuevoDatoClimatico.dirViento = strdup(atributo); //copia dinamicamente una cadena   
                       
                    break;
                
                case 9: //precipitacion puede ir vacía
                    
                    int v_precipitacion = atoi(atributo); //array to int
                    if(esAtributoInvalido(atributo,false)){ return false;}
                    nuevoDatoClimatico.precipitacion = v_precipitacion;
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
    free(fechaAtributo);
    free(horaAtributo);
    insertarDatoClimatico(nuevoDatoClimatico);
    
    return true;
} 
/** 
 * Recibe 2 arreglos de caracteres con los datos de fecha y hora e indica  
 * si esa fecha es anterior a la fecha actual
*/ 
bool cronologiaCorrecta(char *pFecha, char *pHora){
    //Carga datos de fecha
    char anio[5];
    memcpy(anio,pFecha,4);
    anio[5] = '\0';    
    int i_anio = atoi(anio); 
    
    char mes[3];    
    memcpy(mes,pFecha + 5,2);
    mes[2] = '\0';
    int i_mes = atoi(mes); 
    
    char dia[3];    
    memcpy(dia,pFecha + 8,2);
    dia [2] = '\0';
    int i_dia = atoi(dia);

    //Carga datos de hora
    char hora[3];
    memcpy(hora, pHora, 2);
    hora[2] = '\0';
    int i_hora = atoi(hora);

    char mins[3];
    memcpy(mins, pHora+3, 2);
    mins[2] = '\0';    
    int i_mins = atoi(mins);

    //Carga fecha actual 
    struct tm *pointr;
    time_t fechaActual = time(NULL); 
    pointr = localtime(&fechaActual);
    char anioActual[5];
    strftime(anioActual,sizeof(anioActual),"%Y",pointr); 
    anioActual[4] = '\0';
    int i_anioActual = atoi(anioActual);

    if (i_anio > i_anioActual) {                            //Año
        return false;
    }else if(i_anio == i_anioActual){
        char mesActual [3];
        strftime(mesActual,sizeof(mesActual),"%m",pointr);
        mesActual[2] = '\0';
    
        int i_mesActual = atoi(mesActual);
        if (i_mes > i_mesActual){                           //mes
            return false;
        } else if (i_mes == i_mesActual){
            char diaActual [3];
            strftime(diaActual,sizeof(diaActual),"%d",pointr);
            diaActual[2] = '\0';

            int i_diaActual = atoi(diaActual);
            if (i_dia > i_diaActual){                       //dia
                return false;
            }else if(i_dia == i_diaActual){
                char horaActual [3];
                strftime(horaActual,sizeof(horaActual),"%H",pointr);
                horaActual[2] = '\0';

                int i_horaActual = atoi(horaActual);
                if(i_hora > i_horaActual){                  //hora
                    return false;
                } else if(i_hora == i_horaActual){
                    char minsActual [3];
                    strftime(minsActual,sizeof(minsActual),"%M",pointr);
                    minsActual[2] = '\0';

                    int i_minsActual = atoi(minsActual);
                    if (i_mins > i_minsActual){             //minutos
                        return false;
                    }
                }
            }
        }
    }
    
    //printf("\nAnio actual: %s\n",anioActual);
    return true;
}
/** 
 * Recibe una cadena de caracteres y pretende corroborar que el formato de 
 * la hora sea el especificado 
 * retorna valor bool (hh:mm)
*/ 
bool formatoHoraCorrecto(char *pHora){
    if (pHora[2] != ':'){return false;} // valida el separador
    char hora[3];
    memcpy(hora, pHora, 2);
    hora[2] = '\0';
    if(strspn(hora, "0123456789") != strlen(hora)){return false;}   
    int i_hora = atoi(hora);
    char mins[3];
    memcpy(mins, pHora+3, 2);
    mins[2] = '\0';  
    if(strspn(mins, "0123456789") != strlen(mins)){return false;}   
    int i_mins = atoi(mins);
    if((i_hora > 23) || (i_hora < 0) || (i_mins > 59) || (i_mins < 0)){return false;}
    return true;
}
/** 
 * Recibe una cadena de caracteres y pretende corroborar que el formato de 
 * la fecha sea el especificado 
 * retorna valor bool (yyyy-mm-dd)
*/
bool formatoFechaCorrecto(char *pFecha){
    bool esBisiesto = false;
    //parsear estructura de fecha
    if(pFecha[4] != '-' ||pFecha[7] != '-' ){        
        return false;
    }
    //valida año
    char anio[5];
    memcpy(anio,pFecha,4);
    anio[5] = '\0'; 

    if(strspn(anio, "0123456789") != strlen(anio)){return false;} 
    int i_anio = atoi(anio);
    if((i_anio % 4 == 0 && i_anio )|| (i_anio % 400 == 0)){ esBisiesto = true;}   
    
    //valida mes
    char mes[3];    
    memcpy(mes,pFecha + 5,2);
    mes[2] = '\0';

    if(strspn(mes, "0123456789") != strlen(mes)){return false;}  
    int i_mes = atoi(mes); 
    if ( (i_mes > 12) || (i_mes < 1)) { return false; }

    //valida día 
    // 31 días: enero, marzo, mayo, julio, agosto, octubre y diciembre
    // 30 días: abril, junio, septiembre y noviembre
    char dia[3];    
    memcpy(dia,pFecha + 8,2);
    dia [2] = '\0';

    if(strspn(dia, "0123456789") != strlen(dia)){return false;}    
    int i_dia = atoi(dia);
    if ( (i_dia > 31) || (i_dia == 31 && !(esMesLargo(i_mes))) || (i_dia < 1)) { return false; } // Valida 31s 30s y negativos
    if ((i_mes == 2 && i_dia > 29 && esBisiesto )|| (i_mes == 2 && i_dia > 28 && !esBisiesto )) { return false; } // valida el mes de febrero y sus factores

    return true;
}
/** 
 * Verifica si un número entero es un mes que llega hasta 31 o no
*/
bool esMesLargo(int pMes){
    switch (pMes){
        case 1:
            return true;
            break;
        case 3:
            return true;
            break;
        case 5:
            return true;
            break;
        case 7:
            return true;
            break;
        case 8:
            return true;
            break;
        case 10:
            return true;
            break;
        case 12:
            return true;
            break;
        default:
            break;
    }
    return false;
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
    int cantLineasReportadas = 0;
    char **lineasReportadas = (char**)malloc(sizeof(char*));
    printf("\n A continuacion se hace lectura de los datos del archivo ingresado \n");
    for (char* chr = contenido; finDelBucle != true; chr++){
        tamanoLinea++;
        
        if(*chr == '\n'|| *chr == '\0'){ 
            //Final de  linea
            linea = (char*) realloc(linea,tamanoLinea);
            linea[tamanoLinea-1] = '\0';
            
            if (!determinarLinea(linea)){
                cantLineasReportadas++;
                lineasReportadas = (char**) realloc(lineasReportadas,cantLineasReportadas);
                lineasReportadas[cantLineasReportadas-1] = (char*)malloc(sizeof(linea));
                lineasReportadas[cantLineasReportadas-1] = strdup(linea);
            }
            
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
    printf("\n\nTotal de lineas reportadas por error [%d]\n\n",cantLineasReportadas);
    
    for(int i = 0; i < cantLineasReportadas; i++){
        char *linea = (char*)malloc(sizeof(char));    
        int tamanoLinea = 0;
        for(char *chr = lineasReportadas[i]; *chr != '\0'; *chr++){
            tamanoLinea++;
            linea = (char*)realloc(linea, tamanoLinea);
            linea[tamanoLinea-1] = *chr;
        }
        tamanoLinea+=2;
        linea = (char*)realloc(linea, tamanoLinea);
        linea[tamanoLinea-2] = '\n';
        linea[tamanoLinea-1] = '\0';
        printf("->%s",linea);
        
    }
}

#endif