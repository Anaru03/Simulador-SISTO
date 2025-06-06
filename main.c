#include <stdio.h>
#include "procesos.h"
#include "algoritmos.h"
#include "gantt.h"

#define MAX_PROCESOS 100

int main() {
    Proceso procesos[MAX_PROCESOS];
    int cantidad = leerProcesos("procesos.txt", procesos, MAX_PROCESOS);

    if (cantidad == 0) {
        printf("No se pudieron leer procesos.\n");
        return 1;
    }

    // Ejecutar algoritmo FIFO
    fifo(procesos, cantidad);

    // Mostrar diagrama de Gantt
    mostrarGantt(procesos, cantidad);

    return 0;
}
