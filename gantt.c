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

    // Matriz para saber qué celdas ya mostrar
    int **mostrar = malloc(cantidad * sizeof(int*));
    for (int i = 0; i < cantidad; i++) {
        mostrar[i] = calloc(tiempoTotal + 1, sizeof(int));
    }

    // Recorremos cada ciclo y proceso (celda por celda verticalmente)
    for (int t = 0; t <= tiempoTotal; t++) {
        for (int i = 0; i < cantidad; i++) {
            mostrar[i][t] = 1;

            system(LIMPIAR);
            printf("Ciclo: %d - Proceso: %s\n\n", t, procesos[i].pid);

            // Encabezado horizontal (tiempos)
            printf("       ");
            for (int h = 0; h <= tiempoTotal; h++) {
                printf("%4d", h);
            }
            printf("\n       ");
            for (int h = 0; h <= tiempoTotal; h++) {
                printf("----");
            }
            printf("\n");

            // Mostrar tabla de estados hasta la celda actual
            for (int p = 0; p < cantidad; p++) {
                printf("%-6s|", procesos[p].pid);
                for (int c = 0; c <= tiempoTotal; c++) {
                    if (mostrar[p][c]) {
                        EstadoProceso est = obtenerEstado(procesos[p], c);
                        if (est == NUEVO) {
                            printf("%s    \033[0m", colores_estado[NUEVO]);
                        } else {
                            printf("%s%-4s\033[0m", colores_estado[est], procesos[p].pid);
                        }
                    } else {
                        printf("    ");
                    }
                }
                printf("\n");
            }
            PAUSA(300);
        }
    }

    // Mostrar leyenda (sin limpiar ni pedir Enter)
    printf("\nLeyenda de estados:\n");
    printf("\033[100m NUEVO \033[0m     ");
    printf("\033[44m LISTO \033[0m     ");
    printf("\033[42m EJECUTANDO \033[0m     ");
    printf("\033[47m TERMINADO \033[0m\n");

    // Liberar memoria
    for (int i = 0; i < cantidad; i++) {
        free(mostrar[i]);
    }
    free(mostrar);
}
