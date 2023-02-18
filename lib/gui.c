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

    // Get the password and password confirmation from the entries
    GtkEntryBuffer *send_password_buffer = gtk_entry_get_buffer(GTK_ENTRY(entries->password_entry));
    GtkEntryBuffer *send_password_confirmation_buffer = gtk_entry_get_buffer(GTK_ENTRY(entries->password_confirmation_entry));

    const char *send_password = gtk_entry_buffer_get_text(send_password_buffer);
    const char *send_password_confirmation = gtk_entry_buffer_get_text(send_password_confirmation_buffer);

    // Check if the password and password confirmation are the same
    if (strcmp(send_password, send_password_confirmation) != 0)
    {
        return;
    }
    else
    {
        // Check if creation is successful
        if (createLocalAccount((char *)send_password) == 0)
        {
            onMainMenu(entries->main_window);
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

    // Get the password from the entry
    GtkEntryBuffer *send_password_buffer = gtk_entry_get_buffer(GTK_ENTRY(entries->password_entry));

    const char *send_password = gtk_entry_buffer_get_text(send_password_buffer);

    // Check if connection is successful
    if (connectLocalAccount((char *)send_password) == 0)
    {
        onMainMenu(entries->main_window);
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

    // Add a button around the image
    GtkWidget *user_logo_button = gtk_button_new();
    gtk_button_set_child(GTK_BUTTON(user_logo_button), user_logo);

    // Add the button to the headerbar
    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), user_logo_button);

    // Connect the user logo to open the manage account window
    // g_signal_connect(user_logo, "clicked", G_CALLBACK(onUserLogoClicked), main_window);

    //---------------------S E A R C H   B A R---------------------//
    // Create the search entry
    GtkWidget *search_entry = gtk_search_entry_new();
    gtk_widget_set_hexpand(search_entry, TRUE);
    gtk_widget_set_vexpand(search_entry, FALSE);

    // Add the search entry to the headerbar
    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), search_entry);

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

    // Create a new listbox
    GtkWidget *listbox = gtk_list_box_new();
    gtk_widget_set_hexpand(listbox, TRUE);
    gtk_widget_set_vexpand(listbox, TRUE);

    // Add the credentials to the listbox
    char *credentials[3][5] = {
        {"Facebook", "facebook.com", "ANto", "anto@gmail.com", "password"},
        {"Google", "google.com", "ANto", "anto@gmail.com", "password"},
        {"Twitter", "twitter.com", "ANto", "anto@gmail.com", "password"}};

    for (int i = 0; i < 3; i++)
    {
        // Create a new box
        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_widget_set_hexpand(box, TRUE);
        gtk_widget_set_vexpand(box, FALSE);

        // Create a new label
        GtkWidget *label = gtk_label_new(credentials[i][0]);
        gtk_widget_set_hexpand(label, TRUE);
        gtk_widget_set_vexpand(label, FALSE);

        // Add the label to the box
        gtk_box_append(GTK_BOX(box), label);

        // Create a new button
        GtkWidget *edit_button = gtk_button_new_with_label("Edit");
        gtk_widget_set_hexpand(edit_button, FALSE);
        gtk_widget_set_vexpand(edit_button, FALSE);

        // Create a new button
        GtkWidget *delete_button = gtk_button_new_with_label("Delete");
        gtk_widget_set_hexpand(delete_button, FALSE);
        gtk_widget_set_vexpand(delete_button, FALSE);

        // Add the button to the box
        gtk_box_append(GTK_BOX(box), edit_button);
        gtk_box_append(GTK_BOX(box), delete_button);

        // Add space between the buttons
        gtk_box_set_spacing(GTK_BOX(box), 10);

        // Add the box to the listbox
        gtk_list_box_insert(GTK_LIST_BOX(listbox), box, i);
    }

    // Add the listbox to the left side of the grid
    gtk_grid_attach(GTK_GRID(grid), listbox, 0, 0, 1, 1);

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

    int index = gtk_list_box_row_get_index(row);

    // Delete the right side of the grid
    if (gtk_grid_get_child_at(GTK_GRID(grid), 1, 0) != NULL)
    {
        gtk_grid_remove(GTK_GRID(grid), gtk_grid_get_child_at(GTK_GRID(grid), 1, 0));
    }

    // Create a new box
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_hexpand(box, TRUE);
    gtk_widget_set_vexpand(box, TRUE);

    // Add index to the box
    char index_string[10];
    sprintf(index_string, "%d", index);
    GtkWidget *label = gtk_label_new(index_string);
    gtk_widget_set_hexpand(label, TRUE);
    gtk_widget_set_vexpand(label, FALSE);

    // Add the label to the box
    gtk_box_append(GTK_BOX(box), label);

    // Add the box to the right side of the grid
    gtk_grid_attach(GTK_GRID(grid), box, 1, 0, 2, 1);
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

    // // Create a struct to hold the password entries
    // PASSWORD_ENTRIES_T *entries = malloc(sizeof *entries);
    // entries->password_entry = password;
    // entries->password_confirmation_entry = password_confirmation;
    // entries->main_window = main_window;

    // // Connect the send button to local account function
    // g_signal_connect(add_button, "clicked", G_CALLBACK(onSendCreatePassword), entries);
    // g_signal_connect(password_confirmation, "activate", G_CALLBACK(onSendCreatePassword), entries);

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
    gtk_grid_attach(GTK_GRID(grid), button_grid, 0, 5, 1, 1);
}

void onBackOnMainMenu(GtkWidget *button, gpointer data)
{
    GtkWidget *main_window = (GtkWidget *)data;

    onMainMenu(main_window);
}
