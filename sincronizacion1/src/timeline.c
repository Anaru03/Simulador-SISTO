#include "timeline.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum {
    COL_TIME,
    COL_PROCESS,
    COL_DESCRIPTION,
    NUM_COLS
};

static GtkWidget *timeline_view = NULL;
static GtkListStore *list_store = NULL;
static EventLog *global_event_log = NULL;

GtkWidget* create_timeline_view(void) {
    if (timeline_view) return timeline_view;

    list_store = gtk_list_store_new(NUM_COLS, G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING);
    timeline_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
    gtk_widget_set_size_request(timeline_view, -1, 200);

    const char *titles[] = { "Tiempo", "Proceso", "Acción" };
    for (int i = 0; i < NUM_COLS; i++) {
        GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
        GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes(
            titles[i], renderer, "text", i, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(timeline_view), column);
    }

    return timeline_view;
}

void clear_global_event_log(void) {
    if (global_event_log) {
        free_event_log(global_event_log);
        free(global_event_log);
        global_event_log = NULL;
    }
}

void set_global_event_log(EventLog *log) {
    if (!log) return;

    clear_global_event_log();

    global_event_log = malloc(sizeof(EventLog));
    if (!global_event_log) {
        fprintf(stderr, "Error allocating memory for global_event_log\n");
        exit(EXIT_FAILURE);
    }

    global_event_log->count = log->count;
    global_event_log->capacity = log->capacity;
    global_event_log->events = malloc(sizeof(Event) * log->count);
    if (!global_event_log->events) {
        fprintf(stderr, "Error copying events\n");
        free(global_event_log);
        global_event_log = NULL;
        exit(EXIT_FAILURE);
    }

    memcpy(global_event_log->events, log->events, sizeof(Event) * log->count);
}

EventLog* get_global_event_log(void) {
    return global_event_log;
}

void draw_timeline(void) {
    if (!list_store || !global_event_log) return;

    gtk_list_store_clear(list_store);

    for (int i = 0; i < global_event_log->count; i++) {
        GtkTreeIter iter;
        Event *e = &global_event_log->events[i];

        gtk_list_store_append(list_store, &iter);
        gtk_list_store_set(list_store, &iter,
                           COL_TIME, e->time,
                           COL_PROCESS, e->process_id,
                           COL_DESCRIPTION, e->description,
                           -1);
    }
}
