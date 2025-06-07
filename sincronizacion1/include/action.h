#ifndef ACTION_H
#define ACTION_H

typedef enum {
    READ,
    WRITE,
    RELEASE
} ActionType;

typedef struct Action {
    char pid[20];          // para identificar qué proceso ejecuta esta acción
    ActionType type;
    char resource[32];
    int cycle;
    struct Action* next;    // para lista enlazada
} Action;

#endif
