#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "procesos.h"

int cargarProcesos(const char *filename, Proceso procesos[], int maxProcesos) {
    FILE *archivo = fopen(filename, "r");
    if (!archivo) {
        perror("Error al abrir archivo de procesos");
        return 0;
    }

    int count = 0;
    while (fscanf(archivo, "%[^,], %d, %d, %d\n",
                  procesos[count].pid,
                  &procesos[count].burst_time,
                  &procesos[count].arrival_time,
                  &procesos[count].priority) == 4) {
        procesos[count].remaining_time = procesos[count].burst_time;
        procesos[count].start_time = -1;
        procesos[count].finish_time = -1;
        procesos[count].waiting_time = 0;
        procesos[count].turnaround_time = 0;
        count++;
        if (count >= maxProcesos) break;
    }

    fclose(archivo);
    return count;
}

void imprimirProcesos(Proceso procesos[], int cantidad) {
    printf("PID\tBT\tAT\tPR\n");
    for (int i = 0; i < cantidad; i++) {
        printf("%s\t%d\t%d\t%d\n", procesos[i].pid, procesos[i].burst_time, procesos[i].arrival_time, procesos[i].priority);
    }
}
