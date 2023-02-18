#include "encdec.h"

char * getEncDecFilePath(const char * data)
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
        strlen("data/") + strlen(filename_string) + 1,
        sizeof *file_path
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

int do_crypt(FILE *in, FILE *out, int do_encrypt)
{
    /* Allow enough space in output buffer for additional block */
    unsigned char inbuf[1024], outbuf[1024 + EVP_MAX_BLOCK_LENGTH];
    int inlen, outlen;
    EVP_CIPHER_CTX *ctx;
    /*
     * Bogus key and IV: we'd normally set these from
     * another source.
     */
    unsigned char key[] = "0123456789abcdeF";
    unsigned char iv[] = "1234567887654321";

    /* Don't set key or IV right away; we want to check lengths */
    ctx = EVP_CIPHER_CTX_new();
    if (!EVP_CipherInit_ex2(ctx, EVP_aes_128_cbc(), NULL, NULL,
                            do_encrypt, NULL)) {
        /* Error */
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }
    OPENSSL_assert(EVP_CIPHER_CTX_get_key_length(ctx) == 16);
    OPENSSL_assert(EVP_CIPHER_CTX_get_iv_length(ctx) == 16);

    /* Now we can set key and IV */
    if (!EVP_CipherInit_ex2(ctx, NULL, key, iv, do_encrypt, NULL)) {
        /* Error */
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    for (;;) {
        inlen = fread(inbuf, 1, 1024, in);
        if (inlen <= 0)
            break;
        if (!EVP_CipherUpdate(ctx, outbuf, &outlen, inbuf, inlen)) {
            /* Error */
            EVP_CIPHER_CTX_free(ctx);
            return 0;
        }
        fwrite(outbuf, 1, outlen, out);
    }
    if (!EVP_CipherFinal_ex(ctx, outbuf, &outlen)) {
        /* Error */
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }
    fwrite(outbuf, 1, outlen, out);

    EVP_CIPHER_CTX_free(ctx);
    return 1;
}
