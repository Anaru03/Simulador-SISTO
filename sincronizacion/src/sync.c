#include "sync.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eventlog.h"  // Usamos funciones de eventlog.c

// Encuentra índice recurso por nombre
static int find_resource_index(Resource* resources, int rcount, const char* name) {
    for (int i = 0; i < rcount; i++) {
        if (strcmp(resources[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Encuentra índice proceso por pid
static int find_process_index(Process* processes, int pcount, const char* pid) {
    for (int i = 0; i < pcount; i++) {
        if (strcmp(processes[i].pid, pid) == 0) {
            return i;
        }
    }
    return -1;
}

void simulate(Process* processes, int pcount,
              Resource* resources, int rcount,
              Action* actions, int acount,
              SyncType sync_type,
              EventLog *log) {

    int *resource_available = malloc(sizeof(int) * rcount);
    for (int i = 0; i < rcount; i++) {
        resource_available[i] = resources[i].count;
    }

    int *resource_owner = malloc(sizeof(int) * rcount);
    for (int i = 0; i < rcount; i++) {
        resource_owner[i] = -1;
    }

    int max_cycle = 0;
    for (int i = 0; i < acount; i++) {
        if (actions[i].cycle > max_cycle)
            max_cycle = actions[i].cycle;
    }

    for (int cycle = 0; cycle <= max_cycle; cycle++) {
        char event_desc[50];  // ← declaración corregida

        for (int i = 0; i < acount; i++) {
            Action *act = &actions[i];
            if (act->cycle == cycle) {
                int r_idx = find_resource_index(resources, rcount, act->resource);
                if (r_idx == -1) {
                    printf("Recurso no encontrado: %s\n", act->resource);
                    continue;
                }

                int p_idx = find_process_index(processes, pcount, act->pid);
                if (p_idx == -1) {
                    printf("Proceso no encontrado: %s\n", act->pid);
                    continue;
                }

                if (sync_type == MUTEX) {
                    if (resource_owner[r_idx] == -1) {
                        resource_owner[r_idx] = p_idx;
                        snprintf(event_desc, sizeof(event_desc), "ACQUIRE %s", act->resource);
                        add_event(log, cycle, p_idx, event_desc);
                    } else {
                        printf("Recurso %s ocupado en ciclo %d por proceso %s\n", act->resource, cycle, act->pid);
                    }
                } else if (sync_type == SEMAPHORE) {
                    if (resource_available[r_idx] > 0) {
                        resource_available[r_idx]--;
                        snprintf(event_desc, sizeof(event_desc), "ACQUIRE %s", act->resource);
                        add_event(log, cycle, p_idx, event_desc);
                    } else {
                        printf("Semáforo recurso %s bloqueado en ciclo %d por proceso %s\n", act->resource, cycle, act->pid);
                    }
                }
            }
        }

        if (sync_type == MUTEX) {
            for (int r = 0; r < rcount; r++) {
                if (resource_owner[r] != -1) {
                    snprintf(event_desc, sizeof(event_desc), "RELEASE %s", resources[r].name);
                    add_event(log, cycle, resource_owner[r], event_desc);
                    resource_owner[r] = -1;
                }
            }
        } else if (sync_type == SEMAPHORE) {
            for (int r = 0; r < rcount; r++) {
                if (resource_available[r] < resources[r].count) {
                    resource_available[r]++;
                    add_event(log, cycle, -1, "RELEASE");
                }
            }
        }
    }

    free(resource_owner);
    free(resource_available);
}