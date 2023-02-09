#include "gui.h"

void onActivate(GtkApplication *application)
{
    // Create a new window
    GtkWidget *first_window = gtk_application_window_new(application);

    // Set the default window height and width
    gtk_window_set_default_size(GTK_WINDOW(first_window), 1000, 600);

    // Create a new button
    GtkWidget *create_button = gtk_button_new_with_label("Create a local account");

    // Set the height request for the create button
    gtk_widget_set_size_request(create_button, 400, 60);

    // Create a new button
    GtkWidget *close_button = gtk_button_new_with_label("Close");

    // Set the height request for the close button
    gtk_widget_set_size_request(close_button, 400, 60);

    // Connect the close button to close the window
    g_signal_connect_swapped(close_button, "clicked", G_CALLBACK(gtk_window_close), first_window);

    // Create a new grid
    GtkWidget *grid = gtk_grid_new();

    // Add the buttons to the grid as columns
    gtk_grid_attach(GTK_GRID(grid), create_button, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), close_button, 0, 2, 1, 1);

    // Set the row spacing for the grid
    gtk_grid_set_row_spacing(GTK_GRID(grid), 20);

    // Center the grid within the window
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);

    // Make the grid take up the full width and height of the window
    gtk_widget_set_hexpand(grid, TRUE);
    gtk_widget_set_vexpand(grid, TRUE);

    // Make the buttons responsive to the window
    gtk_widget_set_hexpand(create_button, TRUE);
    gtk_widget_set_vexpand(create_button, TRUE);
    gtk_widget_set_hexpand(close_button, TRUE);
    gtk_widget_set_vexpand(close_button, TRUE);

    // Add the grid to the window
    gtk_window_set_child(GTK_WINDOW(first_window), grid);

    // Show the window and all its content
    gtk_window_present(GTK_WINDOW(first_window));
}
