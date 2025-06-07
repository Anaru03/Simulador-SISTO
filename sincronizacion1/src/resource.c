#include "../include/resource.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Resource* load_resources(const char* filename, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;

    Resource* resources = malloc(sizeof(Resource) * 100);
    if (!resources) {
        fclose(file);
        return NULL;
    }

    char line[100];
    int i = 0;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%31[^,], %d", resources[i].name, &resources[i].count) == 2) {
            resources[i].count_available = resources[i].count;  // Inicializar disponibles
            i++;
        }
    }

    fclose(file);
    *count = i;
    return resources;
}
