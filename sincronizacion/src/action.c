#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "action.h"

static Action actions[MAX_ACTIONS];
static int action_count = 0;

static ActionType parse_action_type(const char *str) {
    if (strcasecmp(str, "READ") == 0) return ACTION_READ;
    if (strcasecmp(str, "WRITE") == 0) return ACTION_WRITE;
    return ACTION_UNKNOWN;
}

int load_actions(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("Error abriendo acciones.txt");
        return -1;
    }
    action_count = 0;
    char line[150];
    while (fgets(line, sizeof(line), f)) {
        if (action_count >= MAX_ACTIONS) break;
        char pid[10], action_str[10], resource[20];
        int cycle;
        if (sscanf(line, "%[^,], %[^,], %[^,], %d", pid, action_str, resource, &cycle) == 4) {
            strcpy(actions[action_count].pid, pid);
            actions[action_count].action = parse_action_type(action_str);
            strcpy(actions[action_count].resource, resource);
            actions[action_count].cycle = cycle;
            actions[action_count].accessed = 0; // Inicialmente en espera
            action_count++;
        }
    }
    fclose(f);
    return 0;
}

Action* get_actions(int *count) {
    *count = action_count;
    return actions;
}