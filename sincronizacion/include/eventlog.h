#ifndef EVENTLOG_H
#define EVENTLOG_H

#define INITIAL_CAPACITY 10
#define MAX_DESC_LEN 127

typedef struct {
    int time;
    int process_id;
    char description[MAX_DESC_LEN + 1];
} Event;

typedef struct {
    Event *events;
    int count;
    int capacity;
} EventLog;

void init_event_log(EventLog *log);
void add_event(EventLog *log, int time, int process_id, const char *description);
void free_event_log(EventLog *log);

#endif // EVENTLOG_H