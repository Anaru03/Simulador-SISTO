#include <stdio.h>
#include "procesos.h"

int leerProcesos(const char* filename, Proceso* procesos, int max) {
    FILE* archivo = fopen(filename, "r");
    if (!archivo) {
        perror("Error al abrir procesos.txt");
        return 0;
    }

    int count = 0;
    while (count < max &&
           fscanf(archivo, " %[^,], %d, %d, %d",
                  procesos[count].pid,
                  &procesos[count].duracion,
                  &procesos[count].llegada,
                  &procesos[count].prioridad) == 4) {
        count++;
    }

    fclose(archivo);
    return count;
}