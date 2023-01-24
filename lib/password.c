#include "password.h"
#include <unistd.h>

/**
 * @return 1 if string respects length, 0 if not
 */
int hasLength(size_t len, int expected_len)
{
    if (len < expected_len)
    {
        printf("Too short\n");
        return 0;
    }
    else if (len > expected_len)
    {
        printf("Too long\n");
        return 0;
    }
    return 1;
}

/**
 * @return 1 if string has a digit, 0 if not
 */
int hasDigit(char *str, size_t len)
{
    for (int i = 0; i < len; ++i)
        if (str[i] >= '0' && str[i] <= '9')
            return 1;
    printf("No digit\n");
    return 0;
}

/**
 * @return 1 if string has an uppercase character, 0 if not
 */
int hasUppercase(char *str, size_t len)
{
    for (int i = 0; i < len; ++i)
        if (str[i] >= 'A' && str[i] <= 'Z')
            return 1;
    printf("No uppercase\n");
    return 0;
}

/**
 * @return 1 if string has a lowercase character, 0 if not
 */
int hasLowercase(char *str, size_t len)
{
    for (int i = 0; i < len; ++i)
        if (str[i] >= 'a' && str[i] <= 'z')
            return 1;
    printf("No lowercase\n");
    return 0;
}

/**
 * @return 1 if string has a special character , 0 if not
 */
int hasSpecialChar(char *str, size_t len)
{
    for (int i = 0; i < len; ++i)
    {
        if (
            (str[i] >= '!' && str[i] <= '/') || (str[i] >= ':' && str[i] <= '@') || (str[i] >= '[' && str[i] <= '`') || (str[i] >= '{' && str[i] <= '~'))
            return 1;
    }

    printf("No special character\n");

    return 0;
}

char minimumPasswordRequirementsCheck(char *password)
{
    return pswdCheck(password, strlen(password), 12, "SLUD") ? 1 : 0;
}

/**
 * @param char_requirements String containing the initials of the wanted characters type.
 * [S]pecial characters - [L]owercase - [U]ppercase - [D]igit
 * @return 1 if password satisfies given requirements, 0 if not
 */
int pswdCheck(char *pswd, size_t len, int expected_len, char *char_requirements)
{
    unsigned char has_special_char = 1;
    unsigned char has_lowercase = 1;
    unsigned char has_uppercase = 1;
    unsigned char has_digit = 1;
    unsigned char has_length = hasLength(len, expected_len);

    for (int i = strlen(char_requirements); --i;)
    {
        switch (char_requirements[i])
        {
        case 'S':
            has_special_char = hasSpecialChar(pswd, len);
            break;

        case 'L':
            has_lowercase = hasLowercase(pswd, len);
            break;

        case 'U':
            has_uppercase = hasUppercase(pswd, len);
            break;

        case 'D':
            has_digit = hasDigit(pswd, len);
            break;
        }
    }

    return has_length * has_special_char * has_lowercase * has_uppercase * has_digit;
}

/**
 * @brief Generates a password that includes:
 *  - Digits;
 *  - Lowercases;
 *  - Uppercases;
 *  - Symbols.
 *
 * @return a pointer to the generated password.
 */
char *genPswd(size_t len, int counter)
{
    char *pswd = malloc(sizeof pswd * len);

    // Initialize random seed
    srand(time(NULL) + counter);

    for (int i = 0; i < len; ++i)
    {
        switch (rand() % 4)
        {
        // Add a digit
        case 0:
            *(pswd + i) = rand() % 10 + 48;
            break;

        // Add a lowercase
        case 1:
            *(pswd + i) = rand() % 26 + 65;
            break;

        // Add an uppercase
        case 2:
            *(pswd + i) = rand() % 26 + 97;
            break;

        // Add a symbol
        case 3:
            *(pswd + i) = SYMBOLS[rand() % strlen(SYMBOLS)];
            break;
        }
    }

    // Regenerate password if it doens't meet requirements
    if (!pswdCheck(pswd, strlen(pswd), len, "SLUD"))
    {
        printf("Regenerating password...\n");
        strcpy(pswd, genPswd(len, ++counter));
    }

    return pswd;
}
