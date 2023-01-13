#include "../include/ui.h"

void mainMenu(unsigned char *isRunning)
{
    printf("\nPassword Manager\n\n\
    1. New credentials\n\
    3. Update credentials\n\
    2. Delete credentials\n\
    4. View credentials\n\
    5. Quit\n\nEnter your choice: ");

    unsigned char choice;
    scanf("%hhd", &choice);

    switch (choice)
    {
        case 1:
            createCredentialsDialogue();
            break;

        case 2:
            printf("2");
            break;

        case 3:
            printf("3");
            break;

        case 4:
            printf("4");
            break;

        case 5:
            *isRunning = 0;
            break;

        default:
            printf("Invalid choice");
            break;
    }
}

void pressEnterToContinue()
{
    printf("Press [ENTER] to continue...");
    getchar();
}

void createCredentialsDialogue()
{
    // Get label
    printf("\nCreate new credentials\n\n\
    Enter a label for the credentials: ");
    char label[100];
    scanf("%s", label);

    // Get username
    printf("    Enter the username (optional): ");
    char username[100];
    scanf("%s", username);

    // Get email address
    printf("    Enter the email address (optional): ");
    char email[100];
    scanf("%s", email);

    // Ask if user wants to generate a password
    printf("    Do you want to generate a password? (y/n): ");
    char generatePswd[100];
    scanf("%s", generatePswd);

    char *password = malloc(sizeof password * 255);
    if (strcmp(generatePswd, "y") == 0)
    {
        // Get password length
        printf("    Enter the length of the password: ");
        unsigned char pswdLength;
        scanf("%hhd", &pswdLength);

        // Generate password
        password = genPswd(pswdLength, 0);
    }
    else if (strcmp(generatePswd, "n") == 0)
    {
        // Get password
        printf("    Enter the password: ");
        scanf("%s", password);
    }

    // Print credentials
    printf("\n\
    Label: %s\n\
    Username: %s\n\
    Email: %s\n\
    Password: %s\n\n",
    label, username, email, password);
}
