#ifndef SIMULATION_H
#define SIMULATION_H

#include "process.h"
#include "resource.h"
#include "action.h"
#include "sync.h"

// Funciones para cargar archivos
int load_processes(const char *filename, Process **processes, int *count);
int load_resources(const char *filename, Resource **resources, int *count);
int load_actions(const char *filename, Action **actions, int *count);

// Simulación para GUI
void simulate_with_gui(Process *processes, int pcount,
                       Resource *resources, int rcount,
                       Action *actions, int acount,
                       SyncType mode);

// Simulación para modo consola
void simulate_console(const char *procfile, const char *resfile, const char *actfile, SyncType mode);

#endif // SIMULATION_H