#include "password.h"
#include "ui.h"

int main(int argc, char const *argv[])
{
    unsigned char isRunning = 1;

    while (isRunning)
    {
        mainMenu(&isRunning);
    }

    return 0;
}
