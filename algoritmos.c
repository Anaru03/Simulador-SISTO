#include <stdio.h>
#include "algoritmos.h"

void fifo(Proceso* procesos, int cantidad) {
    int tiempo = 0;

    for (int i = 0; i < cantidad; i++) {
        if (tiempo < procesos[i].llegada)
            tiempo = procesos[i].llegada;

        procesos[i].inicio = tiempo;
        procesos[i].fin = tiempo + procesos[i].duracion;
        procesos[i].espera = procesos[i].inicio - procesos[i].llegada;
        procesos[i].retorno = procesos[i].fin - procesos[i].llegada;

        tiempo = procesos[i].fin;
    }

    printf("PID | Llegada | Duracion | Inicio | Fin | Espera | Retorno\n");
    for (int i = 0; i < cantidad; i++) {
        printf("%3s | %7d | %8d | %6d | %3d | %6d | %7d\n",
            procesos[i].pid,
            procesos[i].llegada,
            procesos[i].duracion,
            procesos[i].inicio,
            procesos[i].fin,
            procesos[i].espera,
            procesos[i].retorno);
    }
}