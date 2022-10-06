#include "password.h"

/**
 * Checks minimal password security requirements:
 *  - 8 characters long
 *  - Digits
 *  - Lowercase
 *  - Uppercase
 *  - Symbols
 * 
 * Uses ASCII table for checks.
 */
int pswdCheck(char *pswd, size_t len)
{
    int has_uppercase, has_lowercase, has_digits, has_symbols, is_too_short;

    for (int i = 0; i < len; i++)
    {
        // Length check
        if (len > 8) is_too_short = 1;

        // Digits check
        if (pswd[i] >= '0' && pswd[i] <= '9') has_digits = 1;

        // Lowercase check
        else if (pswd[i] >= 'a' && pswd[i] <= 'z') has_lowercase = 1;

        // Uppercase check
        else if (pswd[i] >= 'A' && pswd[i] <= 'Z') has_uppercase = 1;

        // Symbols check
        else if
        (
            pswd[i] >= 33 && pswd[i] < 48 || pswd[i] >= 58 && pswd[i] < 65
            || pswd[i] >= 91 && pswd[i] < 97 || pswd[i] >= 123 && pswd[i] < 127
        )
            has_symbols = 1;
    }

    if (!has_digits || !has_lowercase || !has_uppercase || !has_symbols)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
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
char *genPswd(size_t len)
{
    char *pswd = malloc(sizeof pswd * len);

    // Initialize random seed
    srand(time(NULL));

    for (int i = 0; i < len; i++)
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

    return pswd;
}
