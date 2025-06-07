#ifndef SYNC_H
#define SYNC_H

#include "action.h"
#include "resource.h"

typedef enum {
    SYNC_MUTEX,
    SYNC_SEMAPHORE
} SyncType;

void init_sync(SyncType type, Resource *resources, int resource_count);
void simulate_cycle(Action *actions, int action_count, int cycle);
void reset_sync();

#endif