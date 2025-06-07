#include <stdio.h>
#include <string.h>
#include "algoritmos.h"

// FIFO - First In, First Out
// Ejecuta los procesos en orden de llegada sin interrupciones
void fifo(Proceso* procesos, int cantidad) {
    int tiempo = 0;

    for (int i = 0; i < cantidad; i++) {
        // Si el tiempo actual es menor que la llegada del proceso, espera hasta que llegue
        if (tiempo < procesos[i].llegada)
            tiempo = procesos[i].llegada;

        // Establece tiempo de inicio, fin, espera y retorno
        procesos[i].inicio = tiempo;
        procesos[i].fin = tiempo + procesos[i].duracion;
        procesos[i].espera = procesos[i].inicio - procesos[i].llegada;
        procesos[i].retorno = procesos[i].fin - procesos[i].llegada;

        // Avanza el tiempo al fin del proceso actual
        tiempo = procesos[i].fin;
    }
}

// Helper para ordenar por duración ascendente (para SJF)
int compararDuracion(const void* a, const void* b) {
    Proceso* p1 = (Proceso*)a;
    Proceso* p2 = (Proceso*)b;
    if (p1->duracion == p2->duracion)
        return p1->llegada - p2->llegada; // desempate por llegada
    return p1->duracion - p2->duracion;
}

// SJF - Shortest Job First (no preemptivo)
// Selecciona el proceso listo con menor duración para ejecutar
void sjf(Proceso* procesos, int cantidad) {
    int tiempo = 0;
    int procesosRestantes = cantidad;
    int usados[cantidad];
    for (int i = 0; i < cantidad; i++) usados[i] = 0; // marcar procesos no usados

    while (procesosRestantes > 0) {
        int idxMejor = -1;
        int duracionMin = 1000000; // valor alto inicial
        // Buscar proceso con menor duración que ya llegó y no usado
        for (int i = 0; i < cantidad; i++) {
            if (!usados[i] && procesos[i].llegada <= tiempo && procesos[i].duracion < duracionMin) {
                duracionMin = procesos[i].duracion;
                idxMejor = i;
            }
        }

        if (idxMejor == -1) {
            tiempo++; // avanza tiempo si no hay proceso listo
            continue;
        }

        // Calcula tiempos para el proceso seleccionado
        procesos[idxMejor].inicio = tiempo;
        procesos[idxMejor].fin = tiempo + procesos[idxMejor].duracion;
        procesos[idxMejor].espera = procesos[idxMejor].inicio - procesos[idxMejor].llegada;
        procesos[idxMejor].retorno = procesos[idxMejor].fin - procesos[idxMejor].llegada;

        tiempo += procesos[idxMejor].duracion;
        usados[idxMejor] = 1; // marca proceso como usado
        procesosRestantes--;
    }
}

// SRT - Shortest Remaining Time (preemptivo)
// Ejecuta siempre el proceso con menor tiempo restante, puede interrumpir
void srt(Proceso* procesos, int cantidad) {
    int tiempo = 0;
    int completados = 0;
    int remaining[cantidad];       // tiempo restante para cada proceso
    int inicioAsignado[cantidad];  // marca si se asignó inicio
    int finalizados[cantidad];     // marca procesos finalizados

    for (int i = 0; i < cantidad; i++) {
        remaining[i] = procesos[i].duracion;
        inicioAsignado[i] = -1; // -1 indica que no ha iniciado
        finalizados[i] = 0;
    }

    while (completados < cantidad) {
        int idxMejor = -1;
        int minRem = 1000000;
        // Buscar proceso listo con menor tiempo restante
        for (int i = 0; i < cantidad; i++) {
            if (!finalizados[i] && procesos[i].llegada <= tiempo && remaining[i] < minRem && remaining[i] > 0) {
                minRem = remaining[i];
                idxMejor = i;
            }
        }

        if (idxMejor == -1) {
            tiempo++; // si no hay procesos listos, avanzar tiempo
            continue;
        }

        // Asignar tiempo de inicio sólo la primera vez que corre el proceso
        if (inicioAsignado[idxMejor] == -1)
            procesos[idxMejor].inicio = tiempo;

        remaining[idxMejor]--;  // ejecuta 1 unidad de tiempo
        tiempo++;

        if (remaining[idxMejor] == 0) {
            // Proceso finaliza, calcular tiempos
            procesos[idxMejor].fin = tiempo;
            procesos[idxMejor].espera = procesos[idxMejor].fin - procesos[idxMejor].llegada - procesos[idxMejor].duracion;
            procesos[idxMejor].retorno = procesos[idxMejor].fin - procesos[idxMejor].llegada;
            finalizados[idxMejor] = 1;
            completados++;
        }
    }
}

// Round Robin (quantum configurable)
// Asigna CPU en turnos de duración 'quantum' a cada proceso en orden circular
void round_robin(Proceso* procesos, int cantidad, int quantum) {
    int tiempo = 0;
    int remaining[cantidad];
    int finalizados = 0;
    int inicioAsignado[cantidad];

    for (int i = 0; i < cantidad; i++) {
        remaining[i] = procesos[i].duracion;
        inicioAsignado[i] = -1;
        procesos[i].fin = 0;
        procesos[i].inicio = -1;
    }

    while (finalizados < cantidad) {
        int avance = 0; // indica si se ejecutó algún proceso en esta ronda

        for (int i = 0; i < cantidad; i++) {
            if (procesos[i].llegada <= tiempo && remaining[i] > 0) {
                if (inicioAsignado[i] == -1)
                    procesos[i].inicio = tiempo;

                // Ejecutar el menor entre el quantum o el tiempo restante
                int ejecutar = (remaining[i] < quantum) ? remaining[i] : quantum;
                remaining[i] -= ejecutar;
                tiempo += ejecutar;
                avance = 1;

                if (remaining[i] == 0) {
                    // Proceso terminado, calcula tiempos
                    procesos[i].fin = tiempo;
                    procesos[i].espera = procesos[i].fin - procesos[i].llegada - procesos[i].duracion;
                    procesos[i].retorno = procesos[i].fin - procesos[i].llegada;
                    finalizados++;
                }
            } else if (procesos[i].llegada > tiempo) {
                // Si proceso aún no ha llegado, adelanta tiempo para esperar
                tiempo = procesos[i].llegada;
                i--; // repetir con mismo índice después de avanzar tiempo
            }
        }

        // Si no se avanzó, incrementar tiempo para evitar bloqueo
        if (!avance) tiempo++;
    }
}

// Priority con envejecimiento (aging)
// Selecciona proceso con mayor prioridad (menor número) y mejora prioridad mientras espera
void priority(Proceso* procesos, int cantidad) {
    int tiempo = 0;
    int procesosRestantes = cantidad;
    int usados[cantidad];
    int prioridades[cantidad];

    for (int i = 0; i < cantidad; i++) {
        usados[i] = 0;               // proceso no usado aún
        prioridades[i] = procesos[i].prioridad; // prioridad inicial
    }

    while (procesosRestantes > 0) {
        // Envejecimiento: mejorar prioridad de procesos esperando
        for (int i = 0; i < cantidad; i++) {
            if (!usados[i] && procesos[i].llegada <= tiempo) {
                prioridades[i]--;
            }
        }

        // Seleccionar proceso con mayor prioridad listo
        int idxMejor = -1;
        int prioridadMin = 1000000;
        for (int i = 0; i < cantidad; i++) {
            if (!usados[i] && procesos[i].llegada <= tiempo && prioridades[i] < prioridadMin) {
                prioridadMin = prioridades[i];
                idxMejor = i;
            }
        }

        if (idxMejor == -1) {
            tiempo++; // avanza tiempo si no hay proceso listo
            continue;
        }

        // Calcula tiempos para el proceso seleccionado
        procesos[idxMejor].inicio = tiempo;
        procesos[idxMejor].fin = tiempo + procesos[idxMejor].duracion;
        procesos[idxMejor].espera = procesos[idxMejor].inicio - procesos[idxMejor].llegada;
        procesos[idxMejor].retorno = procesos[idxMejor].fin - procesos[idxMejor].llegada;

        tiempo += procesos[idxMejor].duracion;
        usados[idxMejor] = 1; // marca proceso como usado
        procesosRestantes--;
    }
}