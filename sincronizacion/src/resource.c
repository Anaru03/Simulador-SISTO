#include <stdio.h>
#include <string.h>
#include "resource.h"

static Resource resources[MAX_RESOURCES];
static int resource_count = 0;

int load_resources(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("Error abriendo recursos.txt");
        return -1;
    }
    resource_count = 0;
    char line[100];
    while (fgets(line, sizeof(line), f)) {
        if (resource_count >= MAX_RESOURCES) break;
        // Formato: R1, 1
        char name[20];
        int counter;
        if (sscanf(line, "%[^,], %d", name, &counter) == 2) {
            strcpy(resources[resource_count].name, name);
            resources[resource_count].counter = counter;
            resource_count++;
        }
    }
    fclose(f);
    return 0;
}

Resource* get_resources(int *count) {
    *count = resource_count;
    return resources;
}
