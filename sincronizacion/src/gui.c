#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "process.h"
#include "resource.h"
#include "action.h"
#include "sync.h"

#define UNUSED(x) (void)(x)

static GtkWidget *window;
static GtkWidget *textview_processes, *textview_resources, *textview_actions;
static GtkWidget *combo_sync_type;
static GtkWidget *button_load_processes, *button_load_resources, *button_load_actions;
static GtkWidget *button_start_simulation, *button_reset;
static GtkWidget *label_cycle;
static GtkWidget *scrolled_timeline;
static GtkWidget *drawing_area;

static Process *processes = NULL;
static int process_count = 0;

static Resource *resources = NULL;
static int resource_count = 0;

static Action *actions = NULL;
static int action_count = 0;

static SyncType current_sync_type = SYNC_MUTEX;

static int current_cycle = 0;
static int max_cycle = 0;

static gboolean running = FALSE;

static void update_textview(GtkWidget *textview, const char *text) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    gtk_text_buffer_set_text(buffer, text, -1);
}

static void load_processes_cb(GtkButton *button, gpointer user_data) {
    UNUSED(button);
    UNUSED(user_data);

    if (load_processes("data/procesos.txt") == 0) {
        processes = get_processes(&process_count);
        char buffer[1000] = "";
        for (int i = 0; i < process_count; i++) {
            char line[100];
            snprintf(line, sizeof(line), "%s, BT=%d, AT=%d, Priority=%d\n",
                     processes[i].pid, processes[i].bt, processes[i].at, processes[i].priority);
            strncat(buffer, line, sizeof(buffer) - strlen(buffer) - 1);
        }
        update_textview(textview_processes, buffer);
    }
}

static void load_resources_cb(GtkButton *button, gpointer user_data) {
    UNUSED(button);
    UNUSED(user_data);

    if (load_resources("data/recursos.txt") == 0) {
        resources = get_resources(&resource_count);
        char buffer[1000] = "";
        for (int i = 0; i < resource_count; i++) {
            char line[100];
            snprintf(line, sizeof(line), "%s, Count=%d\n",
                     resources[i].name, resources[i].counter);
            strncat(buffer, line, sizeof(buffer) - strlen(buffer) - 1);
        }
        update_textview(textview_resources, buffer);
    }
}

static void load_actions_cb(GtkButton *button, gpointer user_data) {
    UNUSED(button);
    UNUSED(user_data);

    if (load_actions("data/acciones.txt") == 0) {
        actions = get_actions(&action_count);
        char buffer[2000] = "";
        for (int i = 0; i < action_count; i++) {
            const char *act_str = (actions[i].action == ACTION_READ) ? "READ" : "WRITE";
            char line[100];
            snprintf(line, sizeof(line), "%s, %s, %s, Cycle=%d\n",
                     actions[i].pid, act_str, actions[i].resource, actions[i].cycle);
            strncat(buffer, line, sizeof(buffer) - strlen(buffer) - 1);
        }
        update_textview(textview_actions, buffer);

        max_cycle = 0;
        for (int i = 0; i < action_count; i++) {
            if (actions[i].cycle > max_cycle)
                max_cycle = actions[i].cycle;
        }

        int width_needed = (max_cycle + 2) * 40;
        int height_needed = action_count * 30 + 50;
        gtk_widget_set_size_request(drawing_area, width_needed, height_needed);
        gtk_widget_queue_draw(drawing_area);
    }
}

static void draw_timeline_cb(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    UNUSED(widget);
    UNUSED(user_data);
    if (!actions || action_count == 0) return;

    int block_width = 30;
    int block_height = 20;
    int margin = 5;

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 14);
    char cycle_str[20];
    snprintf(cycle_str, sizeof(cycle_str), "Cycle: %d", current_cycle);
    cairo_move_to(cr, margin, 20);
    cairo_show_text(cr, cycle_str);

    int x_start = margin;
    int y_start = 40;

    for (int i = 0; i < action_count; i++) {
        if (actions[i].cycle <= current_cycle) {
            int x = x_start + actions[i].cycle * (block_width + margin);
            int y = y_start + i * (block_height + margin);

            if (actions[i].accessed == 1) {
                cairo_set_source_rgb(cr, 0, 0.7, 0);
            } else {
                cairo_set_source_rgb(cr, 0.8, 0, 0);
            }

            cairo_rectangle(cr, x, y, block_width, block_height);
            cairo_fill(cr);

            cairo_set_source_rgb(cr, 1, 1, 1);
            const char *act_str = (actions[i].action == ACTION_READ) ? "R" : "W";
            cairo_move_to(cr, x + 5, y + 15);
            cairo_show_text(cr, act_str);
        }
    }
}

static gboolean tick_cb(gpointer user_data) {
    UNUSED(user_data);

    if (!running) return FALSE;

    simulate_cycle(actions, action_count, current_cycle);
    gtk_widget_queue_draw(drawing_area);

    current_cycle++;
    if (current_cycle > max_cycle) {
        running = FALSE;
        return FALSE;
    }

    char cycle_label[20];
    snprintf(cycle_label, sizeof(cycle_label), "Cycle: %d", current_cycle);
    gtk_label_set_text(GTK_LABEL(label_cycle), cycle_label);

    return TRUE;
}

static void start_simulation_cb(GtkButton *button, gpointer user_data) {
    UNUSED(button);
    UNUSED(user_data);

    current_cycle = 0;
    reset_sync();

    const gchar *sync_str = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_sync_type));
    current_sync_type = (sync_str && strcmp(sync_str, "Mutex") == 0) ? SYNC_MUTEX : SYNC_SEMAPHORE;
    init_sync(current_sync_type, resources, resource_count);

    running = TRUE;
    g_timeout_add(1000, tick_cb, NULL);
}

static void reset_cb(GtkButton *button, gpointer user_data) {
    UNUSED(button);
    UNUSED(user_data);

    running = FALSE;
    current_cycle = 0;
    reset_sync();
    gtk_widget_queue_draw(drawing_area);
    gtk_label_set_text(GTK_LABEL(label_cycle), "Cycle: 0");
}

void start_gui(int argc, char **argv) {
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Simulador de Sincronizaci\u00f3n");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox_main = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox_main);

    GtkWidget *hbox_top = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox_main), hbox_top, FALSE, FALSE, 5);

    button_load_processes = gtk_button_new_with_label("Cargar Procesos");
    g_signal_connect(button_load_processes, "clicked", G_CALLBACK(load_processes_cb), NULL);
    gtk_box_pack_start(GTK_BOX(hbox_top), button_load_processes, FALSE, FALSE, 5);

    button_load_resources = gtk_button_new_with_label("Cargar Recursos");
    g_signal_connect(button_load_resources, "clicked", G_CALLBACK(load_resources_cb), NULL);
    gtk_box_pack_start(GTK_BOX(hbox_top), button_load_resources, FALSE, FALSE, 5);

    button_load_actions = gtk_button_new_with_label("Cargar Acciones");
    g_signal_connect(button_load_actions, "clicked", G_CALLBACK(load_actions_cb), NULL);
    gtk_box_pack_start(GTK_BOX(hbox_top), button_load_actions, FALSE, FALSE, 5);

    combo_sync_type = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_sync_type), "Mutex");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_sync_type), "Semaphore");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_sync_type), 0);
    gtk_box_pack_start(GTK_BOX(hbox_top), combo_sync_type, FALSE, FALSE, 5);

    button_start_simulation = gtk_button_new_with_label("Iniciar Simulaci\u00f3n");
    g_signal_connect(button_start_simulation, "clicked", G_CALLBACK(start_simulation_cb), NULL);
    gtk_box_pack_start(GTK_BOX(hbox_top), button_start_simulation, FALSE, FALSE, 5);

    button_reset = gtk_button_new_with_label("Resetear");
    g_signal_connect(button_reset, "clicked", G_CALLBACK(reset_cb), NULL);
    gtk_box_pack_start(GTK_BOX(hbox_top), button_reset, FALSE, FALSE, 5);

    label_cycle = gtk_label_new("Cycle: 0");
    gtk_box_pack_start(GTK_BOX(hbox_top), label_cycle, FALSE, FALSE, 5);

    GtkWidget *hbox_textviews = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox_main), hbox_textviews, TRUE, TRUE, 5);

    GtkWidget *scrolled_processes = gtk_scrolled_window_new(NULL, NULL);
    textview_processes = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textview_processes), FALSE);
    gtk_container_add(GTK_CONTAINER(scrolled_processes), textview_processes);
    gtk_box_pack_start(GTK_BOX(hbox_textviews), scrolled_processes, TRUE, TRUE, 5);

    GtkWidget *scrolled_resources = gtk_scrolled_window_new(NULL, NULL);
    textview_resources = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textview_resources), FALSE);
    gtk_container_add(GTK_CONTAINER(scrolled_resources), textview_resources);
    gtk_box_pack_start(GTK_BOX(hbox_textviews), scrolled_resources, TRUE, TRUE, 5);

    GtkWidget *scrolled_actions = gtk_scrolled_window_new(NULL, NULL);
    textview_actions = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textview_actions), FALSE);
    gtk_container_add(GTK_CONTAINER(scrolled_actions), textview_actions);
    gtk_box_pack_start(GTK_BOX(hbox_textviews), scrolled_actions, TRUE, TRUE, 5);

    scrolled_timeline = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_timeline),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_hexpand(scrolled_timeline, TRUE);
    gtk_widget_set_vexpand(scrolled_timeline, TRUE);
    gtk_box_pack_start(GTK_BOX(vbox_main), scrolled_timeline, TRUE, TRUE, 5);

    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_hexpand(drawing_area, TRUE);
    gtk_widget_set_vexpand(drawing_area, TRUE);
    gtk_container_add(GTK_CONTAINER(scrolled_timeline), drawing_area);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_timeline_cb), NULL);

    gtk_widget_show_all(window);
    gtk_main();
}
