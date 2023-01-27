#include "ui.h"
#include "local_account.h"

int main(int argc, char const *argv[])
{
    unsigned char isRunning = 0;

    while (isRunning)
    {
        mainMenu(&isRunning);
    }

    // Generate a random password
    char *password = genPswd(16, 1);

    // Print the password
    printf("Generated password: ");
    for (int i = 0; i < 16; i++) printf("%c", password[i]);
    printf("\n");

    if (createLocalAccount(password) != 0)
    {
        printf("Failed to create local account.\n");
        return -1;
    }

    return 0;
}
