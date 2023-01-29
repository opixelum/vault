#include "ui.h"

// TODO: Add dialogue for connecting to local account
// TODO: In main menu, add option to manage local account (change password, delete account)
// TODO: Add dialogue for managing local account (change password, delete account)

void clearStdin()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void localAccountCreationDialogue()
{
    printf("\nCreate a local account\n\n");
    char password[256];
    unsigned char is_password_valid = 0;

    // Get password
    set_password:
    printf("Enter a password: ");
    do
    {
        scanf("%s", password);
        clearStdin();

        // Check if password respects the minimum requirements
        if (!minimumPasswordRequirementsCheck(password))
        {
            printf("Password doesn't respect the minimum requirements:\n- Length >= 12\n- At least one uppercase\n- At least one lowercase\n- At least one digit\n- At least one special character.\nPlease enter a valid password: ");
        }
        else is_password_valid = 1;
    }
    while (!is_password_valid);

    // Get password confirmation
    printf("Confirm your password: ");
    char password_confirmation[256];
    scanf("%s", password_confirmation);
    clearStdin();

    // Check if passwords match
    if (strcmp(password, password_confirmation) != 0)
    {
        printf("Passwords don't match. Please try again.\n");
        is_password_valid = 0;
        goto set_password;
    }

    // Create local account
    if (createLocalAccount(password) == 0) printf("Password set successfully.\n");
    else fprintf(stderr, "An error occured while creating the local account.\n");
}

void localAccountLogInDialogue()
{
    printf("\nLog in to your local account\n\n");

    // Check if password is correct
    char local_account_login_status = 0;
    do
    {
        // Get password
        char password[256];
        printf("Enter your password: ");
        scanf("%s", password);
        clearStdin();

        // Check if password is correct
        local_account_login_status = connectLocalAccount(password);
        if (local_account_login_status == -1) printf("Password is incorrect.\n");
        else if (local_account_login_status == -2) fprintf(stderr, "An error occured while connecting to the local account.\n");
    }
    while (local_account_login_status != 0);
    
    printf("Logged in successfully.\n");
}

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

    char *password = malloc(sizeof password * 255);
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
        // Get password
        printf("    Enter the password: ");
        scanf("%s", password);
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
}
