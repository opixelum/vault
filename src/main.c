#include "ui.h"
#include "local_account.h"
#include <unistd.h>

int main(int argc, char const *argv[])
{
    unsigned char isRunning = 0;

    while (isRunning)
    {
        mainMenu(&isRunning);
    }

    // Check if a local account exists
    if (isLocalAccountExists()) printf("Local account exists.\n");
    else printf("No local account.\n");

    // Generate a password
    char *password = genPswd(20, 1);
    sleep(1);
    char *password2 = genPswd(20, 1);

    // Create a local account
    createLocalAccount(password);

    // Connect to the local account
    if (connectLocalAccount(password) == 0) printf("Connected to local account.\n");
    else printf("Wrong password.\n");

    // Delete the local account
    if (deleteLocalAccount(password2) == 0) printf("Local account deleted.\n");
    else printf("Wrong password.\n");

    // Check if a local account exists
    if (isLocalAccountExists()) printf("Local account exists.\n");
    else printf("No local account.\n");

    free(password);
    free(password2);

    return 0;
}
