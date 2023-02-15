#include "encdec.h"

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

unsigned char *generateRandomIv()
{
    unsigned char *iv = malloc(IV_LENGTH);
    if (!RAND_bytes(iv, IV_LENGTH)) handleErrors();

    return iv;
}

char *getEncDecFilePath(const char *data)
{
    // Get user's system login
    char *user_name = getlogin();
    if (!user_name)
    {
        fprintf(stderr, "ERROR: couldn't get user name for IV file name.\n");
        return NULL;
    }

    // Concatenate user name with data
    char *filename_before_hashing = calloc
    (
        sizeof *filename_before_hashing,
        strlen(user_name) + strlen(data) + 1
    );
    if (!filename_before_hashing)
    {
        fprintf(stderr, "ERROR: `filename_before_hashing` calloc failed.\n");
        return NULL;
    }
    strcat(filename_before_hashing, user_name);
    strcat(filename_before_hashing, data);

    // Hash filename
    unsigned char filename[SHA_DIGEST_LENGTH];
    SHA1((unsigned char *)filename_before_hashing, strlen(filename_before_hashing), filename);
    free(filename_before_hashing);
    char filename_string[SHA_DIGEST_LENGTH * 2 + 1];
    for (unsigned char i = 0; i < SHA_DIGEST_LENGTH; i++)
        sprintf(filename_string + (i * 2), "%02x", filename[i]);
    filename_string[SHA_DIGEST_LENGTH * 2] = '\0';

    // Build file path
    char *file_path = calloc
    (
        sizeof *file_path,
        strlen("data/") + strlen(filename_string) + 1
    );
    if (!file_path)
    {
        fprintf(stderr, "ERROR: `file_path` calloc failed.\n");
        return NULL;
    }
    strcat(file_path, "data/");
    strcat(file_path, filename_string);

    return file_path;
}

ENCRYPTED_DATA_T *encrypt(char *plaintext, char *password)
{
    // Structure that is used to store the state of a cryptographic operation,
    // such as encryption or decryption
    EVP_CIPHER_CTX *ctx;

    // Contains the number of bytes written to the output buffer
    int len;
    int tag_len;

    // Contains the ciphertext, the iv & the tag
    ENCRYPTED_DATA_T *encrypted_data = malloc(sizeof *encrypted_data);
    encrypted_data->iv = generateRandomIv();

    // Write IV to IV file
    char *iv_file_path = getEncDecFilePath("iv");
    FILE *iv_file = fopen(iv_file_path, "wb");
    free(iv_file_path);
    if (!iv_file)
    {
        fprintf(stderr, "Failed to open IV file.\n");
        return NULL;
    }
    fwrite(encrypted_data->iv, sizeof encrypted_data->iv, IV_LENGTH, iv_file);
    fclose(iv_file);
    
    // Create and initialize the context
    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    // Derive the key from the password using PBKDF2
    unsigned char key[32];
    unsigned char salt[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int iterations = 10000;
    int key_len = 32;
    int salt_len = sizeof salt;
    if(1 != PKCS5_PBKDF2_HMAC(password, strlen(password), salt, salt_len, iterations, EVP_sha256(), key_len, key)) handleErrors();

    // Initialize the context of the encryption operation
    // We will set the key & the IV later, in order to separate the encryption
    // initialization from the key and IV management
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL)) handleErrors();

    // Set parameters of the cipher context & configure the behaviour of the
    // cryptographic operation
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 12, NULL)) handleErrors();

    // Set the key and the iv
    if (1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, encrypted_data->iv)) handleErrors();

    // Allocate memory for the ciphertext
    encrypted_data->ciphertext = malloc(strlen(plaintext) + 1);
    if (encrypted_data->ciphertext == NULL) handleErrors();

    // Provide the message to be encrypted & and obtain the encrypted output
    if (1 != EVP_EncryptUpdate(ctx, encrypted_data->ciphertext, &len, (unsigned char *) plaintext, strlen(plaintext))) handleErrors();

    // Flush any remaining data from encryption & append any additional data to
    // the output buffer
    if (1 != EVP_EncryptFinal_ex(ctx, encrypted_data->ciphertext + len, &len)) handleErrors();

    // Add null terminator to the ciphertext
    encrypted_data->ciphertext[strlen(plaintext)] = '\0';

    // Get the tag length
    tag_len = EVP_GCM_TLS_TAG_LEN;

    // Allocate memory for the tag
    encrypted_data->tag = malloc(tag_len);
    if (encrypted_data->tag == NULL) handleErrors();

    // Get the authentication tag
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, encrypted_data->tag)) handleErrors();

    // Write tag to tag file
    char *tag_file_path = getEncDecFilePath("tag");
    FILE *tag_file = fopen(tag_file_path, "wb");
    free(tag_file_path);
    if (!tag_file)
    {
        fprintf(stderr, "Failed to open tag file.\n");
        return NULL;
    }
    fwrite(encrypted_data->tag, sizeof encrypted_data->tag, TAG_LENGTH, tag_file);
    fclose(tag_file);

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    return encrypted_data;
}

unsigned char *decrypt(ENCRYPTED_DATA_T *encrypted_data, char *password)
{
    unsigned char *plaintext;
    int plaintext_len;
    int ciphertext_len = strlen((char *)encrypted_data->ciphertext);

    // Create the structure that is used to store the state of a cryptographic
    // operation, such as encryption or decryption
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    // Derive the key from the password using PBKDF2
    unsigned char key[32];
    unsigned char salt[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int iterations = 10000;
    int key_len = 32;
    int salt_len = sizeof(salt);
    if(1 != PKCS5_PBKDF2_HMAC(password, strlen(password), salt, salt_len, iterations, EVP_sha256(), key_len, key)) handleErrors();

    // Set the cipher type to AES-256-GCM
    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL) != 1) handleErrors();

    // Set the key and IV
    if (EVP_DecryptInit_ex(ctx, NULL, NULL, key, encrypted_data->iv) != 1) handleErrors();

    // Set the tag
    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, encrypted_data->tag) != 1) handleErrors();

    // Allocate memory for the plaintext buffer
    plaintext = (unsigned char *)malloc(ciphertext_len + 1);
    if (!plaintext) handleErrors();

    // Add null terminator to the plaintext
    plaintext[ciphertext_len] = '\0';
    
    // Decrypt the ciphertext
    if (EVP_DecryptUpdate(ctx, plaintext, &plaintext_len, encrypted_data->ciphertext, ciphertext_len) != 1) handleErrors();

    // Flush any remaining data from decryption, check the authenticity of the
    // decrypted data by checking the integrity tag & append any additional
    // data to the output buffer
    if (EVP_DecryptFinal_ex(ctx, plaintext + plaintext_len, &plaintext_len) != 1) handleErrors();

    // Clean up the cipher context
    EVP_CIPHER_CTX_free(ctx);

    return plaintext;
}
