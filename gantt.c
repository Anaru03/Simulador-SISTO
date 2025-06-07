#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gantt.h"

#ifdef _WIN32
    #include <windows.h>
    #define LIMPIAR "cls"             // Comando para limpiar pantalla en Windows
    #define PAUSA(ms) Sleep(ms)      // Pausa en milisegundos en Windows
#else
    #include <unistd.h>
    #define LIMPIAR "clear"           // Comando para limpiar pantalla en Linux/Mac
    #define PAUSA(ms) usleep((ms) * 1000)  // Pausa en microsegundos en Unix
#endif

// Estado del proceso para la visualización
typedef enum {
    NUEVO,      // Proceso aún no llegó
    LISTO,      // Proceso listo para ejecutar
    EJECUTANDO, // Proceso ejecutándose actualmente
    TERMINADO   // Proceso finalizado
} EstadoProceso;

// Colores para cada estado (ANSI escape codes)
const char* colores_estado[] = {
    "\033[100m", // NUEVO - fondo gris
    "\033[44m",  // LISTO - fondo azul
    "\033[42m",  // EJECUTANDO - fondo verde
    "\033[47m"   // TERMINADO - fondo blanco
};

// Función que determina el estado de un proceso en un ciclo dado
EstadoProceso obtenerEstado(Proceso p, int ciclo) {
    if (ciclo < p.llegada)
        return NUEVO;           // El proceso aún no llegó
    if (ciclo >= p.inicio && ciclo < p.fin)
        return EJECUTANDO;      // Está en ejecución
    if (ciclo >= p.fin)
        return TERMINADO;       // Ya terminó
    return LISTO;               // Llegó pero aún no se ejecuta
}

// Función para mostrar el diagrama de Gantt en consola
void mostrarGantt(Proceso* procesos, int cantidad) {
    // Encontrar el tiempo total máximo para dimensionar el diagrama
    int tiempoTotal = procesos[0].fin;
    for (int i = 1; i < cantidad; i++) {
        if (procesos[i].fin > tiempoTotal)
            tiempoTotal = procesos[i].fin;
    }

    // Crear matriz dinámica para controlar qué celdas mostrar
    int **mostrar = malloc(cantidad * sizeof(int*));
    for (int i = 0; i < cantidad; i++) {
        mostrar[i] = calloc(tiempoTotal + 1, sizeof(int)); // inicializa en 0
    }

    // Recorremos cada ciclo y proceso para imprimir paso a paso
    for (int t = 0; t <= tiempoTotal; t++) {
        for (int i = 0; i < cantidad; i++) {
            mostrar[i][t] = 1;   // Marca la celda actual para mostrar

            system(LIMPIAR);    // Limpia pantalla en cada paso para animación
            printf("Ciclo: %d - Proceso: %s\n\n", t, procesos[i].pid);

            // Encabezado horizontal con los tiempos (0..tiempoTotal)
            printf("       ");
            for (int h = 0; h <= tiempoTotal; h++) {
                printf("%4d", h);
            }
            printf("\n       ");
            for (int h = 0; h <= tiempoTotal; h++) {
                printf("----");
            }
            printf("\n");

            // Mostrar cada fila (proceso) con el estado coloreado
            for (int p = 0; p < cantidad; p++) {
                printf("%-6s|", procesos[p].pid);
                for (int c = 0; c <= tiempoTotal; c++) {
                    if (mostrar[p][c]) {
                        EstadoProceso est = obtenerEstado(procesos[p], c);
                        if (est == NUEVO) {
                            // Nuevo: celda vacía con fondo gris
                            printf("%s    \033[0m", colores_estado[NUEVO]);
                        } else {
                            // Otros estados: mostrar el ID del proceso con fondo color
                            printf("%s%-4s\033[0m", colores_estado[est], procesos[p].pid);
                        }
                    } else {
                        printf("    "); // Celda no mostrada, espacio vacío
                    }
                }
                printf("\n");
            }
            PAUSA(300); // Pausa para ver la animación (300 ms)
        }
    }

    // Mostrar leyenda de colores al final
    printf("\nLeyenda de estados:\n");
    printf("\033[100m NUEVO \033[0m     ");
    printf("\033[44m LISTO \033[0m     ");
    printf("\033[42m EJECUTANDO \033[0m     ");
    printf("\033[47m TERMINADO \033[0m\n");

    // Liberar memoria reservada para la matriz 'mostrar'
    for (int i = 0; i < cantidad; i++) {
        free(mostrar[i]);
    }
    free(mostrar);
}
