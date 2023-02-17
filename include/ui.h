#include <stdio.h>
#include "password.h"
#include "credentials.h"
#include "local_account.h"
#include <termios.h>
#include <unistd.h>
#include "export.h"
#include "gui.h"

/**
 * @brief Get a character from stdin without displaying it.
 * @return The character read from stdin.
 */
unsigned char getCharHide();

/**
 * @brief Get a string from stdin without displaying it.
 * @return The string read from stdin.
*/
char *getStringHide();

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
void mainMenu(unsigned char *isRunning, unsigned char *isConnected);

/**
 * @brief Wait for user to press enter. Used to pause the program.
 */
void pressEnterToContinue();

/**
 * @brief Display the dialogue for creating new credentials.
 */
void createCredentialsDialogue();

/**
 * @brief Display the dialogue for managing local account.
 */
void manageLocalAccountMenu();

/**
 * @brief Display the dialogue for changing the password of the local account.
 */
void changeLocalAccountPasswordDialogue();

/**
 * @brief Display the dialogue for deleting the local account.
 */
void deleteLocalAccountDialogue(unsigned char *isRunning, unsigned char *isConnected);
