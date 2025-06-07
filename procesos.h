#ifndef PROCESOS_H
#define PROCESOS_H

#define MAX_PID_LEN 10  // Longitud máxima del identificador del proceso (PID)

// Estructura que representa un proceso
typedef struct {
    char pid[MAX_PID_LEN]; // Identificador del proceso (cadena de caracteres)
    int duracion;          // Tiempo total que necesita el proceso para ejecutarse
    int llegada;           // Tiempo en el que el proceso llega al sistema (tiempo de llegada)
    int prioridad;         // Prioridad asignada al proceso (mayor prioridad, menor número)
    int inicio;            // Tiempo en que el proceso comienza a ejecutarse
    int fin;               // Tiempo en que el proceso termina su ejecución
    int espera;            // Tiempo total que el proceso estuvo esperando para ejecutarse
    int retorno;           // Tiempo total desde la llegada hasta la finalización (retorno o turnaround)
} Proceso;

// Función para leer los procesos desde un archivo de texto
// Parámetros:
// - filename: nombre del archivo que contiene la lista de procesos
// - procesos: arreglo donde se almacenarán los procesos leídos
// - max: cantidad máxima de procesos que se pueden leer
// Retorna: número de procesos leídos correctamente
int leerProcesos(const char* filename, Proceso* procesos, int max);

#endif