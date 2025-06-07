#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "procesos.h"

void fifo(Proceso* procesos, int cantidad);
void sjf(Proceso* procesos, int cantidad);
void srt(Proceso* procesos, int cantidad);
void round_robin(Proceso* procesos, int cantidad, int quantum);
void priority(Proceso* procesos, int cantidad);

#endif
