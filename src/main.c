#include "ui.h"
#include <unistd.h>

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
