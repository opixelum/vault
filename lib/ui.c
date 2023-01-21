#include "ui.h"

void mainMenu(unsigned char *isRunning)
{
    printf("\nPassword Manager\n\n\
    1. New credentials\n\
    2. Update credentials\n\
    3. Delete credentials\n\
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

    // Get url
    printf("    Enter the url (optional): ");
    char url[100];
    scanf("%s", url);

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
    char choice[100];
    scanf("%s", choice);

    char *password;
    if (strcmp(choice, "y") == 0)
    {
        // Get password length
        printf("    Enter the length of the password: ");
        unsigned char pswdLength;
        scanf("%hhd", &pswdLength);

        // Generate password
        password = genPswd(pswdLength, 0);
    }
    else if (strcmp(choice, "n") == 0)
    {
        password = malloc(sizeof password * 255);
        if (!password)
        {
            fprintf(stderr, "Couldn't allocate memory for password\n");
            exit(1);
        }

        // Get password
        printf("    Enter the password: ");
        scanf("%s", password);

        // Reallocate memory to fit password
        password = realloc(password, sizeof password * strlen(password) + 1);
        if (!password)
        {
            fprintf(stderr, "Couldn't reallocate memory for password\n");
            exit(1);
        }

        // Add null-terminator
        *(password + strlen(password)) = '\0';
    }

    // Print credentials
    printf("\n\
    Label: %s\n\
    url: %s\n\
    Username: %s\n\
    Email: %s\n\
    Password: %s\n",
    label, url, username, email, password);

    Credentials_t credentials = {
        label,
        url,
        username,
        email,
        password
    };

    storeCredentials(credentials);

    // Free memory
    free(password);
}
