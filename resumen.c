#include <stdio.h>
#include "resumen.h"

void resumen(Proceso* procesos, int cantidad) {
    int tiempoTotal = 0;
    int sumaEspera = 0;
    int sumaRetorno = 0;

    printf("\n--- Resumen de ejecucion ---\n");

    for (int i = 0; i < cantidad; i++) {
        if (procesos[i].fin > tiempoTotal) {
            tiempoTotal = procesos[i].fin;
        }
        sumaEspera += procesos[i].espera;
        sumaRetorno += procesos[i].retorno;
    }

    printf("Tiempo total de ejecucion: %d ciclos\n", tiempoTotal);
    printf("Tiempo promedio de espera: %.2f ciclos\n", (float)sumaEspera / cantidad);
    printf("Tiempo promedio de retorno: %.2f ciclos\n", (float)sumaRetorno / cantidad);

    printf("Procesos y tiempos:\n");
    for (int i = 0; i < cantidad; i++) {
        printf("  %s -> Inicio: %d, Fin: %d, Duracion: %d, Espera: %d, Retorno: %d\n",
            procesos[i].pid,
            procesos[i].inicio,
            procesos[i].fin,
            procesos[i].duracion,
            procesos[i].espera,
            procesos[i].retorno);
    }

    printf("---------------------------\n\n");
}
