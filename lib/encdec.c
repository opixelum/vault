#include "encdec.h"

// TODO: Check comments
// TODO: Fix memory leaks

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

unsigned char *generateRandomIv()
{
    unsigned char *iv = malloc(12);
    if (!RAND_bytes(iv, 12)) handleErrors();

    return iv;
}

ENCRYPTED_DATA_T *encrypt(char *plaintext, char *password)
{
    EVP_CIPHER_CTX *ctx;
    int len;

    // Create the variable to store the encrypted data
    ENCRYPTED_DATA_T *encrypted_data = malloc(sizeof encrypted_data);

    // Create the variable to store the ciphertext, the iv & the tag
    encrypted_data->iv = generateRandomIv();
    encrypted_data->tag = malloc(sizeof encrypted_data->tag);
    encrypted_data->ciphertext = malloc(strlen(plaintext));

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    /* Derive the key from the password using PBKDF2 */
    unsigned char key[32];
    unsigned char salt[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int iterations = 10000;
    int key_len = 32;
    int salt_len = sizeof salt;
    if(1 != PKCS5_PBKDF2_HMAC(password, strlen(password), salt, salt_len, iterations, EVP_sha256(), key_len, key)) handleErrors();

    /* Initialise the encryption operation */
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL)) handleErrors();
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 12, NULL)) handleErrors();
    if (1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, encrypted_data->iv)) handleErrors();

    /* Provide the message to be encrypted, and obtain the encrypted output */
    if (1 != EVP_EncryptUpdate(ctx, encrypted_data->ciphertext, &len, (unsigned char *) plaintext, strlen(plaintext))) handleErrors();

    /* Finalise the encryption */
    if (1 != EVP_EncryptFinal_ex(ctx, encrypted_data->ciphertext + len, &len)) handleErrors();

    /* Get the authentication tag */
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, encrypted_data->tag)) handleErrors();

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return encrypted_data;
}

char *decrypt
(
    ENCRYPTED_DATA_T *encrypted_data,
    char *password
)
{
    EVP_CIPHER_CTX *ctx;
    int len;

    // Create the variable to store the plaintext
    char *plaintext = malloc(strlen((char *) encrypted_data->ciphertext));

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    /* Derive the key from the password using PBKDF2 */
    unsigned char key[32];
    unsigned char salt[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int iterations = 10000;
    int key_len = 32;
    int salt_len = sizeof(salt);
    if(1 != PKCS5_PBKDF2_HMAC(password, strlen(password), salt, salt_len, iterations, EVP_sha256(), key_len, key)) handleErrors();

    /* Initialise the decryption operation */
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL)) handleErrors();
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 12, NULL)) handleErrors();
    if (1 != EVP_DecryptInit_ex(ctx, NULL, NULL, key, encrypted_data->iv)) handleErrors();

    /* Set the tag */
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, sizeof encrypted_data->tag, encrypted_data->tag)) handleErrors();

    /* Provide the message to be decrypted, and obtain the decrypted output */
    if (1 != EVP_DecryptUpdate(ctx, (unsigned char *) plaintext, &len, encrypted_data->ciphertext, strlen((char *) encrypted_data->ciphertext))) handleErrors();

    /* Finalize the decryption */
    int decryption_result = EVP_DecryptFinal_ex(ctx, (unsigned char *) plaintext + len, &len);
    if (decryption_result < 0) handleErrors();

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext;
}
