#include "ui.h"

// TODO: Add dialogue for managing local account (change password, delete account)

unsigned char getCharHide()
{
    int ch;
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return (unsigned char)ch;
}

char *getStringHide()
{
    char *string = calloc(1024, sizeof *string);
    if (!string)
    {
        fprintf(stderr, "Couldn't allocate memory for buffer\n");
        exit(EXIT_FAILURE);
    }
    string[0] = '\0';

    unsigned char c;
    unsigned char string_length = 0;

    do
    {
        c = getCharHide();

        if (c == '\n') break;
        else if (c == 127 || c == 8) // Handle del & backspace
        {
            if (string_length > 0)
            {
                // Delete last character
                string[string_length - 1] = '\0';
                string_length--;

                // Move cursor back and delete last '*'
                printf("\b \b");
            }
        }
        else
        {
            string[string_length] = c;
            string_length++;
            printf("*");
        }
    }
    while (string_length < 1024);

    // Reallocate memory to fit the string
    string = realloc(string, sizeof string * string_length);
    string[string_length] = '\0';

    return string;
}

void clearStdin()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void localAccountCreationDialogue()
{
    printf("\nCreate a local account\n\n");
    char *password = NULL;
    unsigned char is_password_valid = 0;

    // Get password
    set_password:
    printf("Enter a password: ");
    do
    {
        password = getStringHide();

        // Check if password respects the minimum requirements
        if (!minimumPasswordRequirementsCheck(password))
        {
            printf("\nPassword doesn't respect the minimum requirements:\n- Length >= 12\n- At least one uppercase\n- At least one lowercase\n- At least one digit\n- At least one special character.\nPlease enter a valid password: ");
            free(password);
        }
        else is_password_valid = 1;
    }
    while (!is_password_valid);

    // Get password confirmation
    printf("\nConfirm your password: ");
    char *password_confirmation = getStringHide();

    // Check if passwords match
    if (strcmp(password, password_confirmation) != 0)
    {
        free(password);
        free(password_confirmation);

        printf("\nPasswords don't match. Please try again.\n");
        is_password_valid = 0;

        goto set_password;
    }
    free(password_confirmation);

    // Create local account
    if (createLocalAccount(password) == 0) printf("\nLocal account created successfully.\n");
    else fprintf(stderr, "An error occurred while creating the local account.\n");
    free(password);
}

void localAccountLogInDialogue()
{
    printf("\nLog in to your local account\n\n");
    char *password = NULL;
    char local_account_login_status = 0;

    do
    {
        // Get password
        printf("Enter your password: ");
        password = getStringHide();

        // Check if password is correct
        local_account_login_status = connectLocalAccount(password);
        if (local_account_login_status == -1) printf("\nPassword is incorrect.\n");
        else if (local_account_login_status == -2) fprintf(stderr, "An error occured while connecting to the local account.\n");
        free(password);
    }
    while (local_account_login_status != 0);

    
    printf("\nLogged in successfully.\n");
}

void mainMenu(unsigned char *isRunning)
{
    printf("\nPassword Manager\n\n\
    1. New credentials\n\
    2. Update credentials\n\
    3. Delete credentials\n\
    4. View credentials\n\
    5. Manage local account\n\
    6. Quit\n\nEnter your choice: ");

    unsigned char choice;
    scanf("%hhd", &choice);
    clearStdin();

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
            manageLocalAccountMenu();
            break;

        case 6:
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

void manageLocalAccountMenu()
{
    printf("\nManage local account\n\n\
    1. Change local account password\n\
    2. Delete local account\n\
    3. Back\n\nEnter your choice: ");

    unsigned char choice;
    scanf("%hhd", &choice);
    clearStdin();

    switch (choice)
    {
        case 1:
            changeLocalAccountPasswordDialogue();
            break;

        case 2:
            // deleteLocalAccountDialogue();
            break;

        case 3:
            break;

        default:
            printf("Invalid choice");
            break;
    }
}

void changeLocalAccountPasswordDialogue()
{
    printf("\nChange local account password\n\n");

    // Get current password
    printf("Enter your current password: ");
    char *current_password = NULL;
    char is_current_password_correct = 0;

    do
    {
        current_password = getStringHide();

        // Check if password is correct
        is_current_password_correct = connectLocalAccount(current_password);

        if (is_current_password_correct == -1) printf("\nPassword is incorrect.\n");
        else if (is_current_password_correct == -2) fprintf(stderr, "An error occurred while connecting to the local account.\n");

        free(current_password);
    } while (is_current_password_correct != 0);

    // Get new password
    set_new_password:
    printf("\nEnter your new password: ");
    char *new_password = NULL;
    char is_new_password_valid = 0;

    do
    {
        new_password = getStringHide();

        // Check if password respects the minimum requirements
        if (!minimumPasswordRequirementsCheck(new_password))
        {
            printf("\nPassword doesn't respect the minimum requirements:\n- Length >= 12\n- At least one uppercase\n- At least one lowercase\n- At least one digit\n- At least one special character.\nPlease enter a valid password: ");
            free(new_password);
        }
        else is_new_password_valid = 1;
    }
    while (!is_new_password_valid);

    // Get password confirmation
    printf("\nConfirm your new password: ");
    char *new_password_confirmation = getStringHide();

    // Check if passwords match
    if (strcmp(new_password, new_password_confirmation) != 0)
    {
        free(new_password);
        free(new_password_confirmation);

        printf("\nPasswords don't match. Please try again.\n");
        is_new_password_valid = 0;

        goto set_new_password;
    }
    free(new_password_confirmation);

    // Create local account
    if (createLocalAccount(new_password) == 0) printf("\nPassword changed successfully.\n");
    else fprintf(stderr, "Error while changing the local account password\n");
    free(new_password);
}
