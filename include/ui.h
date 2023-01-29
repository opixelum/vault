#include <stdio.h>
#include "password.h"
#include "credentials.h"
#include "local_account.h"

/**
 * @brief Clear the stdin buffer.
 * Useful for clearing the buffer before or after a scanf.
 */
void clearStdin();

/**
 * @brief Display the dialogue for creating a new local account.
 */
void localAccountCreationDialogue();

/**
 * @brief Display the dialogue for logging in to a local account.
 */
void localAccountLogInDialogue();

/**
 * @brief Display main menu where user can choose which functionality to run.
 */
void mainMenu(unsigned char *isRunning);

/**
 * @brief Wait for user to press enter. Used to pause the program.
 */
void pressEnterToContinue();

/**
 * @brief Display the dialogue for creating new credentials.
 */
void createCredentialsDialogue();
