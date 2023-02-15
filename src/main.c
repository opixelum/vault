#include "ui.h"
#include "encdec.h"

int main(int argc, char *argv[])
{
    // Create a new application
    GtkApplication *application = gtk_application_new(
        "com.example.GtkApplication",
        G_APPLICATION_FLAGS_NONE);
    g_signal_connect(application, "activate", G_CALLBACK(onActivate), NULL);

    return g_application_run(G_APPLICATION(application), argc, argv);
}
