#include "timeline.h"
#include "eventlog.h"
#include "process.h"
#include "resource.h"
#include "action.h"
#include "sync.h"
#include <string.h>

void simulate_with_gui(Process *processes, int pcount,
                       Resource *resources, int rcount,
                       Action *actions, int acount,
                       SyncType mode) {
    EventLog log;
    init_event_log(&log);

    simulate(processes, pcount, resources, rcount, actions, acount, mode, &log);

    set_global_event_log(&log);
    draw_timeline();

    free_event_log(&log);
}
