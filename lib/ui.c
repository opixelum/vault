#include "../include/ui.h"

void displayMainMenu()
{
    printf("Password Manager\n\n\
    1. New credentials\n\
    3. Update credentials\n\
    2. Delete credentials\n\
    4. View credentials\n\
    5. Exit\n\nEnter your choice: ");
}

void pressEnterToContinue()
{
    printf("Press [ENTER] to continue...");
    getchar();
}
