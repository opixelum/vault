#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include "local_account.h"

typedef struct PasswordEntriesStruct
{
    GtkWidget *password_entry;
    GtkWidget *password_confirmation_entry;
} PASSWORD_ENTRIES_T;

/**
 * @brief Create the main window of the application with all its widgets
 * @param app Pointer to GtkApplication variable, defined with
 * gtk_application_new()
 */
void onActivate(GtkApplication *application);

void onCreateAccount(GtkWidget *button, gpointer data);

void onLogAccount(GtkWidget *button, gpointer data);

void onLoginMenu(GtkWidget *button, gpointer data);

void onSendCreatePassword(GtkWidget *button, gpointer data);

void onSendLogPassword(GtkWidget *button, gpointer data);

void onMainMenu();

#endif
