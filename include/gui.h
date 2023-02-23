#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include "local_account.h"
#include "credentials.h"
#include "export.h"
#include "password.h"

typedef struct HeaderChildStruct
{
    GtkWidget *box;
    GtkWidget *button;
    GtkWidget *menu;
} HEADER_CHILD_T;
typedef struct HeaderBarStruct
{
    GtkWidget *main_window;
    GtkWidget *header_bar;
    HEADER_CHILD_T *header_child;
} HEADER_BAR_T;

typedef struct CreateAccountStruct
{
    GtkWidget *main_window;
    GtkWidget *window;
} CREATE_ACCOUNT_T;

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

typedef struct EditCredentialStruct
{
    GtkWidget *main_window;
    char *label;
} EDIT_CREDENTIAL_T;

typedef struct GtkCredentialsStruct
{
    GtkWidget *main_window;
    GtkWidget *label_entry;
    GtkWidget *url_entry;
    GtkWidget *username_entry;
    GtkWidget *email_entry;
    GtkWidget *password_entry;
    char editOrAdd;
} GTK_CREDENTIALS_T;

typedef struct deleteStruct
{
    GtkWidget *main_window;
    GtkWidget *window;
    char *label;
} DELETE_T;

typedef struct changePasswordStruct
{
    GtkWidget *main_window;
    HEADER_BAR_T *header_bar;
    GtkWidget *old_password_entry;
    GtkWidget *password_entry;
    GtkWidget *password_confirmation_entry;
} CHANGE_PASSWORD_T;

typedef struct generatePasswordStruct
{
    GtkWidget *main_window;
    GtkWidget *window;
    char *password_length;
} GENERATE_PASSWORD_T;

/**
 * @brief Create the main window of the application with all its widgets
 * @param app Pointer to GtkApplication variable, defined with
 * gtk_application_new()
 */
void onActivate(GtkApplication *application);

/**
 * @brief Create the account creation window
 * @param button Pointer to the button that triggered the event
 * @param data Pointer to GtkApplication variable
 */
void onCreateAccount(GtkWidget *button, gpointer data);

/**
 * @brief Create the login window
 * @param button Pointer to the button that triggered the event
 * @param data Pointer to GtkApplication variable
 */
void onLogAccount(GtkWidget *button, gpointer data);

/**
 * @brief Create the first window
 * @param button Pointer to the button that triggered the event
 * @param data Pointer to GtkApplication variable
 */
void onLoginMenu(GtkWidget *button, gpointer data);

/**
 * @brief Send the account creation request to the backend
 * @param button Pointer to the button that triggered the event
 * @param data Pointer to GtkApplication variable
 */
void onSendCreatePassword(GtkWidget *button, gpointer data);

/**
 * @brief Send the login request to the backend
 * @param button Pointer to the button that triggered the event
 * @param data Pointer to GtkApplication variable
 */
void onSendLogPassword(GtkWidget *button, gpointer data);

/**
 * @brief Create the main menu window
 * @param main_window Pointer to the main window
 */
void onMainMenu(GtkWidget *main_window);

/**
 * @brief Check when a row is selected in the listbox
 * @param listbox Pointer to the listbox
 * @param row Pointer to the row
 * @param data Pointer to GtkApplication variable
 */
void onSelectedRowChanged(GtkListBox *listbox, GtkListBoxRow *row, gpointer data);

/**
 * @brief Create the window to add a credential
 * @param button Pointer to the button that triggered the event
 * @param data Pointer to GtkApplication variable
 */
void onAddCredential(GtkWidget *button, gpointer data);

/**
 * @brief Display the main menu window
 * @param button Pointer to the button that triggered the event
 * @param data Pointer to GtkApplication variable
 */
void onBackOnMainMenu(GtkWidget *button, gpointer data);

/**
 * @brief Send the credential to the backend
 * @param button Pointer to the button that triggered the event
 * @param data Pointer to GtkApplication variable
 */
void onSendCredential(GtkWidget *button, gpointer data);

/**
 * @brief Export the credentials as a CSV file
 * @param button Pointer to the button that triggered the event
 * @param data Pointer to GtkApplication variable
 */
void onExportCredentialAsCSV(GtkWidget *button, gpointer data);

/**
 * @brief Export the credentials as a PDF file
 * @param button Pointer to the button that triggered the event
 * @param data Pointer to GtkApplication variable
 */
void onExportCredentialAsPDF(GtkWidget *button, gpointer data);

/**
 * @brief Create the window to delete an account
 * @param button Pointer to the button that triggered the event
 * @param data Pointer to GtkApplication variable
 */
void onDeleteAccount(GtkWidget *button, gpointer data);

/**
 * @brief Create the window to confirm the deletion of an account
 * @param button Pointer to the button that triggered the event
 * @param data Pointer to GtkApplication variable
 */
void onDeleteAccountConfirmation(GtkWidget *button, gpointer data);

/**
 * @brief Check the credentials to add credentials
 * @param button Pointer to the button that triggered the event
 * @param data Pointer to GtkApplication variable
 */
void onCheckCredentials(GtkWidget *button, gpointer data);

/**
 * @brief Create the window to delete a credential
 * @param button Pointer to the button that triggered the event
 * @param data Pointer to GtkApplication variable
 */
void onDeleteCredential(GtkWidget *button, gpointer data);

/**
 * @brief Create the window to confirm the deletion of a credential
 * @param button Pointer to the button that triggered the event
 * @param data Pointer to GtkApplication variable
 */
void onDeleteCredentialConfirmation(GtkWidget *button, gpointer data);

/**
 * @brief Create the window to change the password
 * @param button Pointer to the button that triggered the event
 * @param data Pointer to GtkApplication variable
 */
void onChangePassword(GtkWidget *button, gpointer data);

/**
 * @brief Create the window to confirm the change of the password
 * @param button Pointer to the button that triggered the event
 * @param data Pointer to GtkApplication variable
 */
void onChangePasswordConfirmation(GtkWidget *button, gpointer data);

/**
 * @brief Send the change of the password to the backend
 * @param button Pointer to the button that triggered the event
 * @param data Pointer to GtkApplication variable
 */
void onGeneratePasswordClicked(GtkWidget *button, gpointer data);

/**
 * @brief Create the window to generate a password
 * @param button Pointer to the button that triggered the event
 * @param data Pointer to GtkApplication variable
 */
void onGeneratePassword(GtkWidget *button, gpointer data);

/**
 * @brief Create the window to edit a credential
 * @param button Pointer to the button that triggered the event
 * @param data Pointer to GtkApplication variable
 */
void onEditCredential(GtkWidget *button, gpointer data);

/**
 * @brief Check the credentials to edit credentials
 * @param button Pointer to the button that triggered the event
 * @param data Pointer to GtkApplication variable
 */
void onCheckEditCredentials(GtkWidget *button, gpointer data);

#endif
