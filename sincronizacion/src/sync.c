#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sync.h"

static SyncType sync_type = SYNC_MUTEX;

static int *mutex_locked = NULL;      // Array de mutex para recursos
static int *semaphore_count = NULL;   // Array para conteo de semáforos

static Resource *resources_global = NULL;
static int resource_count_global = 0;

void init_sync(SyncType type, Resource *resources, int resource_count) {
    sync_type = type;

    if (mutex_locked) {
        free(mutex_locked);
        mutex_locked = NULL;
    }
    if (semaphore_count) {
        free(semaphore_count);
        semaphore_count = NULL;
    }

    resource_count_global = resource_count;
    resources_global = resources;

    if (sync_type == SYNC_MUTEX) {
        mutex_locked = calloc(resource_count, sizeof(int));
        if (!mutex_locked) {
            fprintf(stderr, "Error allocating mutex array\n");
            exit(EXIT_FAILURE);
        }
    } else if (sync_type == SYNC_SEMAPHORE) {
        semaphore_count = malloc(resource_count * sizeof(int));
        if (!semaphore_count) {
            fprintf(stderr, "Error allocating semaphore array\n");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < resource_count; i++) {
            semaphore_count[i] = resources[i].counter;
        }
    }
}

void reset_sync(void) {
    if (mutex_locked) {
        free(mutex_locked);
        mutex_locked = NULL;
    }
    if (semaphore_count) {
        free(semaphore_count);
        semaphore_count = NULL;
    }
    resources_global = NULL;
    resource_count_global = 0;
}

int find_resource_index(const char *name) {
    for (int i = 0; i < resource_count_global; i++) {
        if (strcmp(resources_global[i].name, name) == 0) {
            return i;
        }
    }
    return -1; // No encontrado
}

int request_resource(const char *resource_name) {
    int idx = find_resource_index(resource_name);
    if (idx < 0) {
        fprintf(stderr, "Resource %s not found\n", resource_name);
        return 0; // No acceso
    }

    if (sync_type == SYNC_MUTEX) {
        if (mutex_locked[idx] == 0) {
            mutex_locked[idx] = 1; // Lock
            return 1; // Acceso permitido
        }
        return 0; // Mutex ocupado
    } else if (sync_type == SYNC_SEMAPHORE) {
        if (semaphore_count[idx] > 0) {
            semaphore_count[idx]--;
            return 1;
        }
        return 0; // No hay semáforos disponibles
    }
    return 0;
}

void release_resource(const char *resource_name) {
    int idx = find_resource_index(resource_name);
    if (idx < 0) return;

    if (sync_type == SYNC_MUTEX) {
        mutex_locked[idx] = 0; // Unlock
    } else if (sync_type == SYNC_SEMAPHORE) {
        if (semaphore_count[idx] < resources_global[idx].counter) {
            semaphore_count[idx]++;
        }
    }
}

void simulate_cycle(Action *actions, int action_count, int current_cycle) {
    // Para cada acción en el ciclo actual, intentar solicitar recurso
    for (int i = 0; i < action_count; i++) {
        if (actions[i].cycle == current_cycle) {
            int acc = request_resource(actions[i].resource);
            actions[i].accessed = acc;
            if (acc == 1) {
                // Aquí podría simular duración o liberar inmediatamente
                // Por simplicidad, liberamos inmediatamente
                release_resource(actions[i].resource);
            }
        }
    }
}
