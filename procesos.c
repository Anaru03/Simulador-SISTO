#include <stdio.h>
#include "procesos.h"

int leerProcesos(const char* filename, Proceso* procesos, int max) {
    // Abre el archivo en modo lectura
    FILE* archivo = fopen(filename, "r");
    // Verifica si el archivo se abrió correctamente
    if (!archivo) {
        // Si hubo un error al abrir, muestra un mensaje de error
        perror("Error al abrir procesos.txt");
        // Retorna 0 para indicar que no se pudo leer ningún proceso
        return 0;
    }

    int count = 0;  // Contador de procesos leídos
    // Mientras no se supere el máximo y se puedan leer datos en el formato esperado
    while (count < max &&
           fscanf(archivo, " %[^,], %d, %d, %d",  // Lee una línea con el formato: cadena, int, int, int
                  procesos[count].pid,             // Lee el identificador del proceso (cadena hasta la coma)
                  &procesos[count].duracion,      // Lee la duración del proceso (entero)
                  &procesos[count].llegada,       // Lee el tiempo de llegada del proceso (entero)
                  &procesos[count].prioridad)     // Lee la prioridad del proceso (entero)
           == 4) {                            // fscanf devuelve la cantidad de elementos leídos con éxito
        count++;  // Incrementa el contador de procesos leídos
    }

    // Cierra el archivo después de terminar la lectura
    fclose(archivo);
    // Retorna la cantidad total de procesos que se leyeron correctamente
    return count;
}
