#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

#include "process.h"
#include "resource.h"
#include "action.h"
#include "sync.h"
#include "timeline.h"

// Declaración explícita para evitar warning
void free_actions(Action *actions);

// Archivos seleccionados
static char *file_procesos = NULL;
static char *file_recursos = NULL;
static char *file_acciones = NULL;
static SyncType current_mode = MUTEX;

// Timeline view
static GtkWidget *timeline_view = NULL;

// Prototipos (por si acaso no están incluidos correctamente en headers)
Action *load_actions(const char *filename, int *count);
Process *load_processes(const char *filename, int *count);
Resource *load_resources(const char *filename, int *count);

static void on_file_select(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog;
    GtkWidget *parent = GTK_WIDGET(user_data);
    char **target = (char **)g_object_get_data(G_OBJECT(button), "target");

    dialog = gtk_file_chooser_dialog_new("Seleccionar archivo",
        GTK_WINDOW(parent),
        GTK_FILE_CHOOSER_ACTION_OPEN,
        "_Cancelar", GTK_RESPONSE_CANCEL,
        "_Abrir", GTK_RESPONSE_ACCEPT,
        NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        if (*target) {
            g_free(*target);
        }
        *target = filename;  // Se transfiere propiedad
    }

    gtk_widget_destroy(dialog);
}

static void on_run_simulation(GtkButton *button, gpointer user_data) {
    (void)button;  // evitar warning unused parameter

    GtkWidget *parent_window = GTK_WIDGET(user_data);

    if (!file_procesos || !file_recursos || !file_acciones) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent_window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_CLOSE,
            "Por favor selecciona todos los archivos antes de continuar.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    int pcount = 0, rcount = 0, acount = 0;
    Process *processes = load_processes(file_procesos, &pcount);
    Resource *resources = load_resources(file_recursos, &rcount);
    Action *actions = load_actions(file_acciones, &acount);

    if (!processes || !resources || !actions) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent_window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Error al cargar los archivos. Verifica los formatos.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        if (processes) free(processes);
        if (resources) free(resources);
        if (actions) free_actions(actions);

        return;
    }

    EventLog log;
    init_event_log(&log);

    simulate(processes, pcount, resources, rcount, actions, acount, current_mode, &log);

    set_global_event_log(&log);
    draw_timeline();

    free(processes);
    free(resources);
    free_actions(actions);
}

static void on_mode_toggle(GtkToggleButton *toggle, gpointer user_data) {
    (void)user_data;  // evitar warning unused parameter

    if (gtk_toggle_button_get_active(toggle)) {
        const char *label = gtk_button_get_label(GTK_BUTTON(toggle));
        current_mode = (g_strcmp0(label, "MUTEX") == 0) ? MUTEX : SEMAPHORE;
    }
}

static void on_reset(GtkButton *button, gpointer user_data) {
    (void)button;  // evitar warning unused parameter

    GtkWidget *vbox = GTK_WIDGET(user_data);

    if (timeline_view) {
        GtkWidget *parent = gtk_widget_get_parent(timeline_view);
        if (parent)
            gtk_container_remove(GTK_CONTAINER(parent), timeline_view);
        timeline_view = NULL;
    }

    clear_global_event_log();

    timeline_view = create_timeline_view();
    gtk_box_pack_start(GTK_BOX(vbox), timeline_view, TRUE, TRUE, 8);
    gtk_widget_show_all(vbox);
}

void launch_gui(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Simulador de Sincronización");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Frame modo sincronización
    GtkWidget *frame_mode = gtk_frame_new("Modo de Sincronización");
    GtkWidget *hbox_mode = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    GtkWidget *btn_mutex = gtk_radio_button_new_with_label(NULL, "MUTEX");
    GtkWidget *btn_semaphore = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(btn_mutex), "SEMAPHORE");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(btn_mutex), TRUE);

    g_signal_connect(btn_mutex, "toggled", G_CALLBACK(on_mode_toggle), NULL);
    g_signal_connect(btn_semaphore, "toggled", G_CALLBACK(on_mode_toggle), NULL);

    gtk_box_pack_start(GTK_BOX(hbox_mode), btn_mutex, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_mode), btn_semaphore, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(frame_mode), hbox_mode);
    gtk_box_pack_start(GTK_BOX(vbox), frame_mode, FALSE, FALSE, 0);

    // Grid selección archivos
    GtkWidget *grid_files = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid_files), 6);
    gtk_grid_set_column_spacing(GTK_GRID(grid_files), 6);

    GtkWidget *btn_proc = gtk_button_new_with_label("Seleccionar Procesos");
    GtkWidget *btn_res = gtk_button_new_with_label("Seleccionar Recursos");
    GtkWidget *btn_act = gtk_button_new_with_label("Seleccionar Acciones");

    g_object_set_data(G_OBJECT(btn_proc), "target", &file_procesos);
    g_object_set_data(G_OBJECT(btn_res), "target", &file_recursos);
    g_object_set_data(G_OBJECT(btn_act), "target", &file_acciones);

    g_signal_connect(btn_proc, "clicked", G_CALLBACK(on_file_select), window);
    g_signal_connect(btn_res, "clicked", G_CALLBACK(on_file_select), window);
    g_signal_connect(btn_act, "clicked", G_CALLBACK(on_file_select), window);

    gtk_grid_attach(GTK_GRID(grid_files), btn_proc, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_files), btn_res, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_files), btn_act, 0, 2, 1, 1);
    gtk_box_pack_start(GTK_BOX(vbox), grid_files, FALSE, FALSE, 0);

    // Botones control
    GtkWidget *hbox_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *btn_run = gtk_button_new_with_label("Ejecutar Simulación");
    GtkWidget *btn_reset = gtk_button_new_with_label("Resetear Timeline");

    g_signal_connect(btn_run, "clicked", G_CALLBACK(on_run_simulation), window);
    g_signal_connect(btn_reset, "clicked", G_CALLBACK(on_reset), vbox);

    gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_run, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_reset, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox_buttons, FALSE, FALSE, 0);

    // Timeline inicial
    timeline_view = create_timeline_view();
    gtk_box_pack_start(GTK_BOX(vbox), timeline_view, TRUE, TRUE, 8);

    gtk_widget_show_all(window);
    gtk_main();
}
