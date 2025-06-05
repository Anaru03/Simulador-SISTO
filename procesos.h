#ifndef PROCESOS_H
#define PROCESOS_H

typedef struct {
    char pid[10];
    int burst_time;
    int arrival_time;
    int priority;
    int remaining_time;
    int start_time;
    int finish_time;
    int waiting_time;
    int turnaround_time;
} Proceso;

int cargarProcesos(const char *filename, Proceso procesos[], int maxProcesos);
void imprimirProcesos(Proceso procesos[], int cantidad);

#endif
