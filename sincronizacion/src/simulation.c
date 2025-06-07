#include "simulation.h"
#include "timeline.h"
#include "eventlog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int load_processes(const char *filename, Process **processes, int *count) {
    FILE *file = fopen(filename, "r");
    if (!file) return 0;

    fscanf(file, "%d", count);
    *processes = malloc(sizeof(Process) * (*count));
    for (int i = 0; i < *count; ++i) {
        fscanf(file, "%s", (*processes)[i].name);
    }

    fclose(file);
    return 1;
}

int load_resources(const char *filename, Resource **resources, int *count) {
    FILE *file = fopen(filename, "r");
    if (!file) return 0;

    fscanf(file, "%d", count);
    *resources = malloc(sizeof(Resource) * (*count));
    for (int i = 0; i < *count; ++i) {
        fscanf(file, "%s", (*resources)[i].name);
    }

    fclose(file);
    return 1;
}

int load_actions(const char *filename, Action **actions, int *count) {
    FILE *file = fopen(filename, "r");
    if (!file) return 0;

    fscanf(file, "%d", count);
    *actions = malloc(sizeof(Action) * (*count));
    for (int i = 0; i < *count; ++i) {
        fscanf(file, "%s %s %s %d", 
               (*actions)[i].process_name, 
               (*actions)[i].resource_name, 
               (*actions)[i].type,
               &(*actions)[i].time);
    }

    fclose(file);
    return 1;
}

void simulate_with_gui(Process *processes, int pcount,
                       Resource *resources, int rcount,
                       Action *actions, int acount,
                       SyncType mode) {
    EventLog log;
    init_event_log(&log);

    simulate(processes, pcount, resources, rcount, actions, acount, mode, &log);

    set_global_event_log(&log);
    draw_timeline();

    free_event_log(&log);
}

void simulate_console(const char *procfile, const char *resfile, const char *actfile, SyncType mode) {
    Process *processes = NULL;
    Resource *resources = NULL;
    Action *actions = NULL;
    int pcount = 0, rcount = 0, acount = 0;

    if (!load_processes(procfile, &processes, &pcount)) {
        fprintf(stderr, "Error loading processes\n");
        return;
    }

    if (!load_resources(resfile, &resources, &rcount)) {
        fprintf(stderr, "Error loading resources\n");
        free(processes);
        return;
    }

    if (!load_actions(actfile, &actions, &acount)) {
        fprintf(stderr, "Error loading actions\n");
        free(processes);
        free(resources);
        return;
    }

    EventLog log;
    init_event_log(&log);

    simulate(processes, pcount, resources, rcount, actions, acount, mode, &log);

    for (int i = 0; i < log.count; ++i) {
        printf("[%d] %s %s %s\n", log.entries[i].time, log.entries[i].process_name, log.entries[i].action_type, log.entries[i].resource_name);
    }

    free_event_log(&log);
    free(processes);
    free(resources);
    free(actions);
}