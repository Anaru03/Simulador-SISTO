#include <stdio.h>
#include <unistd.h>     // Para sleep()
#include <stdlib.h>     // Para system("cls") o system("clear")
#include <string.h>
#include "gantt.h"

// Colores ANSI
const char* colores[] = {
    "\033[41m", "\033[42m", "\033[43m", "\033[44m", "\033[45m",
    "\033[46m", "\033[47m", "\033[100m", "\033[101m", "\033[102m"
};

void mostrarGantt(Proceso* procesos, int cantidad) {
    int tiempoTotal = procesos[cantidad - 1].fin;

    printf("\nDiagrama de Gantt (Simulación dinámica):\n");

    for (int tiempo = 0; tiempo <= tiempoTotal; tiempo++) {
        system("cls"); // Usa "cls" en Windows

        printf("Ciclo: %d\n", tiempo);
        printf("      ");

        for (int t = 0; t <= tiempo; t++) {
            int encontrado = 0;
            for (int i = 0; i < cantidad; i++) {
                if (t >= procesos[i].inicio && t < procesos[i].fin) {
                    printf("%s %s \033[0m", colores[i % 10], procesos[i].pid);
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) printf("     ");
        }
        printf("\n");
        sleep(1); // 1 segundo por ciclo, puedes ajustar
    }

    // Línea final con leyenda
    printf("\nLeyenda:\n");
    for (int i = 0; i < cantidad; i++) {
        printf("%s %s \033[0m  ", colores[i % 10], procesos[i].pid);
    }
    printf("\n");
}
