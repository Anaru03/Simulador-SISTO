#include <stdio.h>
#include <stdlib.h>
#include "procesos.h"
#include "algoritmos.h"
#include "gantt.h"
#include "resumen.h"

#define MAX_PROCESOS 100

// Función para mostrar el menú principal con las opciones de algoritmos
void mostrarMenu() {
    printf("\n--- Simulador de Planificacion de Procesos ---\n");
    printf("Seleccione un algoritmo:\n");
    printf("1. FIFO\n");
    printf("2. SJF (Shortest Job First)\n");
    printf("3. SRT (Shortest Remaining Time)\n");
    printf("4. Round Robin\n");
    printf("5. Priority con Envejecimiento\n");
    printf("0. Salir\n");
    printf("Opcion: ");
}

int main() {
    // Array para almacenar los procesos leídos
    Proceso procesos[MAX_PROCESOS];
    // Leer procesos desde el archivo "procesos.txt"
    int cantidad = leerProcesos("procesos.txt", procesos, MAX_PROCESOS);

    // Si no se leen procesos, terminar el programa
    if (cantidad == 0) {
        printf("No se pudieron leer procesos.\n");
        return 1;
    }

    int opcion;    // Variable para almacenar la opción del usuario
    int quantum;   // Variable para almacenar el quantum (solo para Round Robin)

    do {
        mostrarMenu();     // Mostrar el menú con opciones
        scanf("%d", &opcion);  // Leer la opción ingresada

        // Crear una copia del arreglo original para no modificarlo directamente
        Proceso copia[MAX_PROCESOS];
        for (int i = 0; i < cantidad; i++) {
            copia[i] = procesos[i];
        }

        // Ejecutar el algoritmo seleccionado
        switch (opcion) {
            case 1:
                fifo(copia, cantidad);            // Ejecutar algoritmo FIFO
                mostrarGantt(copia, cantidad);   // Mostrar diagrama de Gantt
                resumen(copia, cantidad);         // Mostrar resumen de ejecución
                break;
            case 2:
                sjf(copia, cantidad);             // Ejecutar algoritmo SJF
                mostrarGantt(copia, cantidad);
                resumen(copia, cantidad);
                break;
            case 3:
                srt(copia, cantidad);             // Ejecutar algoritmo SRT
                mostrarGantt(copia, cantidad);
                resumen(copia, cantidad);
                break;
            case 4:
                printf("Ingrese el quantum: ");
                scanf("%d", &quantum);            // Leer el quantum para Round Robin
                round_robin(copia, cantidad, quantum);
                mostrarGantt(copia, cantidad);
                resumen(copia, cantidad);
                break;
            case 5:
                priority(copia, cantidad);        // Ejecutar algoritmo Priority con envejecimiento
                mostrarGantt(copia, cantidad);
                resumen(copia, cantidad);
                break;
            case 0:
                printf("Saliendo...\n");          
                break;
            default:
                printf("Opcion no válida. Intente nuevamente.\n");
        }

    } while (opcion != 0);  // Repetir mientras la opción no sea salir (0)

    return 0;
}