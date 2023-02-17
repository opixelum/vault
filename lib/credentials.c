#include "credentials.h"

char storeCredentials(CREDENTIALS_T credentials) {
    // Check if data directory exists
    if (access("data", F_OK) == -1)
    {
        if (mkdir("data", 0700) == -1)
        {
            fprintf(stderr, "ERROR: Could not create data directory.\n");
            return -1;
        }
    }
    
    const char *enc_file = getEncDecFilePath("credentials");
    const char *tmp_file = getEncDecFilePath("temporary");

    // Decrypt the encrypted file to a temporary file
    FILE *enc_in = fopen(enc_file, "rb");
    // If the encrypted file does not exist, create it
    if (!enc_in)
    {
        enc_in = fopen(enc_file, "wb");
        fclose(enc_in);
        enc_in = fopen(enc_file, "rb");
    }
    FILE *tmp_out = fopen(tmp_file, "wb");
    do_crypt(enc_in, tmp_out, 0);
    fclose(enc_in);
    fclose(tmp_out);

    // Open temporary file for appending
    FILE *tmp_in = fopen(tmp_file, "a");

    // Write header to temporary file if it is empty
    long file_size;
    fseek(tmp_in, 0, SEEK_END);
    file_size = ftell(tmp_in);
    if (file_size == 0)
        fprintf(tmp_in, "label,url,username,email,password\n");

    // Write new credentials to temporary file
    fprintf(tmp_in, "%s,%s,%s,%s,%s\n", credentials.label, credentials.url,
            credentials.username, credentials.email, credentials.password);
    fclose(tmp_in);

    // Encrypt the temporary file and overwrite the encrypted file
    FILE *enc_out = fopen(enc_file, "wb");
    FILE *tmp_in_enc = fopen(tmp_file, "rb");
    do_crypt(tmp_in_enc, enc_out, 1);
    fclose(enc_out);
    fclose(tmp_in_enc);

    // Remove the temporary file
    remove(tmp_file);
    return 0;
}
