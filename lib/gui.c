#include "gui.h"

void onActivate(GtkApplication *application)
{
    // Create a new window
    GtkWidget *first_window = gtk_application_window_new(application);

    // Set the title of the window
    gtk_window_set_title(GTK_WINDOW(first_window), "First Window");

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

    // Connect the create button to open the create account window
    // g_signal_connect(create_button, "clicked", G_CALLBACK(onCreateAccount), first_window);

    // Create a new grid
    GtkWidget *grid = gtk_grid_new();

    // Add the buttons to the grid as columns
    gtk_grid_attach(GTK_GRID(grid), create_button, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), close_button, 0, 1, 1, 1);

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

// void onCreateAccount(GtkWidget *button, gpointer data)
// {
//     GtkWidget *first_window = (GtkWidget *)data;
//     GtkWidget *second_window;

//     second_window = gtk_application_window_new((GtkApplication *)gtk_window_get_application(GTK_WINDOW(data)));
//     gtk_window_set_title(GTK_WINDOW(second_window), "Second Window");
//     gtk_window_set_default_size(GTK_WINDOW(second_window), 200, 200);
//     gtk_window_present(second_window);

//     // Close the first window
//     gtk_window_close(GTK_WINDOW(first_window));

//     // Create a new window
//     GtkWidget *second_window = gtk_application_window_new((GtkApplication *)gtk_window_get_application(GTK_WINDOW(first_window)));

//     // Set the default window size
//     gtk_window_set_default_size(GTK_WINDOW(second_window), 1000, 600);

//     // Create a new "Back" button
//     GtkWidget *back_button = gtk_button_new_with_label("Back");

//     // Connect the "Back" button to close the second window
//     g_signal_connect_swapped(back_button, "clicked", G_CALLBACK(gtk_window_close), second_window);

//     // Create a new "Create" button
//     GtkWidget *create_button = gtk_button_new_with_label("Create");

//     // Create a new grid
//     GtkWidget *grid = gtk_grid_new();

//     // Add the buttons to the grid as columns
//     gtk_grid_attach(GTK_GRID(grid), create_button, 0, 0, 1, 1);
//     gtk_grid_attach(GTK_GRID(grid), back_button, 0, 2, 1, 1);

//     // Center the grid within the window
//     gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
//     gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);

//     // Make the grid take up the full width of the window
//     gtk_widget_set_hexpand(grid, TRUE);
//     gtk_widget_set_vexpand(grid, TRUE);

//     // Make the buttons responsive to the window
//     gtk_widget_set_hexpand(back_button, TRUE);
//     gtk_widget_set_vexpand(back_button, TRUE);
//     gtk_widget_set_hexpand(create_button, TRUE);
//     gtk_widget_set_vexpand(create_button, TRUE);

//     // Add the grid to the window
//     gtk_window_set_child(GTK_WINDOW(second_window), grid);

//     // Show the window and all its content
//     gtk_window_present(GTK_WINDOW(second_window));
// }
