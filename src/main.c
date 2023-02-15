#include "ui.h"
#include "encdec.h"

int main(int argc, char const *argv[])
{
    unsigned char isRunning = 1;
    unsigned char isConnected = 0;

    while (isRunning)
    {
        if (!isConnected)
        {
            isLocalAccountExists()
            ? localAccountLogInDialogue()
            : localAccountCreationDialogue();
            isConnected = 1;
        }
        mainMenu(&isRunning, &isConnected);
    }

    return 0;
}
