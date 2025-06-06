#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gantt.h"

#ifdef _WIN32
    #include <windows.h>
    #define LIMPIAR "cls"
    #define PAUSA(ms) Sleep(ms)
#else
    #include <unistd.h>
    #define LIMPIAR "clear"
    #define PAUSA(ms) usleep((ms) * 1000)
#endif

typedef enum {
    NUEVO, LISTO, EJECUTANDO, TERMINADO
} EstadoProceso;

const char* colores_estado[] = {
    "\033[100m", // NUEVO - gris
    "\033[44m",  // LISTO - azul
    "\033[42m",  // EJECUTANDO - verde
    "\033[47m"   // TERMINADO - blanco
};

EstadoProceso obtenerEstado(Proceso p, int ciclo) {
    if (ciclo < p.llegada) return NUEVO;
    if (ciclo >= p.inicio && ciclo < p.fin) return EJECUTANDO;
    if (ciclo >= p.fin) return TERMINADO;
    return LISTO;
}

void mostrarGantt(Proceso* procesos, int cantidad) {
    int tiempoTotal = procesos[0].fin;
    for (int i = 1; i < cantidad; i++) {
        if (procesos[i].fin > tiempoTotal)
            tiempoTotal = procesos[i].fin;
    }

    for (int ciclo = 0; ciclo <= tiempoTotal; ciclo++) {
        system(LIMPIAR);
        printf("Ciclo: %d\n", ciclo);
        printf("       ");

        // Encabezado de ciclos
        for (int t = 0; t <= tiempoTotal; t++) {
            printf("%4d", t);
        }
        printf("\n       ");
        for (int t = 0; t <= tiempoTotal; t++) {
            printf("----");
        }
        printf("\n");

        // Filas por proceso
        for (int i = 0; i < cantidad; i++) {
            printf("%-6s|", procesos[i].pid);
            for (int t = 0; t <= tiempoTotal; t++) {
                if (t > ciclo) {
                    printf("    ");
                } else {
                    EstadoProceso estado = obtenerEstado(procesos[i], t);
                    if (estado == EJECUTANDO) {
                        printf("%s%-4s\033[0m", colores_estado[estado], procesos[i].pid);
                    } else {
                        printf("%s    \033[0m", colores_estado[estado]);
                    }
                }
            }
            printf("\n");
        }

        // Pausa entre ciclos
        PAUSA(500); // 500 ms
    }

    // Leyenda final
    printf("\nLeyenda de estados:\n");
    printf("\033[100m NUEVO \033[0m     ");
    printf("\033[44m LISTO \033[0m     ");
    printf("\033[42m EJECUTANDO (con nombre) \033[0m     ");
    printf("\033[47m TERMINADO \033[0m\n");
}
