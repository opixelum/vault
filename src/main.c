#include "ui.h"
#include <unistd.h>

// TODO: On launch, check if there is a local account. If yes, ask for password, if no, ask for password creation

int main(int argc, char const *argv[])
{
    unsigned char isRunning = 1;

    while (isRunning)
    {
        isLocalAccountExists()
        ? localAccountLogInDialogue()
        : localAccountCreationDialogue();
        mainMenu(&isRunning);
    }

    return 0;
}
