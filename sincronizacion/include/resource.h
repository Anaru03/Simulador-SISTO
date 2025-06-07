#ifndef RESOURCE_H
#define RESOURCE_H

#define MAX_RESOURCES 50

typedef struct {
    char name[20];
    int counter;
} Resource;

int load_resources(const char *filename);
Resource* get_resources(int *count);

#endif