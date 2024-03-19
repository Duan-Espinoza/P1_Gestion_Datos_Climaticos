#include "headers/correlacion.h"
#include <math.h>

//Se requiere una funcionalidad previa que filtre los datos obtenidos de daatos_climaticos,json

double calcularCorrelacionPearson(double *x, double *y, int n) {
    double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0;
    double sum_x_squared = 0.0, sum_y_squared = 0.0;

    for (int i = 0; i < n; i++) {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_x_squared += x[i] * x[i];
        sum_y_squared += y[i] * y[i];
    }

    double correlacion_numerador = n * sum_xy - sum_x * sum_y;
    double correlacion_denominador = sqrt((n * sum_x_squared - sum_x * sum_x) * (n * sum_y_squared - sum_y * sum_y));

    if (correlacion_denominador == 0) {
        return 0; // Evitar división por cero
    }

    return correlacion_numerador / correlacion_denominador;
}
