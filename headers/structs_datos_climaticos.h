#ifndef STRUCTS_DATOS_CLIMATICOS_H
#define STRUCTS_DATO_CLIMATICOS_H
struct datoClimatico{
    int id; 
    char *region;
    char *fecha;         // yyyy-mm-dd
    char *hora;          // hh:mm
    float temperatura;  // celsius
    float humedad;      // %
    float presion;      // hPa
    int velcdViento;    // hm/h
    char *dirViento;     //N, S, W, E, NW, NE, SW, SE
    int precipitacion;  // mm    
}typedef datoClimatico;
#endif