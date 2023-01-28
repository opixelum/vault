#include "ui.h"
#include "local_account.h"

int main(int argc, char const *argv[])
{
    unsigned char isRunning = 0;

    while (isRunning)
    {
        mainMenu(&isRunning);
    }

    // Generate a password
    char *password = genPswd(20, 1);

    // Create a local account
    createLocalAccount(password);

    // Connect to the local account
    if (connectLocalAccount(password) == 0) printf("Connected to local account.\n");
    else printf("Wrong password.\n");

    free(password);

    return 0;
}
