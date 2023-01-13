#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/kdf.h>
#include <openssl/rand.h>
#include <string.h>

/**
 * @brief Encrypts the plaintext using AES-256 in GCM mode and a password
 * derived from PBKDF2.
 * 
 * @param plaintext The plaintext to encrypt
 * @param plaintext_len The length of the plaintext
 * @param password The password to use for encryption
 * @param password_len The length of the password
 * @param ciphertext The buffer to store the resulting ciphertext
 * @param tag The buffer to store the resulting authentication tag
 * 
 * @return The length of the ciphertext
 */
int encrypt
(
    unsigned char *plaintext,
    unsigned int plaintext_len,
    char *password,
    unsigned int password_len,
    unsigned char *ciphertext,
    unsigned char *tag,
    unsigned char *iv
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
int decrypt
(
    unsigned char *ciphertext,
    int ciphertext_len,
    char *password,
    int password_len,
    unsigned char *tag,
    int tag_len,
    unsigned char *plaintext,
    unsigned char *iv
);