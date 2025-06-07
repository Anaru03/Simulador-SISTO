#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "action.h"

Action* load_actions(const char *filename, int *count) {
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;

    Action *head = NULL;
    Action *tail = NULL;
    *count = 0;

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        Action *a = malloc(sizeof(Action));
        if (!a) break;

        char type_str[16];
        // Formato esperado: PID TYPE RESOURCE CYCLE
        if (sscanf(line, "%19s %15s %31s %d", a->pid, type_str, a->resource, &a->cycle) != 4) {
            free(a);
            continue;
        }

        // Convierte string a enum
        if (strcmp(type_str, "READ") == 0) a->type = READ;
        else if (strcmp(type_str, "WRITE") == 0) a->type = WRITE;
        else if (strcmp(type_str, "RELEASE") == 0) a->type = RELEASE;
        else {
            free(a);
            continue;
        }

        a->next = NULL;

        if (!head) {
            head = tail = a;
        } else {
            tail->next = a;
            tail = a;
        }

        (*count)++;
    }

    fclose(file);
    return head;
}

void free_actions(Action *head) {
    Action *tmp;
    while (head) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}