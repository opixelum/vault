#include "encdec.h"

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

unsigned char *generateRandomIV()
{
    unsigned char *iv = malloc(12);
    if (!RAND_bytes(iv, 12)) handleErrors();
    return iv;
}

int encrypt(unsigned char *plaintext, unsigned int plaintext_len, char *password, unsigned int password_len,
  unsigned char *ciphertext, unsigned char *tag, unsigned char *iv) {

    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    /* Derive the key from the password using PBKDF2 */
    unsigned char key[32];
    unsigned char salt[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int iterations = 10000;
    int key_len = 32;
    int salt_len = sizeof(salt);
    if(1 != PKCS5_PBKDF2_HMAC(password, password_len, salt, salt_len, iterations, EVP_sha256(), key_len, key)) handleErrors();

    /* Initialise the encryption operation */
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL)) handleErrors();
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 12, NULL)) handleErrors();
    if (1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv)) handleErrors();

    /* Provide the message to be encrypted, and obtain the encrypted output */
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) handleErrors();
    ciphertext_len = len;

    /* Finalise the encryption */
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    /* Get the authentication tag */
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag)) handleErrors();

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return ciphertext_len;
}

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
)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    /* Derive the key from the password using PBKDF2 */
    unsigned char key[32];
    unsigned char salt[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int iterations = 10000;
    int key_len = 32;
    int salt_len = sizeof(salt);
    if(1 != PKCS5_PBKDF2_HMAC(password, password_len, salt, salt_len, iterations, EVP_sha256(), key_len, key)) handleErrors();

    /* Initialise the decryption operation */
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL)) handleErrors();
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 12, NULL)) handleErrors();
    if (1 != EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv)) handleErrors();

    /* Set the tag */
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, tag_len, tag)) handleErrors();

    /* Provide the message to be decrypted, and obtain the decrypted output */
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) handleErrors();
    plaintext_len = len;

    /* Finalise the decryption */
    int decryption_result = EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
    if (decryption_result < 0) handleErrors();
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);
    if (decryption_result > 0) {
        return plaintext_len;
    } else {
        return -1;
    }
}
