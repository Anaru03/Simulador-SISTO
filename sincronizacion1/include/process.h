#ifndef PROCESS_H
#define PROCESS_H

#include "action.h"  // se queda aquí, correcto

typedef struct {
    char pid[20];
    int burst_time;
    int arrival_time;      // <--- agregado
    int remaining_time;
    int priority;
    int current_state;
} Process;

typedef struct ProcessNode {
    Process process;
    Action* actions;
    struct ProcessNode* next;
} ProcessNode;

#endif
