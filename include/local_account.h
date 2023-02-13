#include <openssl/sha.h>
#include <stdio.h>
#include <unistd.h>
#include "password.h"

#define SALT_LENGTH 255

/**
 * @brief Check if a local account exists
 * It checks if the local account file exists and if it is not empty. If one of
 * these conditions is not met, it means that no local account exists. 
 * @return Returns 1 if a local account exists, 0 otherwise
 */
char isLocalAccountExists();

/**
 * @brief Create a local account.
 * 
 * The password hash is stored in the local account file.
 * If an account already exists, it is overwritten.
 * 
 * @param password The password to use to create the local account
 * 
 * @return 0 on success, -1 if password is not strong enough, -2 for other
 * errors
 */
char createLocalAccount(char *password);

/**
 * @brief Connect to the local account in order to use the password manager
 * @param password The password to use to connect to the local account
 * @return 0 on success, -1 if wrong password, -2 for other errors
 */
char connectLocalAccount(char *password);

/**
 * @brief Delete the local account with all its credentials
 * @param password The password to use to delete the local account
 * @return 0 on success, -1 if wrong password, -2 for other errors
 */
char deleteLocalAccount(char *password);
