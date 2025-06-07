#ifndef ACTION_H
#define ACTION_H

#define MAX_ACTIONS 200

typedef enum {
    ACTION_READ,
    ACTION_WRITE,
    ACTION_UNKNOWN
} ActionType;

typedef struct {
    char pid[10];
    ActionType action;
    char resource[20];
    int cycle;
    int accessed; // 1 = acceso exitoso, 0 = espera
} Action;

int load_actions(const char *filename);
Action* get_actions(int *count);

#endif