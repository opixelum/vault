#include "gui.h"

void onActivate(GtkApplication *application)
{
    // Create a new window
    GtkWidget *main_window = gtk_application_window_new(application);

    // Set the title of the window
    gtk_window_set_title(GTK_WINDOW(main_window), "Vault");

    // Set the default window height and width
    gtk_window_set_default_size(GTK_WINDOW(main_window), 1000, 600);

    // Create a new button
    GtkWidget *create_button = gtk_button_new_with_label("Create a local account");

    // Set the height request for the create button
    gtk_widget_set_size_request(create_button, 400, 60);

    // Create a new button
    GtkWidget *close_button = gtk_button_new_with_label("Close");

    // Set the height request for the close button
    gtk_widget_set_size_request(close_button, 400, 60);

    // Connect the close button to close the window
    g_signal_connect_swapped(close_button, "clicked", G_CALLBACK(gtk_window_close), main_window);

    // Connect the create button to open the create account window
    g_signal_connect(create_button, "clicked", G_CALLBACK(onCreateAccount), main_window);

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
    gtk_window_set_child(GTK_WINDOW(main_window), grid);

    // Show the window and all its content
    gtk_window_present(GTK_WINDOW(main_window));
}

void onCreateAccount(GtkWidget *button, gpointer data)
{
    GtkWidget *main_window = (GtkWidget *)data;

    // Set the title of the window
    gtk_window_set_title(GTK_WINDOW(main_window), "Vault - Local Account Creation");

    // Delete all content from the first window
    gtk_window_set_child(GTK_WINDOW(main_window), NULL);

    // Create a send button
    GtkWidget *send_button = gtk_button_new_with_label("Create account");

    // Set the height request for the send button
    gtk_widget_set_size_request(send_button, 400, 60);

    // Create a back button
    GtkWidget *back_button = gtk_button_new_with_label("Back");

    // Set the height request for the close button
    gtk_widget_set_size_request(back_button, 400, 60);

    // Create a text area for the password
    GtkWidget *password = gtk_entry_new();

    // Set the height request for the password text area
    gtk_widget_set_size_request(password, 400, 60);

    // Create a text area for the password verification
    GtkWidget *password_verify = gtk_entry_new();

    // Set the height request for the password verification text area
    gtk_widget_set_size_request(password_verify, 400, 60);

    // Connect the back button to open the main window
    g_signal_connect(back_button, "clicked", G_CALLBACK(onMainMenu), main_window);

    // Create a new grid
    GtkWidget *grid = gtk_grid_new();

    // Add the password text areas to the grid as columns
    gtk_grid_attach(GTK_GRID(grid), password, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), password_verify, 0, 2, 1, 1);

    // Add the buttons to the grid as columns
    gtk_grid_attach(GTK_GRID(grid), send_button, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), back_button, 0, 4, 1, 1);

    // Set the row spacing for the grid
    gtk_grid_set_row_spacing(GTK_GRID(grid), 20);

    // Center the grid within the window
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);

    // Make the grid take up the full width and height of the window
    gtk_widget_set_hexpand(grid, TRUE);
    gtk_widget_set_vexpand(grid, TRUE);

    // Make the buttons responsive to the window
    gtk_widget_set_hexpand(send_button, TRUE);
    gtk_widget_set_vexpand(send_button, TRUE);
    gtk_widget_set_hexpand(back_button, TRUE);
    gtk_widget_set_vexpand(back_button, TRUE);
    gtk_widget_set_hexpand(password, TRUE);
    gtk_widget_set_vexpand(password, TRUE);
    gtk_widget_set_hexpand(password_verify, TRUE);
    gtk_widget_set_vexpand(password_verify, TRUE);

    // Add the grid to the window
    gtk_window_set_child(GTK_WINDOW(main_window), grid);
}

void onMainMenu(GtkWidget *button, gpointer data)
{
    GtkWidget *main_window = (GtkWidget *)data;

    // Set the title of the window
    gtk_window_set_title(GTK_WINDOW(main_window), "Vault");

    // Delete all content from the first window
    gtk_window_set_child(GTK_WINDOW(main_window), NULL);

    // Create a new button
    GtkWidget *create_button = gtk_button_new_with_label("Create a local account");

    // Set the height request for the create button
    gtk_widget_set_size_request(create_button, 400, 60);

    // Create a new button
    GtkWidget *close_button = gtk_button_new_with_label("Close");

    // Set the height request for the close button
    gtk_widget_set_size_request(close_button, 400, 60);

    // Connect the close button to close the window
    g_signal_connect_swapped(close_button, "clicked", G_CALLBACK(gtk_window_close), main_window);

    // Connect the create button to open the create account window
    g_signal_connect(create_button, "clicked", G_CALLBACK(onCreateAccount), main_window);

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
    gtk_window_set_child(GTK_WINDOW(main_window), grid);
}
