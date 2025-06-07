#ifndef SYNC_H
#define SYNC_H

#include "process.h"
#include "resource.h"
#include "action.h"
#include "timeline.h"  // Importa Event y EventLog desde aquí

typedef enum { MUTEX, SEMAPHORE } SyncType;

void init_event_log(EventLog *log);
void add_event(EventLog *log, int time, int process_id, const char *description);
void free_event_log(EventLog *log);

void simulate(Process* processes, int pcount,
              Resource* resources, int rcount,
              Action* actions, int acount,
              SyncType sync_type,
              EventLog *log);

#endif // SYNC_H