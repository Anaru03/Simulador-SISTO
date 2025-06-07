#include <stdio.h>
#include <string.h>
#include "process.h"

static Process processes[MAX_PROCESSES];
static int process_count = 0;

int load_processes(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("Error abriendo procesos.txt");
        return -1;
    }
    process_count = 0;
    char line[100];
    while (fgets(line, sizeof(line), f)) {
        if (process_count >= MAX_PROCESSES) break;
        // Formato: P1, 8, 0, 1
        char pid[10];
        int bt, at, priority;
        if (sscanf(line, "%[^,], %d, %d, %d", pid, &bt, &at, &priority) == 4) {
            strcpy(processes[process_count].pid, pid);
            processes[process_count].bt = bt;
            processes[process_count].at = at;
            processes[process_count].priority = priority;
            process_count++;
        }
    }
    fclose(f);
    return 0;
}

Process* get_processes(int *count) {
    *count = process_count;
    return processes;
}
