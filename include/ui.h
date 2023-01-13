#include <stdio.h>
#include "password.h"
#include "credentials.h"
#include "store.h"

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
