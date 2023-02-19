#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include "local_account.h"
#include "credentials.h"
#include "export.h"

typedef struct CreateEntriesStruct
{
    GtkWidget *main_window;
    GtkWidget *password_entry;
    GtkWidget *password_confirmation_entry;
} PASSWORD_ENTRIES_T;

typedef struct LogEntriesStruct
{
    GtkWidget *main_window;
    GtkWidget *password_entry;
} LOG_ENTRIES_T;

typedef struct GtkCredentialsStruct
{
    GtkWidget *main_window;
    GtkWidget *label_entry;
    GtkWidget *url_entry;
    GtkWidget *username_entry;
    GtkWidget *email_entry;
    GtkWidget *password_entry;
} GTKCREDENTIALS_T;

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

void onMainMenu(GtkWidget *main_window);

void onSelectedRowChanged(GtkListBox *listbox, GtkListBoxRow *row, gpointer user_data);

void onAddCredential(GtkWidget *button, gpointer data);

void onBackOnMainMenu(GtkWidget *button, gpointer data);

void onSendCredential(GtkWidget *button, gpointer data);

void onExportCredentialAsCSV(GtkWidget *button, gpointer data);

void onExportCredentialAsPDF(GtkWidget *button, gpointer data);

void onDeleteAccount(GtkWidget *button, gpointer data);

void onDeleteAccountConfirmation(GtkWidget *button, gpointer data);

#endif
