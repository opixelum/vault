#include <openssl/sha.h>
#include <stdio.h>
#include "password.h"

/**
 * @brief Hash a string using SHA512.
 * @param string The string to hash .
 * @return The hash of the string.
 * @warning The returned hash must be freed after use.
 */
unsigned char *sha512Hash(const char *string);

/**
 * @brief Compare two hashes using SHA512.
 * @return 0 if hashes are equal, a non-zero value otherwise.
 */
int sha512HashCompare(const unsigned char *hash1, const unsigned char *hash2);

/**
 * @brief Create a local account.
 * 
 * The password hash is stored in the local account file.
 * If an account already exists, it is overwritten.
 * 
 * @param password The password to use to create the local account
 * 
 * @return 1 on success, -1 if password is not strong enough, -2 for other
 * errors
 */
char createLocalAccount(char *password);

/**
 * @brief Connect to the local account in order to use the password manager
 * @param password The password to use to connect to the local account
 * @return 1 on success, -1 if wrong password, -2 for other errors
 */
char connectLocalAccount(char *password);

/**
 * @brief Delete the local account with all its credentials
 * @param password The password to use to delete the local account
 * @return 1 on success, -1 if wrong password, -2 for other errors
 */
char removeLocalAccount(char *password);
