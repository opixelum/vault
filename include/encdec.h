#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/kdf.h>
#include <openssl/rand.h>
#include <string.h>

typedef struct EncryptedDataStruct
{
    unsigned char *ciphertext;
    unsigned char *iv;
    unsigned char *tag;
}
ENCRYPTED_DATA_T;

/**
 * @brief Encrypts the plaintext using AES-256 in GCM mode and a password
 * derived from PBKDF2.
 * 
 * @param plaintext The plaintext to encrypt
 * @param password The password to use for encryption
 * 
 * @return The length of the ciphertext
 */
ENCRYPTED_DATA_T *encrypt
(
    char *plaintext,
    char *password
);

/**
 * @brief Decrypts the ciphertext using AES-256 in GCM mode and a password
 * derived from PBKDF2.
 * 
 * @param ciphertext The ciphertext to decrypt
 * @param ciphertext_len The length of the ciphertext
 * @param password The password used for encryption
 * @param password_len The length of the password
 * @param tag The authentication tag to check integrity
 * @param tag_len The length of the tag
 * @param plaintext The buffer to store the resulting plaintext
 * @return The length of the plaintext or a negative value if the decryption
 * failed
 *
 * @warning It is assumed that the key and IV have been derived from the
 * password using PBKDF2
 */
char *decrypt
(
    ENCRYPTED_DATA_T *encrypted_data,
    char *password
);