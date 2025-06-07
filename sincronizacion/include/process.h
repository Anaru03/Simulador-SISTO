#ifndef PROCESS_H
#define PROCESS_H

#define MAX_PROCESSES 100

typedef struct {
    char pid[10];
    int bt;
    int at;
    int priority;
} Process;

int load_processes(const char *filename);
Process* get_processes(int *count);

#endif
