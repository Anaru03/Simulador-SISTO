#include "../include/process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Process* load_processes(const char* filename, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;

    Process* processes = malloc(sizeof(Process) * 100);
    if (!processes) {
        fclose(file);
        return NULL;
    }

    char line[100];
    int i = 0;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%19[^,], %d, %d, %d",
            processes[i].pid,
            &processes[i].burst_time,
            &processes[i].arrival_time,
            &processes[i].priority) == 4) {
            processes[i].remaining_time = processes[i].burst_time;
            processes[i].current_state = 0; // ready
            i++;
        }
    }

    fclose(file);
    *count = i;
    return processes;
}
