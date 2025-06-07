#ifndef GANTT_H
#define GANTT_H

#include "procesos.h" // Incluye la definición de la estructura Proceso

// Función que recibe un arreglo de procesos y su cantidad,
// y muestra la representación gráfica del diagrama de Gantt.
void mostrarGantt(Proceso* procesos, int cantidad);

#endif // GANTT_H