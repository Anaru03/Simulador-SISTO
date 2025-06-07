#ifndef RESOURCE_H
#define RESOURCE_H

typedef struct Resource {
    char name[32];
    int count;
    int count_available;
    struct Resource* next;
} Resource;

#endif