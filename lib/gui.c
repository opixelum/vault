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
    gtk_window_set_resizable(GTK_WINDOW(main_window), FALSE);

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
    if (local_account_exists == 1)
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
    entries->main_window = main_window;

    // Connect the send button to local account function
    g_signal_connect(send_button, "clicked", G_CALLBACK(onSendCreatePassword), entries);
    g_signal_connect(password_confirmation, "activate", G_CALLBACK(onSendCreatePassword), entries);

    // Connect the back button to open the main window
    g_signal_connect(back_button, "clicked", G_CALLBACK(onLoginMenu), main_window);

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

    // Add the grid to the window
    gtk_window_set_child(GTK_WINDOW(main_window), grid);
}

void onSendCreatePassword(GtkWidget *button, gpointer data)
{
    PASSWORD_ENTRIES_T *entries = data;

    // Get the main window
    GtkWidget *main_window = entries->main_window;

    // Get the password and password confirmation from the entries
    GtkEntryBuffer *send_password_buffer = gtk_entry_get_buffer(GTK_ENTRY(entries->password_entry));
    GtkEntryBuffer *send_password_confirmation_buffer = gtk_entry_get_buffer(GTK_ENTRY(entries->password_confirmation_entry));

    const char *send_password = gtk_entry_buffer_get_text(send_password_buffer);
    const char *send_password_confirmation = gtk_entry_buffer_get_text(send_password_confirmation_buffer);

    // Check if fields are empty
    if (strlen(send_password) == 0 || strlen(send_password_confirmation) == 0)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("You must fill all the fields");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if the password and password confirmation are the same
    if (strcmp(send_password, send_password_confirmation) != 0)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Current password and new password must be different");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if the password is valid
    if (strlen(send_password_confirmation) < 12 || strlen(send_password_confirmation) >= 255)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Password length needs to be between 12 and 255 characters");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if the password contains uppercase letters
    if (strpbrk(send_password_confirmation, "ABCDEFGHIJKLMNOPQRSTUVWXYZ") == NULL)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Password needs to contain at least one uppercase letter");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if the password contains lowercase letters
    if (strpbrk(send_password_confirmation, "abcdefghijklmnopqrstuvwxyz") == NULL)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Password needs to contain at least one lowercase letter");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if the password contains numbers
    if (strpbrk(send_password_confirmation, "0123456789") == NULL)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Password needs to contain at least one number");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if the password contains a ,
    if (strpbrk(send_password_confirmation, ",") != NULL)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("You can't use a , in your password");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if the password contains special characters
    if (strpbrk(send_password_confirmation, "!@#$%^&*()_+-=[]{};':\"./<>?") == NULL)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Password needs to contain at least one special character");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }
    else
    {
        // Check if creation is successful
        if (createLocalAccount((char *)send_password) == 0)
        {
            local_account_exists = 1;
            onMainMenu(main_window);
        }
    }
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
    g_signal_connect(back_button, "clicked", G_CALLBACK(onLoginMenu), main_window);

    // Create a struct to hold data
    LOG_ENTRIES_T *log_entry = malloc(sizeof *log_entry);
    log_entry->password_entry = password;
    log_entry->main_window = main_window;

    // Connect the send button to local account function
    g_signal_connect(sign_in_button, "clicked", G_CALLBACK(onSendLogPassword), log_entry);
    g_signal_connect(password, "activate", G_CALLBACK(onSendLogPassword), log_entry);

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

    // Add the grid to the window
    gtk_window_set_child(GTK_WINDOW(main_window), grid);
}

void onSendLogPassword(GtkWidget *button, gpointer data)
{
    LOG_ENTRIES_T *entries = data;

    // Get the main window
    GtkWidget *main_window = entries->main_window;

    // Get the password from the entry
    GtkEntryBuffer *send_password_buffer = gtk_entry_get_buffer(GTK_ENTRY(entries->password_entry));

    const char *send_password = gtk_entry_buffer_get_text(send_password_buffer);

    // Check if field is empty
    if (strlen(send_password) == 0)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Password field cannot be empty");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if connection is successful
    if (connectLocalAccount((char *)send_password) == 0)
    {
        local_account_exists = 1;
        onMainMenu(main_window);
    }
    else
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Incorrect password");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }
}

void onLoginMenu(GtkWidget *button, gpointer data)
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

    // Add the grid to the window
    gtk_window_set_child(GTK_WINDOW(main_window), grid);
}

void onMainMenu(GtkWidget *main_window)
{
    // Set the title of the window
    gtk_window_set_title(GTK_WINDOW(main_window), "Vault");

    // Delete all content from the first window
    gtk_window_set_child(GTK_WINDOW(main_window), NULL);

    // Create the headerbar
    GtkWidget *headerbar = gtk_header_bar_new();
    gtk_widget_set_hexpand(headerbar, TRUE);
    gtk_widget_set_vexpand(headerbar, FALSE);

    //---------------------M A N A G E   A C C O U N T---------------------//

    // Add an image
    GtkWidget *user_logo = gtk_image_new_from_file("images/logo.png");
    gtk_widget_set_size_request(user_logo, 35, 35);

    // Add the image to a box
    GtkWidget *user_logo_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_append(GTK_BOX(user_logo_box), user_logo);

    // Add a new menu button
    GtkWidget *manage_menu = gtk_menu_button_new();
    gtk_widget_set_hexpand(manage_menu, FALSE);
    gtk_widget_set_vexpand(manage_menu, FALSE);

    // Create a new menu
    GtkWidget *manage_menu_popover = gtk_popover_new();
    gtk_menu_button_set_popover(GTK_MENU_BUTTON(manage_menu), manage_menu_popover);

    // Create a new grid
    GtkWidget *manage_menu_grid = gtk_grid_new();

    // Create a new button
    GtkWidget *manage_menu_change_password = gtk_button_new_with_label("Change password");

    // Create a new button
    GtkWidget *manage_menu_delete_account = gtk_button_new_with_label("Delete account");

    // Create a new button
    GtkWidget *manage_menu_log_out = gtk_button_new_with_label("Log out");

    // Connect the buttons to their respective functions
    g_signal_connect(manage_menu_change_password, "clicked", G_CALLBACK(onChangePassword), main_window);
    g_signal_connect(manage_menu_delete_account, "clicked", G_CALLBACK(onDeleteAccount), main_window);
    g_signal_connect_swapped(manage_menu_log_out, "clicked", G_CALLBACK(gtk_window_close), main_window);

    // Add the buttons to the grid as columns
    gtk_grid_attach(GTK_GRID(manage_menu_grid), manage_menu_change_password, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(manage_menu_grid), manage_menu_delete_account, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(manage_menu_grid), manage_menu_log_out, 0, 2, 1, 1);

    // Set the row spacing for the grid
    gtk_grid_set_row_spacing(GTK_GRID(manage_menu_grid), 10);

    // Center the grid within the window
    gtk_widget_set_halign(manage_menu_grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(manage_menu_grid, GTK_ALIGN_CENTER);

    // Add the grid to the popover
    gtk_popover_set_child(GTK_POPOVER(manage_menu_popover), manage_menu_grid);

    // Add the menu button to the image box
    gtk_box_append(GTK_BOX(user_logo_box), manage_menu);

    // Add a margin to the image box
    gtk_widget_set_margin_start(user_logo_box, 10);
    gtk_widget_set_margin_end(user_logo_box, 10);

    // Add the image box to the headerbar
    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), user_logo_box);

    // //---------------------S E A R C H   B A R---------------------//
    // // Create the search entry
    // GtkWidget *search_entry = gtk_search_entry_new();
    // gtk_widget_set_hexpand(search_entry, TRUE);
    // gtk_widget_set_vexpand(search_entry, FALSE);

    // // Add the search entry to the headerbar
    // gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), search_entry);

    //---------------------E X P O R T   A C C O U N T---------------------//
    // Add a new menu button
    GtkWidget *export_menu = gtk_menu_button_new();
    gtk_widget_set_hexpand(export_menu, FALSE);
    gtk_widget_set_vexpand(export_menu, FALSE);

    // Create a new menu
    GtkWidget *export_menu_popover = gtk_popover_new();
    gtk_menu_button_set_popover(GTK_MENU_BUTTON(export_menu), export_menu_popover);

    // Set the label of the menu button
    gtk_menu_button_set_label(GTK_MENU_BUTTON(export_menu), "Export");

    // Create a new grid
    GtkWidget *export_grid = gtk_grid_new();

    // Set the row spacing for the grid
    gtk_grid_set_row_spacing(GTK_GRID(export_grid), 10);

    // Add the grid to the popover
    gtk_popover_set_child(GTK_POPOVER(export_menu_popover), export_grid);

    // Create a new button
    GtkWidget *csv_button = gtk_button_new_with_label("Export as CSV");

    // Add the button to the grid
    gtk_grid_attach(GTK_GRID(export_grid), csv_button, 0, 0, 1, 1);

    // Connect the export button to open the export window
    g_signal_connect(csv_button, "clicked", G_CALLBACK(onExportCredentialAsCSV), main_window);

    // Create a new button
    GtkWidget *pdf_button = gtk_button_new_with_label("Export as PDF");

    // Add the button to the grid
    gtk_grid_attach(GTK_GRID(export_grid), pdf_button, 0, 1, 1, 1);

    // Connect the import button to open the import window
    g_signal_connect(pdf_button, "clicked", G_CALLBACK(onExportCredentialAsPDF), main_window);

    // Add the menu to the headerbar
    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), export_menu);

    // Add the headerbar to the main window
    gtk_window_set_titlebar(GTK_WINDOW(main_window), headerbar);

    //---------------------A D D  B U T T O N---------------------//
    // Add a new button on the top right
    GtkWidget *add_button = gtk_button_new_with_label("+ Add new item");
    gtk_header_bar_pack_end(GTK_HEADER_BAR(headerbar), add_button);

    // Connect the add button to open the add credential window
    g_signal_connect(add_button, "clicked", G_CALLBACK(onAddCredential), main_window);

    //---------------------L I S T   O F   C R E D E N T I A L S---------------------//
    // Create a new grid
    GtkWidget *grid = gtk_grid_new();
    gtk_widget_set_hexpand(grid, TRUE);
    gtk_widget_set_vexpand(grid, TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

    // Add the grid to the window
    gtk_window_set_child(GTK_WINDOW(main_window), grid);

    // Create a new scrolled window
    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    gtk_widget_set_hexpand(scrolled_window, TRUE);
    gtk_widget_set_vexpand(scrolled_window, TRUE);

    // Doesn't allows horizontal scrolling
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    // Create a new listbox
    GtkWidget *listbox = gtk_list_box_new();
    gtk_widget_set_hexpand(listbox, TRUE);
    gtk_widget_set_vexpand(listbox, TRUE);

    // Get the labels of the database
    char **labels = getLabels();

    int i = 0;

    while (labels[i] != NULL)
    {
        // Create a new box
        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_widget_set_hexpand(box, TRUE);
        gtk_widget_set_vexpand(box, FALSE);

        // Create a new label
        GtkWidget *label = gtk_label_new(labels[i]);
        gtk_widget_set_hexpand(label, TRUE);
        gtk_widget_set_vexpand(label, FALSE);

        // Add the label to the box
        gtk_box_append(GTK_BOX(box), label);

        // // Create a new button
        // GtkWidget *edit_button = gtk_button_new_with_label("Edit");
        // gtk_widget_set_hexpand(edit_button, FALSE);
        // gtk_widget_set_vexpand(edit_button, FALSE);

        // Create a new button
        GtkWidget *delete_button = gtk_button_new_with_label("Delete");
        gtk_widget_set_hexpand(delete_button, FALSE);
        gtk_widget_set_vexpand(delete_button, FALSE);

        // Hold data in a struct
        DELETE_T *data = malloc(sizeof *data);
        data->label = labels[i];
        data->main_window = main_window;

        // Connect the delete button to delete the credential
        g_signal_connect(delete_button, "clicked", G_CALLBACK(onDeleteCredential), data);

        // Add the button to the box
        // gtk_box_append(GTK_BOX(box), edit_button);
        gtk_box_append(GTK_BOX(box), delete_button);

        // Add space between the buttons
        gtk_box_set_spacing(GTK_BOX(box), 10);

        // Add margin to the box
        gtk_widget_set_margin_start(box, 10);
        gtk_widget_set_margin_end(box, 10);

        // Add the box to the listbox
        gtk_list_box_insert(GTK_LIST_BOX(listbox), box, i);

        i++;
    }

    // Add the listbox to the scrolled window
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), listbox);

    // Add the scrolled window to the left side of the grid
    gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 0, 1, 1);

    // Connect the listbox to the function that will be called when a row is selected
    g_signal_connect(listbox, "row-selected", G_CALLBACK(onSelectedRowChanged), grid);
}

void onSelectedRowChanged(GtkListBox *listbox, GtkListBoxRow *row, gpointer data)
{
    // Get the data from the struct
    GtkWidget *grid = (GtkWidget *)data;

    // Get the index of the selected row
    if (row == NULL)
        return;

    // Get text from the row
    GtkWidget *get_box = gtk_list_box_row_get_child(row);

    // Find the GtkLabel in the GtkBox
    GtkWidget *child = gtk_widget_get_first_child(get_box);
    GtkWidget *label = NULL;

    while (child != NULL)
    {
        if (GTK_IS_LABEL(child))
        {
            label = child;
            break;
        }
        child = gtk_widget_get_next_sibling(child);
    }

    // Get the text from the GtkLabel
    const char *text = gtk_label_get_text(GTK_LABEL(label));

    // Get the credentials
    CREDENTIALS_T *credentials = getCredentials(text);

    // Get informations from the credentials
    char *label_text = credentials->label;
    char *url = credentials->url;
    char *username = credentials->username;
    char *email = credentials->email;
    char *password = credentials->password;

    // Delete the right side of the grid
    if (gtk_grid_get_child_at(GTK_GRID(grid), 1, 0) != NULL)
    {
        gtk_grid_remove(GTK_GRID(grid), gtk_grid_get_child_at(GTK_GRID(grid), 1, 0));
    }

    // Format text
    char *text_label = "<b>Label:</b>\n";
    char *text_url = "<b>URL:</b>\n";
    char *text_username = "<b>Username:</b>\n";
    char *text_email = "<b>Email:</b>\n";
    char *text_password = "<b>Password:</b>\n";
    char *text_frame = "<b>Details</b>";

    // Check if entries contains "NULL"
    if (strcmp(url, "NULL") == 0)
    {
        url = "";
    }

    if (strcmp(username, "NULL") == 0)
    {
        username = "";
    }

    if (strcmp(email, "NULL") == 0)
    {
        email = "";
    }

    if (strcmp(password, "NULL") == 0)
    {
        password = "";
    }

    // Create new labels
    GtkWidget *label_label = gtk_label_new(text_label);
    gtk_label_set_xalign(GTK_LABEL(label_label), 0);
    gtk_label_set_yalign(GTK_LABEL(label_label), 0.5);
    gtk_label_set_use_markup(GTK_LABEL(label_label), TRUE);

    GtkWidget *label_url = gtk_label_new(text_url);
    gtk_label_set_xalign(GTK_LABEL(label_url), 0);
    gtk_label_set_yalign(GTK_LABEL(label_url), 0.5);
    gtk_label_set_use_markup(GTK_LABEL(label_url), TRUE);

    GtkWidget *label_username = gtk_label_new(text_username);
    gtk_label_set_xalign(GTK_LABEL(label_username), 0);
    gtk_label_set_yalign(GTK_LABEL(label_username), 0.5);
    gtk_label_set_use_markup(GTK_LABEL(label_username), TRUE);

    GtkWidget *label_email = gtk_label_new(text_email);
    gtk_label_set_xalign(GTK_LABEL(label_email), 0);
    gtk_label_set_yalign(GTK_LABEL(label_email), 0.5);
    gtk_label_set_use_markup(GTK_LABEL(label_email), TRUE);

    GtkWidget *label_password = gtk_label_new(text_password);
    gtk_label_set_xalign(GTK_LABEL(label_password), 0);
    gtk_label_set_yalign(GTK_LABEL(label_password), 0.5);
    gtk_label_set_use_markup(GTK_LABEL(label_password), TRUE);

    GtkWidget *label_frame = gtk_label_new(text_frame);
    gtk_label_set_use_markup(GTK_LABEL(label_frame), TRUE);

    // Create new entries
    GtkWidget *entry_label = gtk_entry_new();
    gtk_editable_set_text(GTK_EDITABLE(entry_label), label_text);
    gtk_editable_set_editable(GTK_EDITABLE(entry_label), FALSE);

    GtkWidget *entry_url = gtk_entry_new();
    gtk_editable_set_text(GTK_EDITABLE(entry_url), url);
    gtk_editable_set_editable(GTK_EDITABLE(entry_url), FALSE);

    GtkWidget *entry_username = gtk_entry_new();
    gtk_editable_set_text(GTK_EDITABLE(entry_username), username);
    gtk_editable_set_editable(GTK_EDITABLE(entry_username), FALSE);

    GtkWidget *entry_email = gtk_entry_new();
    gtk_editable_set_text(GTK_EDITABLE(entry_email), email);
    gtk_editable_set_editable(GTK_EDITABLE(entry_email), FALSE);

    GtkWidget *entry_password = gtk_entry_new();
    gtk_editable_set_text(GTK_EDITABLE(entry_password), password);
    gtk_editable_set_editable(GTK_EDITABLE(entry_password), FALSE);

    // Create a new box for each label and entry pair
    GtkWidget *box_label_label = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *box_label_url = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *box_label_username = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *box_label_email = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *box_label_password = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    // Add the labels and entries to the boxes
    gtk_box_append(GTK_BOX(box_label_label), label_label);
    gtk_box_append(GTK_BOX(box_label_label), entry_label);
    gtk_box_append(GTK_BOX(box_label_url), label_url);
    gtk_box_append(GTK_BOX(box_label_url), entry_url);
    gtk_box_append(GTK_BOX(box_label_username), label_username);
    gtk_box_append(GTK_BOX(box_label_username), entry_username);
    gtk_box_append(GTK_BOX(box_label_email), label_email);
    gtk_box_append(GTK_BOX(box_label_email), entry_email);
    gtk_box_append(GTK_BOX(box_label_password), label_password);
    gtk_box_append(GTK_BOX(box_label_password), entry_password);

    // Create a new grid for all boxes
    GtkWidget *grid_details = gtk_grid_new();

    // Add the boxes to the grid
    gtk_grid_attach(GTK_GRID(grid_details), box_label_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_details), box_label_url, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_details), box_label_username, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_details), box_label_email, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_details), box_label_password, 0, 4, 1, 1);

    // Set the spacing between the boxes
    gtk_grid_set_row_spacing(GTK_GRID(grid_details), 30);

    // Add margin to the grid

    gtk_widget_set_margin_start(grid_details, 20);

    // Add a frame around the grid
    GtkWidget *frame = gtk_frame_new(NULL);
    gtk_frame_set_label_align(GTK_FRAME(frame), 0.5);
    gtk_frame_set_label_widget(GTK_FRAME(frame), label_frame);
    gtk_widget_set_margin_top(frame, 20);
    gtk_widget_set_margin_bottom(frame, 20);
    gtk_widget_set_margin_start(frame, 20);
    gtk_widget_set_margin_end(frame, 20);
    gtk_frame_set_child(GTK_FRAME(frame), grid_details);

    // Add the frame to the right side of the grid
    gtk_grid_attach(GTK_GRID(grid), frame, 1, 0, 2, 1);
}

void onAddCredential(GtkWidget *button, gpointer data)
{
    GtkWidget *main_window = (GtkWidget *)data;

    // Set the title of the window
    gtk_window_set_title(GTK_WINDOW(main_window), "Vault - Add new item");

    // Delete all content from the first window
    gtk_window_set_child(GTK_WINDOW(main_window), NULL);

    // Create an add button
    GtkWidget *add_button = gtk_button_new_with_label("Add item");

    // Set the height request for the add button
    gtk_widget_set_size_request(add_button, 200, 60);

    // Create a back button
    GtkWidget *back_button = gtk_button_new_with_label("Back");

    // Set the height request for the back button
    gtk_widget_set_size_request(back_button, 200, 60);

    // Create a text area for the label
    GtkWidget *label = gtk_entry_new();

    // Set the placeholder text for the label entry
    gtk_entry_set_placeholder_text(GTK_ENTRY(label), "Label");

    // Set the height request for the label text area
    gtk_widget_set_size_request(label, 200, 60);

    // Create a text area for the url
    GtkWidget *url = gtk_entry_new();

    // Set the placeholder text for the url entry
    gtk_entry_set_placeholder_text(GTK_ENTRY(url), "URL");

    // Set the height request for the url text area
    gtk_widget_set_size_request(url, 200, 60);

    // Create a text area for the username
    GtkWidget *username = gtk_entry_new();

    // Set the placeholder text for the username entry
    gtk_entry_set_placeholder_text(GTK_ENTRY(username), "Username");

    // Set the height request for the username text area
    gtk_widget_set_size_request(username, 200, 60);

    // Create a text area for the email
    GtkWidget *email = gtk_entry_new();

    // Set the placeholder text for the email entry
    gtk_entry_set_placeholder_text(GTK_ENTRY(email), "Email");

    // Set the height request for the email text area
    gtk_widget_set_size_request(email, 200, 60);

    // Create a text area for the password
    GtkWidget *password = gtk_entry_new();

    // Set the placeholder text for the password entry
    gtk_entry_set_placeholder_text(GTK_ENTRY(password), "Password");

    // Set the height request for the password text area
    gtk_widget_set_size_request(password, 200, 60);

    // Hide the password
    gtk_entry_set_visibility(GTK_ENTRY(password), FALSE);

    // Add an generate password button
    GtkWidget *button_generate_password = gtk_button_new_with_label("Generate password");

    // Set the height request for the generate password button
    gtk_widget_set_size_request(button_generate_password, 200, 60);

    // Connect the generate password button to the generate password function
    g_signal_connect(button_generate_password, "clicked", G_CALLBACK(onGeneratePasswordClicked), main_window);

    // Connect the back button to open the main window
    g_signal_connect(back_button, "clicked", G_CALLBACK(onBackOnMainMenu), main_window);

    // Create a new grid
    GtkWidget *grid = gtk_grid_new();

    // Add the text areas to the grid as columns
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), url, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), username, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), email, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), password, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_generate_password, 0, 5, 1, 1);

    // Set the row spacing for the grid
    gtk_grid_set_row_spacing(GTK_GRID(grid), 20);

    // Center the grid within the window
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);

    // Add a new grid
    GtkWidget *button_grid = gtk_grid_new();

    // Add the buttons to the grid as columns
    gtk_grid_attach(GTK_GRID(button_grid), add_button, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(button_grid), back_button, 0, 0, 1, 1);

    gtk_grid_set_column_spacing(GTK_GRID(button_grid), 20);

    // Make the grid take up the full width and height of the window
    gtk_widget_set_hexpand(grid, TRUE);
    gtk_widget_set_vexpand(grid, TRUE);

    // Add the grid to the window
    gtk_window_set_child(GTK_WINDOW(main_window), grid);

    // Add the button grid to the window
    gtk_grid_attach(GTK_GRID(grid), button_grid, 0, 6, 1, 1);

    // Create a struct to hold the entries
    GTKCREDENTIALS_T *entries = malloc(sizeof *entries);
    entries->main_window = main_window;

    entries->label_entry = label;

    entries->url_entry = url;

    entries->username_entry = username;

    entries->email_entry = email;

    entries->password_entry = password;

    // Connect the add button if entries meet the requirements

    g_signal_connect(add_button, "clicked", G_CALLBACK(onCheckCredentials), entries);
    g_signal_connect(add_button, "activate", G_CALLBACK(onCheckCredentials), entries);
}

void onCheckCredentials(GtkWidget *button, gpointer data)
{
    GTKCREDENTIALS_T *entries = (GTKCREDENTIALS_T *)data;
    // Get the main window
    GtkWidget *main_window = entries->main_window;

    // Get the text from the entries
    GtkEntryBuffer *label_buffer = gtk_entry_get_buffer(GTK_ENTRY(entries->label_entry));
    const char *label = gtk_entry_buffer_get_text(label_buffer);
    GtkEntryBuffer *url_buffer = gtk_entry_get_buffer(GTK_ENTRY(entries->url_entry));
    const char *url = gtk_entry_buffer_get_text(url_buffer);
    GtkEntryBuffer *username_buffer = gtk_entry_get_buffer(GTK_ENTRY(entries->username_entry));
    const char *username = gtk_entry_buffer_get_text(username_buffer);
    GtkEntryBuffer *email_buffer = gtk_entry_get_buffer(GTK_ENTRY(entries->email_entry));
    const char *email = gtk_entry_buffer_get_text(email_buffer);
    GtkEntryBuffer *password_buffer = gtk_entry_get_buffer(GTK_ENTRY(entries->password_entry));
    const char *password = gtk_entry_buffer_get_text(password_buffer);

    if (gtk_entry_get_text_length(GTK_ENTRY(entries->label_entry)) <= 0)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("You must enter a label");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Hide the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }
    else if (gtk_entry_get_text_length(GTK_ENTRY(entries->label_entry)) > 20)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Label must be less than 20 characters");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Hide the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }
    else if (strpbrk(label, ",") != NULL)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Label cannot contain a comma");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Hide the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }
    else if (doesLabelExist(label))
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Label already exists");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if the url doesn't contain a ,
    if (strpbrk(url, ",") != NULL)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("URL cannot contain a comma");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Hide the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if the url less than 255 characters
    if (strlen(url) >= 255)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("URL must be less than 255 characters");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Hide the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if the username doesn't contain a ,
    if (strpbrk(username, ",") != NULL)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Username cannot contain a comma");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Hide the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if the username is less than 255 characters
    if (strlen(username) >= 255)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Username must be less than 255 characters");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Hide the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if the email doesn't contain a ,
    if (strpbrk(email, ",") != NULL)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Email cannot contain a comma");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Hide the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if the email is less than 255 characters
    if (strlen(email) >= 255)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Email must be less than 255 characters");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Hide the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    if (strlen(password) == 0)
    {
        // Nothing
    }
    else if (strlen(password) < 12 || strlen(password) >= 255)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Password length needs to be between 12 and 255 characters");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }
    else if (strpbrk(password, "ABCDEFGHIJKLMNOPQRSTUVWXYZ") == NULL)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Password needs to contain at least uppercase letter");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }
    else if (strpbrk(password, "abcdefghijklmnopqrstuvwxyz") == NULL)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Password needs to contain at least one lowercase letter");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }
    else if (strpbrk(password, "0123456789") == NULL)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Password needs to contain at least one number");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }
    else if (strpbrk(password, ",") != NULL)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("You can't use a , in your password");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }
    else if (strpbrk(password, "!@#$%^&*()_+-=[]{};':\"./<>?") == NULL)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Password needs to contain at least one special character");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    onSendCredential(button, data);
}

void onSendCredential(GtkWidget *button, gpointer data)
{
    GTKCREDENTIALS_T *entries = (GTKCREDENTIALS_T *)data;

    // Initialize the variables
    const char *label = NULL;
    const char *url = NULL;
    const char *username = NULL;
    const char *email = NULL;
    const char *password = NULL;

    // Get the text from the entries
    GtkEntryBuffer *label_buffer = gtk_entry_get_buffer(GTK_ENTRY(entries->label_entry));
    label = gtk_entry_buffer_get_text(label_buffer);

    if (gtk_entry_get_text_length(GTK_ENTRY(entries->url_entry)) > 0)
    {
        GtkEntryBuffer *url_buffer = gtk_entry_get_buffer(GTK_ENTRY(entries->url_entry));
        url = gtk_entry_buffer_get_text(url_buffer);
    }
    else
    {
        url = "";
    }

    if (gtk_entry_get_text_length(GTK_ENTRY(entries->username_entry)) > 0)
    {
        GtkEntryBuffer *username_buffer = gtk_entry_get_buffer(GTK_ENTRY(entries->username_entry));
        username = gtk_entry_buffer_get_text(username_buffer);
    }
    else
    {
        username = "";
    }

    if (gtk_entry_get_text_length(GTK_ENTRY(entries->email_entry)) > 0)
    {
        GtkEntryBuffer *email_buffer = gtk_entry_get_buffer(GTK_ENTRY(entries->email_entry));
        email = gtk_entry_buffer_get_text(email_buffer);
    }
    else
    {
        email = "";
    }

    if (gtk_entry_get_text_length(GTK_ENTRY(entries->password_entry)) > 0)
    {
        GtkEntryBuffer *password_buffer = gtk_entry_get_buffer(GTK_ENTRY(entries->password_entry));
        password = gtk_entry_buffer_get_text(password_buffer);
    }
    else
    {
        password = "";
    }

    // Create a new struct with text from the entries
    CREDENTIALS_T credentials = {
        label,
        url,
        username,
        email,
        password};

    // Check if item has been added
    if (storeCredentials(credentials) == 0)
    {
        onMainMenu(entries->main_window);
    }
}

void onBackOnMainMenu(GtkWidget *button, gpointer data)
{
    GtkWidget *main_window = (GtkWidget *)data;

    onMainMenu(main_window);
}

void onExportCredentialAsPDF(GtkWidget *button, gpointer data)
{
    if (exportCredentialsAsPDF() == 0)
    {
        // Get the main window
        GtkWidget *main_window = (GtkWidget *)data;

        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Export successful");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("File exported successfully to the desktop");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Hide the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_destroy, window);
    }
}

void onExportCredentialAsCSV(GtkWidget *button, gpointer data)
{
    if (exportCredentialsAsCSV() == 0)
    {
        // Get the main window
        GtkWidget *main_window = (GtkWidget *)data;

        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Export successful");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("File exported successfully to the desktop");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Hide the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_destroy, window);
    }
}

void onDeleteAccount(GtkWidget *button, gpointer data)
{
    GtkWidget *main_window = (GtkWidget *)data;

    // Set the title of the window
    gtk_window_set_title(GTK_WINDOW(main_window), "Vault - Delete account");

    // Delete all content from the first window
    gtk_window_set_child(GTK_WINDOW(main_window), NULL);

    // Create a yes button
    GtkWidget *yes_button = gtk_button_new_with_label("Yes");

    // Set the height request for the yes button
    gtk_widget_set_size_request(yes_button, 200, 60);

    // Create a no button
    GtkWidget *no_button = gtk_button_new_with_label("No");

    // Set the height request for the no button
    gtk_widget_set_size_request(no_button, 200, 60);

    // Create a label for the password
    GtkWidget *password_label = gtk_label_new("Are you sure you want to delete your account?");
    gtk_label_set_xalign(GTK_LABEL(password_label), 0);

    // Center the label
    gtk_widget_set_halign(password_label, GTK_ALIGN_CENTER);

    // Create a text area for the password
    GtkWidget *password = gtk_entry_new();

    // Set the placeholder text for the password entry
    gtk_entry_set_placeholder_text(GTK_ENTRY(password), "Password");

    // Set the height request for the password text area
    gtk_widget_set_size_request(password, 200, 60);

    // Hide the password
    gtk_entry_set_visibility(GTK_ENTRY(password), FALSE);

    // Connect the no button to open the main window
    g_signal_connect(no_button, "clicked", G_CALLBACK(onBackOnMainMenu), main_window);

    LOG_ENTRIES_T *delete_entry = malloc(sizeof *delete_entry);
    delete_entry->password_entry = password;
    delete_entry->main_window = main_window;

    // Connect the yes button to delete the account
    g_signal_connect(yes_button, "clicked", G_CALLBACK(onDeleteAccountConfirmation), delete_entry);
    g_signal_connect(password, "activate", G_CALLBACK(onDeleteAccountConfirmation), delete_entry);

    // Create a new grid
    GtkWidget *grid = gtk_grid_new();

    // Add the text areas to the grid as columns
    gtk_grid_attach(GTK_GRID(grid), password_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), password, 0, 1, 1, 1);

    // Set the row spacing for the grid
    gtk_grid_set_row_spacing(GTK_GRID(grid), 20);

    // Center the grid within the window
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);

    // Add a new grid
    GtkWidget *button_grid = gtk_grid_new();

    // Add the buttons to the grid as columns
    gtk_grid_attach(GTK_GRID(button_grid), yes_button, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(button_grid), no_button, 0, 0, 1, 1);

    gtk_grid_set_column_spacing(GTK_GRID(button_grid), 20);

    // Make the grid take up the full width and height of the window
    gtk_widget_set_hexpand(grid, TRUE);
    gtk_widget_set_vexpand(grid, TRUE);

    // Add the grid to the window
    gtk_window_set_child(GTK_WINDOW(main_window), grid);

    // Add the button grid to the window
    gtk_grid_attach(GTK_GRID(grid), button_grid, 0, 2, 1, 1);
}

void onDeleteAccountConfirmation(GtkWidget *button, gpointer data)
{
    LOG_ENTRIES_T *entries = data;

    // Get the password from the entry
    GtkEntryBuffer *delete_password_buffer = gtk_entry_get_buffer(GTK_ENTRY(entries->password_entry));

    const char *delete_password = gtk_entry_buffer_get_text(delete_password_buffer);

    // Check if field is empty
    if (strlen(delete_password) == 0)
    {
        // Get the main window
        GtkWidget *main_window = entries->main_window;

        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Please enter your password");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Hide the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_destroy, window);

        return;
    }

    // Check if the password is correct
    if (connectLocalAccount(delete_password) != 0)
    {
        // Get the main window
        GtkWidget *main_window = entries->main_window;

        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("The password you entered is incorrect");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Hide the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_destroy, window);

        return;
    }

    // Check if the password is correct and delete the account
    if (deleteLocalAccount(delete_password) == 0)
    {
        // Get the main window
        GtkWidget *main_window = entries->main_window;

        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Account deleted");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Your account has been deleted");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Hide the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_destroy, window);

        local_account_exists = 0;

        // Redirect to create account page
        onLoginMenu(NULL, main_window);
    }
}

void onDeleteCredential(GtkWidget *button, gpointer data)
{
    DELETE_T *entries = data;

    // Get the main window
    GtkWidget *main_window = entries->main_window;

    // Create a top-level window
    GtkWidget *window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(window), "Delete item");

    gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

    // Set the default size of the window
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

    // Make the window transient for the main window
    gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

    // Create a new label
    GtkWidget *label = gtk_label_new("Are you sure you want to delete this item?");

    // Add margins to the label
    gtk_widget_set_margin_top(label, 10);

    // Add buttons to the window
    GtkWidget *yes_button = gtk_button_new_with_label("Yes");
    GtkWidget *no_button = gtk_button_new_with_label("No");

    // Add this window
    entries->window = window;

    // Connect the yes button to delete the account
    g_signal_connect(yes_button, "clicked", G_CALLBACK(onDeleteCredentialConfirmation), entries);

    // Connect the no button to close the window
    g_signal_connect_swapped(no_button, "clicked", G_CALLBACK(gtk_window_destroy), window);

    // Create a new grid
    GtkWidget *grid = gtk_grid_new();

    // Add the text areas to the grid as columns
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    // Set the row spacing for the grid
    gtk_grid_set_row_spacing(GTK_GRID(grid), 20);

    // Center the grid within the window
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);

    // Add a new grid
    GtkWidget *button_grid = gtk_grid_new();

    // Add the buttons to the grid as columns
    gtk_grid_attach(GTK_GRID(button_grid), yes_button, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(button_grid), no_button, 0, 0, 1, 1);

    // Set size to the buttons
    gtk_widget_set_size_request(yes_button, 100, 30);
    gtk_widget_set_size_request(no_button, 100, 30);

    // Add margins to the button grid
    gtk_widget_set_margin_top(button_grid, 10);
    gtk_widget_set_margin_bottom(button_grid, 10);

    gtk_grid_set_column_spacing(GTK_GRID(button_grid), 20);

    // Center the button grid within the window
    gtk_widget_set_halign(button_grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button_grid, GTK_ALIGN_CENTER);

    // Make the grid take up the full width and height of the window
    gtk_widget_set_hexpand(grid, TRUE);
    gtk_widget_set_vexpand(grid, TRUE);

    // Add the grid to the window
    gtk_window_set_child(GTK_WINDOW(window), grid);

    // Add the button grid to the window
    gtk_grid_attach(GTK_GRID(grid), button_grid, 0, 5, 1, 1);

    // Show the window
    gtk_widget_show(window);
}

void onDeleteCredentialConfirmation(GtkWidget *button, gpointer data)
{
    DELETE_T *entries = data;

    // Get the main window
    GtkWidget *main_window = entries->main_window;

    // Get the old window
    GtkWidget *old_window = entries->window;

    // Delete contents of the window
    gtk_window_set_child(GTK_WINDOW(main_window), NULL);

    // Get the label
    GtkWidget *delete_label = entries->label;

    // Delete the credential
    deleteCredentials(delete_label);

    // Close the old window
    gtk_window_destroy(GTK_WINDOW(old_window));

    // Create a top-level window
    GtkWidget *window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(window), "Item deleted");

    gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

    // Set the default size of the window
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

    // Make the window transient for the main window
    gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

    // Create a new label
    GtkWidget *label = gtk_label_new("The item has been deleted");

    // Add the label to the window
    gtk_window_set_child(GTK_WINDOW(window), label);

    // Show the window
    gtk_widget_show(window);

    // Hide the window after 2 seconds
    g_timeout_add_seconds(2, (GSourceFunc)gtk_window_destroy, window);

    // Redirect to the main menu
    onBackOnMainMenu(NULL, main_window);
}

void onChangePassword(GtkWidget *button, gpointer data)
{
    GtkWidget *main_window = (GtkWidget *)data;

    // Set the title of the window
    gtk_window_set_title(GTK_WINDOW(main_window), "Vault - Change password");

    // Delete all content from the first window
    gtk_window_set_child(GTK_WINDOW(main_window), NULL);

    // Create a yes button
    GtkWidget *yes_button = gtk_button_new_with_label("Yes");

    // Set the height request for the yes button
    gtk_widget_set_size_request(yes_button, 200, 60);

    // Create a no button
    GtkWidget *no_button = gtk_button_new_with_label("No");

    // Set the height request for the no button
    gtk_widget_set_size_request(no_button, 200, 60);

    // Create a label for the password
    GtkWidget *password_label = gtk_label_new("Do you want to change your password?");
    gtk_label_set_xalign(GTK_LABEL(password_label), 0);

    // Center the label
    gtk_widget_set_halign(password_label, GTK_ALIGN_CENTER);

    // Create a text area for the password
    GtkWidget *password = gtk_entry_new();

    // Set the placeholder text for the password entry
    gtk_entry_set_placeholder_text(GTK_ENTRY(password), "Current password");

    // Set the height request for the password text area
    gtk_widget_set_size_request(password, 200, 60);

    // Hide the password
    gtk_entry_set_visibility(GTK_ENTRY(password), FALSE);

    // Create a new password entry
    GtkWidget *new_password = gtk_entry_new();

    // Set the placeholder text for the password entry
    gtk_entry_set_placeholder_text(GTK_ENTRY(new_password), "New password");

    // Set the height request for the password text area
    gtk_widget_set_size_request(new_password, 200, 60);

    // Hide the password
    gtk_entry_set_visibility(GTK_ENTRY(new_password), FALSE);

    // Create a new password entry
    GtkWidget *confirm_password = gtk_entry_new();

    // Set the placeholder text for the password entry
    gtk_entry_set_placeholder_text(GTK_ENTRY(confirm_password), "Confirm password");

    // Set the height request for the password text area
    gtk_widget_set_size_request(confirm_password, 200, 60);

    // Hide the password
    gtk_entry_set_visibility(GTK_ENTRY(confirm_password), FALSE);

    // Connect the no button to open the main window
    g_signal_connect(no_button, "clicked", G_CALLBACK(onBackOnMainMenu), main_window);

    CHANGEPASSWORD_T *change_password = malloc(sizeof *change_password);
    change_password->main_window = main_window;
    change_password->old_password_entry = password;
    change_password->password_entry = new_password;
    change_password->password_confirmation_entry = confirm_password;

    // Connect the yes button to the change password function
    g_signal_connect(yes_button, "clicked", G_CALLBACK(onChangePasswordConfirmation), change_password);
    g_signal_connect(confirm_password, "activate", G_CALLBACK(onChangePasswordConfirmation), change_password);

    // Create a new grid
    GtkWidget *grid = gtk_grid_new();

    // Add the text areas to the grid as columns
    gtk_grid_attach(GTK_GRID(grid), password_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), password, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), new_password, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), confirm_password, 0, 3, 1, 1);

    // Set the row spacing for the grid
    gtk_grid_set_row_spacing(GTK_GRID(grid), 20);

    // Center the grid within the window
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);

    // Add a new grid
    GtkWidget *button_grid = gtk_grid_new();

    // Add the buttons to the grid as columns
    gtk_grid_attach(GTK_GRID(button_grid), yes_button, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(button_grid), no_button, 0, 0, 1, 1);

    gtk_grid_set_column_spacing(GTK_GRID(button_grid), 20);

    // Make the grid take up the full width and height of the window
    gtk_widget_set_hexpand(grid, TRUE);
    gtk_widget_set_vexpand(grid, TRUE);

    // Add the grid to the window
    gtk_window_set_child(GTK_WINDOW(main_window), grid);

    // Add the button grid to the window
    gtk_grid_attach(GTK_GRID(grid), button_grid, 0, 4, 1, 1);
}

void onChangePasswordConfirmation(GtkWidget *button, gpointer data)
{
    CHANGEPASSWORD_T *change_password = data;

    // Get the main window
    GtkWidget *main_window = change_password->main_window;

    // Get the password and password confirmation from the entries
    GtkEntryBuffer *password_buffer = gtk_entry_get_buffer(GTK_ENTRY(change_password->old_password_entry));
    GtkEntryBuffer *new_password_buffer = gtk_entry_get_buffer(GTK_ENTRY(change_password->password_entry));
    GtkEntryBuffer *confirm_password_buffer = gtk_entry_get_buffer(GTK_ENTRY(change_password->password_confirmation_entry));

    const char *current_password = gtk_entry_buffer_get_text(password_buffer);
    const char *new_password = gtk_entry_buffer_get_text(new_password_buffer);
    const char *confirm_password = gtk_entry_buffer_get_text(confirm_password_buffer);

    // Check if all the fields are filled
    if (strlen(current_password) == 0 || strlen(new_password) == 0 || strlen(confirm_password) == 0)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("You must fill all the fields");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if the password is correct
    if (connectLocalAccount(current_password) != 0)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Current password is incorrect");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if the password and password confirmation are the same
    if (strcmp(new_password, confirm_password) != 0)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Passwords and password confirmation do not match");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if the current password is not the same as the new password
    if (strcmp(current_password, new_password) == 0)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Current password and new password must be different");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if the password is valid
    if (strlen(confirm_password) < 12 || strlen(confirm_password) >= 255)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Password length needs to be between 12 and 255 characters");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if the password contains uppercase letters
    if (strpbrk(confirm_password, "ABCDEFGHIJKLMNOPQRSTUVWXYZ") == NULL)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Password needs to contain at least one uppercase letter");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if the password contains lowercase letters
    if (strpbrk(confirm_password, "abcdefghijklmnopqrstuvwxyz") == NULL)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Password needs to contain at least one lowercase letter");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if the password contains numbers
    if (strpbrk(confirm_password, "0123456789") == NULL)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Password needs to contain at least one number");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if the password contains a ,
    if (strpbrk(confirm_password, ",") != NULL)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("You can't use a , in your password");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if the password contains special characters
    if (strpbrk(confirm_password, "!@#$%^&*()_+-=[]{};':\"./<>?") == NULL)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Password needs to contain at least one special character");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Change the password
    if (createLocalAccount(new_password) == 0)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Success");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Password changed successfully");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        // Redirect to the login page
        onLoginMenu(NULL, main_window);
    }
    else
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Failed to change password");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);
    }
}

void onGeneratePasswordClicked(GtkWidget *button, gpointer data)
{
    // Get the main window
    GtkWidget *main_window = (GtkWidget *)data;

    // Create a top-level window
    GtkWidget *window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(window), "Generate Password");

    gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

    // Set the default size of the window
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

    // Make the window transient for the main window
    gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

    // Create a new label
    GtkWidget *label = gtk_label_new("Do you want to generate a password?");

    // Create a new entry for the password size
    GtkWidget *password_size_entry = gtk_entry_new();

    // Add a placeholder to the entry
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_size_entry), "Password length");

    // Add buttons to the window
    GtkWidget *generate_button = gtk_button_new_with_label("Generate");
    GtkWidget *no_button = gtk_button_new_with_label("No");

    GENERATEPASSWORD_T *generate_password_t = malloc(sizeof *generate_password_t);
    generate_password_t->main_window = main_window;
    generate_password_t->window = window;
    generate_password_t->password_length = password_size_entry;

    // Connect the yes button to delete the account
    g_signal_connect(generate_button, "clicked", G_CALLBACK(onGeneratePassword), generate_password_t);
    g_signal_connect(password_size_entry, "activate", G_CALLBACK(onGeneratePassword), generate_password_t);

    // Connect the no button to close the window
    g_signal_connect_swapped(no_button, "clicked", G_CALLBACK(gtk_window_destroy), window);

    // Create a new grid
    GtkWidget *grid = gtk_grid_new();

    // Add the text areas to the grid as columns
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    // Add margin to the label
    gtk_widget_set_margin_top(label, 10);

    // Add the password size entry to the grid as a column
    gtk_grid_attach(GTK_GRID(grid), password_size_entry, 0, 1, 1, 1);

    // Set the row spacing for the grid
    gtk_grid_set_row_spacing(GTK_GRID(grid), 20);

    // Center the grid within the window
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);

    // Add a new grid
    GtkWidget *button_grid = gtk_grid_new();

    // Add the buttons to the grid as columns
    gtk_grid_attach(GTK_GRID(button_grid), generate_button, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(button_grid), no_button, 0, 0, 1, 1);

    // Set size to the buttons
    gtk_widget_set_size_request(generate_button, 100, 30);
    gtk_widget_set_size_request(no_button, 100, 30);

    // Add margins to the button grid
    gtk_widget_set_margin_top(button_grid, 10);
    gtk_widget_set_margin_bottom(button_grid, 10);

    // Set the column spacing for the button grid
    gtk_grid_set_column_spacing(GTK_GRID(button_grid), 20);

    // Center the button grid within the window
    gtk_widget_set_halign(button_grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button_grid, GTK_ALIGN_CENTER);

    // Make the grid take up the full width and height of the window
    gtk_widget_set_hexpand(grid, TRUE);
    gtk_widget_set_vexpand(grid, TRUE);

    // Add the grid to the window
    gtk_window_set_child(GTK_WINDOW(window), grid);

    // Add the button grid to the window
    gtk_grid_attach(GTK_GRID(grid), button_grid, 0, 2, 1, 1);

    // Show the window
    gtk_widget_show(window);
}

void onGeneratePassword(GtkWidget *button, gpointer data)
{
    GENERATEPASSWORD_T *generate_password_t = data;

    // Get the main window
    GtkWidget *main_window = generate_password_t->main_window;

    // Get the window
    GtkWidget *old_window = generate_password_t->window;

    // Get the password length
    GtkEntryBuffer *password_length_buffer = gtk_entry_get_buffer(GTK_ENTRY(generate_password_t->password_length));
    const char *password_length_text = gtk_entry_buffer_get_text(password_length_buffer);

    // Check if the password length is empty
    if (strlen(password_length_text) == 0)
    {

        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Field cannot be empty");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Convert the password length to an integer
    int password_length = atoi(password_length_text);

    // Check if the password length is different than 0
    if (password_length == 0)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Password length needs to be an integer");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Check if the password length is upper than 12 characters and lower than 255
    if (password_length < 12 || password_length >= 255)
    {
        // Create a top-level window
        GtkWidget *window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(window), "Error");

        gtk_window_set_deletable(GTK_WINDOW(window), FALSE);

        // Set the default size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

        // Make the window transient for the main window
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

        // Create a new label
        GtkWidget *label = gtk_label_new("Password length needs to be between 12 and 255 characters");

        // Add the label to the window
        gtk_window_set_child(GTK_WINDOW(window), label);

        // Show the window
        gtk_widget_show(window);

        // Close the window after 2 seconds
        g_timeout_add_seconds(2, (GSourceFunc)gtk_window_close, window);

        return;
    }

    // Create a new password
    char *password = genPswd(password_length, 0);

    // Close the old window
    gtk_window_close(GTK_WINDOW(old_window));

    // Create a top-level window
    GtkWidget *window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(window), "Password");

    gtk_window_set_deletable(GTK_WINDOW(window), TRUE);

    // Set the default size of the window
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

    // Make the window transient for the main window
    gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));

    // Create a new entry
    GtkWidget *entry = gtk_entry_new();

    // Set the text of the entry
    gtk_editable_set_text(GTK_EDITABLE(entry), password);

    // Make the entry read only
    gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE);

    // Add the entry to the window
    gtk_window_set_child(GTK_WINDOW(window), entry);

    // Show the window
    gtk_widget_show(window);
}
