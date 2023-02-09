#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>

/**
 * @brief Create the main window of the application with all its widgets
 * @param app Pointer to GtkApplication variable, defined with
 * gtk_application_new()
 */
void onActivate(GtkApplication *application);

#endif
