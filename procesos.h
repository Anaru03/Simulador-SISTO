#ifndef PROCESOS_H
#define PROCESOS_H

#define MAX_PID_LEN 10

typedef struct {
    char pid[MAX_PID_LEN]; 
    int duracion;          
    int llegada;           
    int prioridad;         
    int inicio;
    int fin;
    int espera;
    int retorno;
} Proceso;

int leerProcesos(const char* filename, Proceso* procesos, int max);

#endif
