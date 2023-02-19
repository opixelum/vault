#include <openssl/sha.h>
#include <stdio.h>
#include <unistd.h>
#include "password.h"
#include <sys/stat.h>
#include "encdec.h"

#define SALT_LENGTH 255

/**
 * @brief Get the local account file path
 * @warning The returned string must be freed
 * @return Local account file path string or NULL on error
 */
char *getLocalAccountFilePath();

/**
 * @brief Check if a local account exists
 * It checks if the local account file exists and if it is not empty. If one of
 * these conditions is not met, it means that no local account exists. 
 * @return Returns 1 if a local account exists, 0 otherwise
 */
char isLocalAccountExists();

/**
 * @brief Generate a random salt with a length of 255.
 * @warning The returned string must be freed.
 * @return The random generated salt string.
 */
char *generateRandomSalt();

/**
 * @brief Hash a password with a salt using SHA-512
 * @param password A string.
 * @param salt A string. If NULL, a random salt will be generated.
 * @warning The returned string must be freed.
 * @return The hashed password in hexadecimal with salt or NULL on error.
 */
unsigned char *hashPasswordWithSalt(char *password, char *salt);

/**
 * @brief Create a local account.
 * 
 * The password hash is stored in the local account file.
 * If an account already exists, it is overwritten.
 * 
 * @param password The password to use to create the local account
 * 
 * @return 0 on success, -1 if password is not strong enough, -2 if
 * confirmation password is different, -3 for other errors
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
