# Simulador de Planificación de Procesos

Este proyecto es un simulador de planificación de procesos en sistemas operativos, implementado en C. Permite probar varios algoritmos clásicos de planificación y visualizar el diagrama de Gantt junto con un resumen de resultados.

---

## Estructura del proyecto

- `algoritmos.c` / `algoritmos.h`: Implementación de los algoritmos de planificación (FIFO, SJF, SRT, Round Robin, Priority con envejecimiento).
- `gantt.c` / `gantt.h`: Funciones para mostrar el diagrama de Gantt en consola con colores para cada estado de proceso.
- `main.c`: Programa principal con el menú para seleccionar el algoritmo y ejecutar la simulación.
- `procesos.c` / `procesos.h`: Definición de la estructura `Proceso` y funciones para leer procesos desde archivo.
- `resumen.c` / `resumen.h`: Funciones para mostrar estadísticas de la planificación (tiempos de espera, retorno, etc.).
- `procesos.txt`: Archivo de entrada con la definición de procesos a simular.

---

## Requisitos

- Compilador GCC (compatible con Windows y Linux)
- Terminal que soporte códigos de color ANSI (en Windows usar `cmd.exe` o PowerShell compatibles)

---

## Compilación

En el directorio raíz del proyecto, ejecutar:

```bash
gcc algoritmos.c gantt.c main.c procesos.c resumen.c -o simulador.exe

```


## Ejecución

En el directorio raíz del proyecto, ejecutar:

```bash
./simulador.exe