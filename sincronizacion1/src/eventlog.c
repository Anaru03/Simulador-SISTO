#include "eventlog.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void init_event_log(EventLog *log) {
    log->count = 0;
    log->capacity = INITIAL_CAPACITY;
    log->events = malloc(sizeof(Event) * log->capacity);
    if (!log->events) {
        fprintf(stderr, "Error allocating EventLog\n");
        exit(EXIT_FAILURE);
    }
}

void add_event(EventLog *log, int time, int process_id, const char *description) {
    if (log->count >= log->capacity) {
        log->capacity *= 2;
        Event *new_events = realloc(log->events, sizeof(Event) * log->capacity);
        if (!new_events) {
            fprintf(stderr, "Error reallocating EventLog\n");
            exit(EXIT_FAILURE);
        }
        log->events = new_events;
    }
    Event *e = &log->events[log->count++];
    e->time = time;
    e->process_id = process_id;
    strncpy(e->description, description, MAX_DESC_LEN);
    e->description[MAX_DESC_LEN] = '\0'; // Null terminate
}

void free_event_log(EventLog *log) {
    free(log->events);
    log->events = NULL;
    log->count = 0;
    log->capacity = 0;
}
