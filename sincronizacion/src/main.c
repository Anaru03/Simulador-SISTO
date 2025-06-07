#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "resource.h"
#include "action.h"

Resource* recursos = NULL;
ProcessNode* procesos = NULL;

int cargar_recursos(const char* nombre_archivo) {
    FILE* archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        perror("No se pudo abrir el archivo de recursos");
        return 0;
    }

    char nombre[32];
    int cantidad;
    while (fscanf(archivo, "%31s %d", nombre, &cantidad) == 2) {
        Resource* nuevo = malloc(sizeof(Resource));
        if (!nuevo) {
            perror("Error de memoria");
            fclose(archivo);
            return 0;
        }
        strcpy(nuevo->name, nombre);
        nuevo->count = cantidad;
        nuevo->count_available = cantidad;
        nuevo->next = recursos;
        recursos = nuevo;
    }

    fclose(archivo);
    return 1;
}

int cargar_procesos(const char* nombre_archivo) {
    FILE* archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        perror("No se pudo abrir el archivo de procesos");
        return 0;
    }

    char pid[20];
    while (fscanf(archivo, "%19s", pid) == 1) {
        ProcessNode* nuevo = malloc(sizeof(ProcessNode));
        if (!nuevo) {
            perror("Error de memoria");
            fclose(archivo);
            return 0;
        }
        strcpy(nuevo->process.pid, pid);
        nuevo->process.burst_time = 10;      // default
        nuevo->process.remaining_time = 10;  // default
        nuevo->process.priority = 1;          // default
        nuevo->process.current_state = 0;     // ready
        nuevo->actions = NULL;
        nuevo->next = procesos;
        procesos = nuevo;
    }

    fclose(archivo);
    return 1;
}

ActionType parse_action_type(const char* str) {
    if (strcmp(str, "READ") == 0) return READ;
    if (strcmp(str, "WRITE") == 0) return WRITE;
    if (strcmp(str, "RELEASE") == 0) return RELEASE;
    return READ; // fallback
}

ProcessNode* buscar_proceso(const char* pid) {
    for (ProcessNode* p = procesos; p != NULL; p = p->next) {
        if (strcmp(p->process.pid, pid) == 0)
            return p;
    }
    return NULL;
}

int cargar_acciones(const char* nombre_archivo) {
    FILE* archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        perror("No se pudo abrir el archivo de acciones");
        return 0;
    }

    char linea[128];
    while (fgets(linea, sizeof(linea), archivo)) {
        char pid[20], tipo[20], recurso[32];
        int ciclo;

        if (sscanf(linea, "%19[^,], %19[^,], %31[^,], %d", pid, tipo, recurso, &ciclo) != 4) {
            fprintf(stderr, "Formato inválido: %s\n", linea);
            continue;
        }

        ProcessNode* proceso = buscar_proceso(pid);
        if (!proceso) {
            fprintf(stderr, "Proceso no encontrado: %s\n", pid);
            continue;
        }

        Action* nueva = malloc(sizeof(Action));
        if (!nueva) {
            perror("Error de memoria");
            fclose(archivo);
            return 0;
        }

        nueva->type = parse_action_type(tipo);
        strcpy(nueva->resource, recurso);
        nueva->cycle = ciclo;
        nueva->next = NULL;

        if (!proceso->actions) {
            proceso->actions = nueva;
        } else {
            Action* temp = proceso->actions;
            while (temp->next) temp = temp->next;
            temp->next = nueva;
        }
    }

    fclose(archivo);
    return 1;
}

Resource* buscar_recurso(const char* nombre) {
    for (Resource* r = recursos; r != NULL; r = r->next) {
        if (strcmp(r->name, nombre) == 0)
            return r;
    }
    return NULL;
}

void simular() {
    printf("Simulación:\n");

    for (ProcessNode* p = procesos; p != NULL; p = p->next) {
        printf("Proceso %s:\n", p->process.pid);

        for (Action* a = p->actions; a != NULL; a = a->next) {
            Resource* res = buscar_recurso(a->resource);
            const char* tipo_str = (a->type == READ) ? "READ" :
                                   (a->type == WRITE) ? "WRITE" : "RELEASE";

            printf("  Acción: %s recurso %s en ciclo %d\n", tipo_str, a->resource, a->cycle);

            if (a->type == READ || a->type == WRITE) {
                if (!res) {
                    printf("    Recurso %s no encontrado.\n", a->resource);
                } else if (res->count_available > 0) {
                    res->count_available--;
                    printf("    Recurso %s tomado. Disponibles ahora: %d\n", res->name, res->count_available);
                } else {
                    printf("    Recurso %s NO disponible. Esperando...\n", res->name);
                }
            } else if (a->type == RELEASE) {
                if (!res) {
                    printf("    Recurso %s no encontrado para liberar.\n", a->resource);
                } else {
                    res->count_available++;
                    if (res->count_available > res->count)
                        res->count_available = res->count; // No exceder máximo
                    printf("    Recurso %s liberado. Disponibles ahora: %d\n", res->name, res->count_available);
                }
            }
        }
        printf("\n");
    }
}

int main() {
    if (!cargar_recursos("data/recursos.txt")) return 1;
    if (!cargar_procesos("data/procesos.txt")) return 1;
    if (!cargar_acciones("data/acciones.txt")) return 1;

    printf("Recursos cargados:\n");
    for (Resource* r = recursos; r != NULL; r = r->next) {
        printf("  %s: Total=%d, Disponibles=%d\n", r->name, r->count, r->count_available);
    }

    printf("\nProcesos y acciones:\n");
    for (ProcessNode* p = procesos; p != NULL; p = p->next) {
        printf("Proceso %s:\n", p->process.pid);
        for (Action* a = p->actions; a != NULL; a = a->next) {
            const char* tipo_str = (a->type == READ) ? "READ" :
                                   (a->type == WRITE) ? "WRITE" : "RELEASE";
            printf("  %s %s en ciclo %d\n", tipo_str, a->resource, a->cycle);
        }
    }

    printf("\n--- Inicio de simulación ---\n\n");
    simular();

    return 0;
}