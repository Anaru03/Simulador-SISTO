#ifndef TIMELINE_H
#define TIMELINE_H

#include <gtk/gtk.h>
#include "eventlog.h"

// Funciones para la vista del timeline y manejo del log global
GtkWidget* create_timeline_view(void);
void clear_global_event_log(void);
void set_global_event_log(EventLog *log);
EventLog* get_global_event_log(void);
void draw_timeline(void);

#endif // TIMELINE_H