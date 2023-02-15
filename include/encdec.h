#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/kdf.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <string.h>
#include "local_account.h"

#define IV_LENGTH 12
#define TAG_LENGTH 16

typedef struct EncryptedDataStruct
{
    // Encrypted text.
    unsigned char *ciphertext;

    // Initialization vector, used to make the ciphertext different each time
    // the same plain text is encrypted.
    unsigned char *iv;

    // Authentication tag, used to verify the ciphertext integrity.
    unsigned char *tag;
}
ENCRYPTED_DATA_T;

/**
 * @brief Prints the OpenSSL errors to stderr and aborts the program.
 */
void handleErrors(void);

/**
 * @brief Generates a random initialization vector.
 * 
 * The initialization vector is used to make the ciphertext different each time
 * the same plain text is encrypted. It is also used to decrypt the ciphertext.
 * In AES-256 in GCM mode (our case), the IV must be 12 bytes long.
 * 
 * @warning The returned pointer must be freed.
 * 
 * @return A pointer to the generated IV.
 */
unsigned char *generateRandomIv();

/**
 * @brief Builds the file path for encrypted/decrypted related data such as the
 * IV, the tag & credentials.
 * @param data A string of wanted data, such as "iv", "tag" or "credentials".
 * @return A pointer to the built file path.
 * @warning The returned pointer must be freed.
 */
char *getEncDecFilePath(const char *data);

/**
 * @brief Encrypts the plaintext using AES-256 in GCM mode and a password.
 * 
 * @param plaintext The plaintext to encrypt
 * @param password The password to use for encryption
 * 
 * @return A structure containing the ciphertext, the initialization vector
 * and the tag.
 */
ENCRYPTED_DATA_T *encrypt(char *plaintext, char *password);

/**
 * @brief Decrypts the ciphertext using AES-256 in GCM mode and a password.
 * 
 * @param encrypted_data The structure containing the ciphertext, the
 * initialization vector and the tag.
 * @param password The password used for encryption.
 * 
 * @warning It is assumed that the key and IV have been derived from the
 * password using PBKDF2.
 */
unsigned char *decrypt(ENCRYPTED_DATA_T *encrypted_data, char *password);
