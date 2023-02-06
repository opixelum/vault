#include "ui.h"

unsigned char getCharHide()
{
    int character;

    /*
     * Termios is a struct that contains terminal attributes
     * Terminal attributes used here are:
     * 
     * - ICANON: Terminal control flag for canonical mode
     *   If set, input is processed line by line,
     *   If not, input is processed character by character
     * 
     * - ECHO: Terminal control flag for echo
     *   If set, input is displayed on the terminal
     *   If not, input is not displayed on the terminal
     */
    struct termios old_terminal_attributes, new_terminal_attributes;

    // Get terminal attributes & store in old_terminal_attributes
    tcgetattr(STDIN_FILENO, &old_terminal_attributes);

    // Copy old_terminal_attributes to new_terminal_attributes for modification
    new_terminal_attributes = old_terminal_attributes;

    // Turn off canonical & echo mode in new_terminal_attributes
    // It does bitwise operations on flags
    new_terminal_attributes.c_lflag &= ~(ICANON | ECHO);

    // Set new terminal attributes
    tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal_attributes);

    // Get character from user input
    character = getchar();

    // Set terminal attributes back to old ones
    tcsetattr(STDIN_FILENO, TCSANOW, &old_terminal_attributes);

    return (unsigned char)character;
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

    unsigned char character;
    unsigned char string_length = 0;

    do
    {
        character = getCharHide();

        if (character == '\n') break;
        else if (character == 127 || character == 8) // Handle del & backspace
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
            string[string_length] = character;
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
    else fprintf(stderr, "\nError while creating local account.\n");
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
        else if (local_account_login_status == -2) fprintf(stderr, "\nError while connecting to local account.\n");
        free(password);
    }
    while (local_account_login_status != 0);

    
    printf("\nLogged in successfully.\n");
}

void mainMenu(unsigned char *isRunning, unsigned char *isConnected)
{
    printf("\nPassword Manager\n\n\
    1. New credentials\n\
    2. Update credentials\n\
    3. Delete credentials\n\
    4. View credentials\n\
    5. Export credentials\n\
    6. Manage local account\n\
    7. Quit\n\nEnter your choice: ");

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
            exportCredentials();
            break;
           
        case 6:
            manageLocalAccountMenu(isRunning, isConnected);
            break;

        case 7:
            *isRunning = 0;
            *isConnected = 0;
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

    CREDENTIALS_T credentials = {
        label,
        url,
        username,
        email,
        password
    };

    storeCredentials(credentials);
}

void manageLocalAccountMenu(unsigned char *isRunning, unsigned char *isConnected)
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
            deleteLocalAccountDialogue(isRunning, isConnected);
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

    char is_current_password_correct = 0;

    // Get current password
    do
    {
        printf("Enter your current password: ");
        char *current_password = NULL;
        current_password = getStringHide();

        // Check if password is correct
        is_current_password_correct = connectLocalAccount(current_password);
        if (is_current_password_correct == -1) printf("\nPassword is incorrect.\n");
        else if (is_current_password_correct == -2) fprintf(stderr, "\nError while connecting to local account.\n");

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
    else fprintf(stderr, "\nError while changing local account password\n");
    free(new_password);
}

void deleteLocalAccountDialogue(unsigned char *isRunning, unsigned char *isConnected)
{
    printf("\nDelete local account\n\nPLEASE NOTE THAT THIS ACTION WILL DELETE ALL YOUR CREDENTIALS AND IS IRREVERSIBLE.\n\n");

    char is_current_password_correct = 0;

    // Get current password
    do
    {
        printf("Enter your current password: ");
        char *current_password = NULL;
        current_password = getStringHide();

        // Check if password is correct
        is_current_password_correct = connectLocalAccount(current_password);
        if (is_current_password_correct == -1) printf("\nPassword is incorrect.\n");
        else if (is_current_password_correct == -2) fprintf(stderr, "\nErro while connecting to local account.\n");

        free(current_password);
    } while (is_current_password_correct != 0);

    // Delete credentials
    if (remove("credentials.csv") != 0) fprintf(stderr, "\nCouldn't delete credentials file\n");
    else printf("\nCredentials deleted successfully.\n");

    // Delete local account
    if (remove("local_account") != 0) fprintf(stderr, "Couldn't delete local account file\n");
    else printf("Local account deleted successfully.\n");

    // Stop program
    *isConnected = 0;
    *isRunning = 0;
}
