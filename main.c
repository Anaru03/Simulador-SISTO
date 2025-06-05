#include <stdio.h>
#include "procesos.h"

#define MAX_PROCESOS 100

int main() {
    Proceso procesos[MAX_PROCESOS];
    int cantidad = cargarProcesos("procesos.txt", procesos, MAX_PROCESOS);

    if (cantidad == 0) {
        printf("No se cargaron procesos.\n");
        return 1;
    }

    printf("Procesos cargados:\n");
    imprimirProcesos(procesos, cantidad);

    // Aquí podrías llamar al algoritmo de calendarización (por ejemplo, FIFO)
    return 0;
}