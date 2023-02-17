#include "credentials.h"

char storeCredentials(CREDENTIALS_T credentials)
{
    // Check if data directory exists
    if (access("data", F_OK) == -1)
    {
        if (mkdir("data", 0700) == -1)
        {
            fprintf(stderr, "ERROR: Could not create data directory.\n");
            return -1;
        }
    }
    
    char * encrypted_file_path = getEncDecFilePath("credentials");
    char * temporary_file_path = getEncDecFilePath("temporary");

    // Decrypt the encrypted file to a temporary file
    FILE *encrypted_file = fopen(encrypted_file_path, "rb");
    // If the encrypted file does not exist, create it
    if (!encrypted_file)
    {
        encrypted_file = fopen(encrypted_file_path, "wb");
        fclose(encrypted_file);
        encrypted_file = fopen(encrypted_file_path, "rb");
    }
    FILE * temporary_file = fopen(temporary_file_path, "wb");
    do_crypt(encrypted_file, temporary_file, 0);
    fclose(encrypted_file);
    fclose(temporary_file);

    // Open temporary file for appending
    temporary_file = fopen(temporary_file_path, "a");

    // Write header to temporary file if it is empty
    long file_size;
    fseek(temporary_file, 0, SEEK_END);
    file_size = ftell(temporary_file);
    if (file_size == 0)
        fprintf(temporary_file, "label,url,username,email,password\n");

    // Write new credentials to temporary file
    fprintf
    (
        temporary_file,
        "%s,%s,%s,%s,%s\n",
        credentials.label,
        credentials.url,
        credentials.username,
        credentials.email,
        credentials.password
    );
    fclose(temporary_file);

    // Encrypt the temporary file and overwrite the encrypted file
    encrypted_file = fopen(encrypted_file_path, "wb");
    temporary_file = fopen(temporary_file_path, "rb");
    do_crypt(temporary_file, encrypted_file, 1);
    fclose(encrypted_file);
    fclose(temporary_file);

    // Remove the temporary file
    remove(temporary_file_path);
    free(encrypted_file_path);

    return 0;
}
