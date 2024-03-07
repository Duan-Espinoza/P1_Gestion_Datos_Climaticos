#ifndef STRUCT_DATO_CLIMATICO_H
#define STRUCT_DATO_CLIMATICO_H
struct datoClimatico{
    int id; 
    char *region;
    char *fecha;         // yyyy-mm-dd
    char *hora;          // hh:mm
    float temperatura;  // celsius
    float humedad;      // %
    float presion;      // hPa
    int velcdViento;    // hm/h
    char *dirViento;     // N,S,O,E,NW,NE
    int precipitacion;  // mm    
}typedef datoClimatico;
#endif