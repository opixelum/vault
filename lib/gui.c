#include "gui.h"

// Initialize
int local_account_exists = 0;

void onActivate(GtkApplication *application)
{
    if (isLocalAccountExists())
    {
        local_account_exists = 1;
    }

    // Initialize
    GtkWidget *next_button;

    // Create a new window
    GtkWidget *main_window = gtk_application_window_new(application);

    // Set the title of the window
    gtk_window_set_title(GTK_WINDOW(main_window), "Vault");

    // Set the default window height and width
    gtk_window_set_default_size(GTK_WINDOW(main_window), 1000, 600);

    // Check if a local account exists
    if (local_account_exists)
    {
        // Create a new button
        next_button = gtk_button_new_with_label("Log in");
    }
    else
    {
        // Create a new button
        next_button = gtk_button_new_with_label("Create a local account");
    }

    // Set the height request for the create button
    gtk_widget_set_size_request(next_button, 400, 60);

    // Create a new button
    GtkWidget *close_button = gtk_button_new_with_label("Close");

    // Set the height request for the close button
    gtk_widget_set_size_request(close_button, 400, 60);

    // Connect the close button to close the window
    g_signal_connect_swapped(close_button, "clicked", G_CALLBACK(gtk_window_close), main_window);

    // Check if a local account exists
    if (local_account_exists)
    {
        // Connect the create button to open the log account window
        g_signal_connect(next_button, "clicked", G_CALLBACK(onLogAccount), main_window);
    }
    else
    {
        // Connect the create button to open the create account window
        g_signal_connect(next_button, "clicked", G_CALLBACK(onCreateAccount), main_window);
    }

    // Create a new grid
    GtkWidget *grid = gtk_grid_new();

    // Add the buttons to the grid as columns
    gtk_grid_attach(GTK_GRID(grid), next_button, 0, 0, 1, 1);
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
    gtk_widget_set_hexpand(next_button, TRUE);
    gtk_widget_set_vexpand(next_button, TRUE);
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

    // Set the placeholder text for the password entry
    gtk_entry_set_placeholder_text(GTK_ENTRY(password), "Password");

    // Set the height request for the password text area
    gtk_widget_set_size_request(password, 400, 60);

    // Hide the password
    gtk_entry_set_visibility(GTK_ENTRY(password), FALSE);

    // Create a text area for the password verification
    GtkWidget *password_confirmation = gtk_entry_new();

    // Set the placeholder text for the password entry
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_confirmation), "Password confirmation");

    // Set the height request for the password verification text area
    gtk_widget_set_size_request(password_confirmation, 400, 60);

    // Hide the password
    gtk_entry_set_visibility(GTK_ENTRY(password_confirmation), FALSE);

    // Create a struct to hold the password entries
    PASSWORD_ENTRIES_T *entries = malloc(sizeof *entries);
    entries->password_entry = password;
    entries->password_confirmation_entry = password_confirmation;

    // Connect the send button to local account function
    g_signal_connect(send_button, "clicked", G_CALLBACK(onSendCreatePassword), entries);

    // Connect the back button to open the main window
    g_signal_connect(back_button, "clicked", G_CALLBACK(onMainMenu), main_window);

    // Create a new grid
    GtkWidget *grid = gtk_grid_new();

    // Add the password text areas to the grid as columns
    gtk_grid_attach(GTK_GRID(grid), password, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), password_confirmation, 0, 2, 1, 1);

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
    gtk_widget_set_hexpand(password_confirmation, TRUE);
    gtk_widget_set_vexpand(password_confirmation, TRUE);

    // Add the grid to the window
    gtk_window_set_child(GTK_WINDOW(main_window), grid);
}

void onSendCreatePassword(GtkWidget *button, gpointer data)
{
    PASSWORD_ENTRIES_T *entries = data;

    // Get the password and password confirmation from the entries
    GtkEntryBuffer *send_password_buffer = gtk_entry_get_buffer(GTK_ENTRY(entries->password_entry));
    GtkEntryBuffer *send_password_confirmation_buffer = gtk_entry_get_buffer(GTK_ENTRY(entries->password_confirmation_entry));

    const char *send_password = gtk_entry_buffer_get_text(send_password_buffer);
    const char *send_password_confirmation = gtk_entry_buffer_get_text(send_password_confirmation_buffer);

    // Check if the password and password confirmation are the same
    if (strcmp(send_password, send_password_confirmation) != 0)
    {
        printf("Passwords do not match");

        onCreateAccount();
    }

    createLocalAccount((char *)send_password);
}

void onLogAccount(GtkWidget *button, gpointer data)
{
    GtkWidget *main_window = (GtkWidget *)data;

    // Set the title of the window
    gtk_window_set_title(GTK_WINDOW(main_window), "Vault - Local Account login");

    // Delete all content from the first window
    gtk_window_set_child(GTK_WINDOW(main_window), NULL);

    // Create a new button
    GtkWidget *sign_in_button = gtk_button_new_with_label("Sign in");

    // Set the height request for the sign in button
    gtk_widget_set_size_request(sign_in_button, 400, 60);

    // Create a new button
    GtkWidget *back_button = gtk_button_new_with_label("Back");

    // Set the height request for the back button
    gtk_widget_set_size_request(back_button, 400, 60);

    // Create a new password entry
    GtkWidget *password = gtk_entry_new();

    // Set the placeholder text for the password entry
    gtk_entry_set_placeholder_text(GTK_ENTRY(password), "Password");

    // Set the height request for the password text area
    gtk_widget_set_size_request(password, 400, 60);

    // Set the visibility of the password entry
    gtk_entry_set_visibility(GTK_ENTRY(password), FALSE);

    // Connect the back button to open the main window
    g_signal_connect(back_button, "clicked", G_CALLBACK(onMainMenu), main_window);

    // Connect the send button to local account function
    g_signal_connect(sign_in_button, "clicked", G_CALLBACK(onSendLogPassword), password);

    // Create a new grid
    GtkWidget *grid = gtk_grid_new();

    // Add the buttons to the grid as columns
    gtk_grid_attach(GTK_GRID(grid), password, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), sign_in_button, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), back_button, 0, 2, 1, 1);

    // Set the row spacing for the grid
    gtk_grid_set_row_spacing(GTK_GRID(grid), 20);

    // Center the grid within the window
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);

    // Make the grid take up the full width and height of the window
    gtk_widget_set_hexpand(grid, TRUE);
    gtk_widget_set_vexpand(grid, TRUE);

    // Make the buttons responsive to the window
    gtk_widget_set_hexpand(sign_in_button, TRUE);
    gtk_widget_set_vexpand(sign_in_button, TRUE);
    gtk_widget_set_hexpand(back_button, TRUE);
    gtk_widget_set_vexpand(back_button, TRUE);
    gtk_widget_set_hexpand(password, TRUE);
    gtk_widget_set_vexpand(password, TRUE);

    // Add the grid to the window
    gtk_window_set_child(GTK_WINDOW(main_window), grid);
}

void onSendLogPassword(GtkWidget *button, gpointer data)
{
    GtkWidget *password = (GtkWidget *)data;

    // Get the password from the entry
    GtkEntryBuffer *send_password_buffer = gtk_entry_get_buffer(GTK_ENTRY(password));

    const char *send_password = gtk_entry_buffer_get_text(send_password_buffer);

    connectLocalAccount((char *)send_password);
}

void onMainMenu(GtkWidget *button, gpointer data)
{
    // Initialize
    GtkWidget *next_button;

    GtkWidget *main_window = (GtkWidget *)data;

    // Set the title of the window
    gtk_window_set_title(GTK_WINDOW(main_window), "Vault");

    // Delete all content from the first window
    gtk_window_set_child(GTK_WINDOW(main_window), NULL);

    // Check if a local account exists
    if (local_account_exists)
    {
        // Create a new button
        next_button = gtk_button_new_with_label("Log in");
    }
    else
    {
        // Create a new button
        next_button = gtk_button_new_with_label("Create a local account");
    }

    // Set the height request for the create button
    gtk_widget_set_size_request(next_button, 400, 60);

    // Create a new button
    GtkWidget *close_button = gtk_button_new_with_label("Close");

    // Set the height request for the close button
    gtk_widget_set_size_request(close_button, 400, 60);

    // Connect the close button to close the window
    g_signal_connect_swapped(close_button, "clicked", G_CALLBACK(gtk_window_close), main_window);

    // Check if a local account exists
    if (local_account_exists)
    {
        // Connect the create button to open the log account window
        g_signal_connect(next_button, "clicked", G_CALLBACK(onLogAccount), main_window);
    }
    else
    {
        // Connect the create button to open the create account window
        g_signal_connect(next_button, "clicked", G_CALLBACK(onCreateAccount), main_window);
    }

    // Create a new grid
    GtkWidget *grid = gtk_grid_new();

    // Add the buttons to the grid as columns
    gtk_grid_attach(GTK_GRID(grid), next_button, 0, 0, 1, 1);
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
    gtk_widget_set_hexpand(next_button, TRUE);
    gtk_widget_set_vexpand(next_button, TRUE);
    gtk_widget_set_hexpand(close_button, TRUE);
    gtk_widget_set_vexpand(close_button, TRUE);

    // Add the grid to the window
    gtk_window_set_child(GTK_WINDOW(main_window), grid);
}
