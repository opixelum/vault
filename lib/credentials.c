#include "credentials.h"

char storeFirstCredentials(CREDENTIALS_T credentials, char * password)
{
    // Create data folder if it doesn't exist
    if (access("data", F_OK) == -1) mkdir("data", 0700);

    // Format credentials in CSV
    char * credentials_csv_header = "label,url,username,email,password\n";
    char * credentials_csv_row = calloc
    (
        strlen(credentials.label) +
        strlen(credentials.url) +
        strlen(credentials.username) +
        strlen(credentials.email) +
        strlen(credentials.password) +
        6, // 4 commas + 1 newline + 1 null terminator
        sizeof * credentials_csv_row
    );
    if (!credentials_csv_row)
    {
        fprintf(stderr, "ERROR: `credentials_csv_row` calloc failed.\n");
        return -1;
    }
    sprintf
    (
        credentials_csv_row,
        "%s,%s,%s,%s,%s\n",
        credentials.label,
        credentials.url,
        credentials.username,
        credentials.email,
        credentials.password
    );
    credentials_csv_row[strlen(credentials_csv_row)] = '\0';
    char * credentials_csv_file_content = calloc
    (
        strlen(credentials_csv_header) + strlen(credentials_csv_row) + 1,
        sizeof * credentials_csv_file_content
    );
    if (!credentials_csv_file_content)
    {
        fprintf(stderr, "ERROR: `credentials_csv_file_content` calloc failed.\n");
        return -1;
    }
    strcpy(credentials_csv_file_content, credentials_csv_header);
    strcat(credentials_csv_file_content, credentials_csv_row);
    free(credentials_csv_row);

    // Encrypt credentials CSV file content
    ENCRYPTED_DATA_T * encrypted_credentials_csv_file_content = encrypt
    (
        credentials_csv_file_content,
        password
    );

    // Store IV
    char * iv_file_path = getEncDecFilePath("iv");
    FILE * iv_file = fopen(iv_file_path, "wb");
    fwrite
    (
        encrypted_credentials_csv_file_content->iv,
        sizeof encrypted_credentials_csv_file_content->iv,
        IV_LENGTH,
        iv_file
    );
    free(encrypted_credentials_csv_file_content->iv);
    free(iv_file_path);
    fclose(iv_file);

    // Store tag
    char * tag_file_path = getEncDecFilePath("tag");
    FILE * tag_file = fopen(tag_file_path, "wb");
    fwrite
    (
        encrypted_credentials_csv_file_content->tag,
        sizeof encrypted_credentials_csv_file_content->tag,
        TAG_LENGTH,
        tag_file
    );
    free(encrypted_credentials_csv_file_content->tag);
    free(tag_file_path);
    fclose(tag_file);

    // Store encrypted credentials CSV file content
    char * encrypted_credentials_csv_file_path = getEncDecFilePath("credentials");
    FILE * encrypted_credentials_file = fopen(encrypted_credentials_csv_file_path, "wb");
    fwrite
    (
        encrypted_credentials_csv_file_content->ciphertext,
        sizeof encrypted_credentials_csv_file_content->ciphertext,
        strlen((char *)encrypted_credentials_csv_file_content->ciphertext),
        encrypted_credentials_file
    );
    free(encrypted_credentials_csv_file_content->ciphertext);
    free(encrypted_credentials_csv_file_content);
    free(encrypted_credentials_csv_file_path);
    fclose(encrypted_credentials_file);

    return 0;
}

char storeCredentials(CREDENTIALS_T credentials, char * password)
{
    FILE *encrypted_credentials_file = NULL;

    // Get encrypted credentials file path
    char *encrypted_credentials_file_path = getEncDecFilePath("credentials");

    // Check if encrypted credentials file exists
    if (access(encrypted_credentials_file_path, F_OK) != -1)
    {
        // Create data folder if it doesn't exist
        if (access("data", F_OK) == -1) mkdir("data", 0700);

        // Create encrypted credentials file
        encrypted_credentials_file = fopen(encrypted_credentials_file_path, "w");
        fclose(encrypted_credentials_file);
    }

    // Open encrypted credentials file
    encrypted_credentials_file = fopen(encrypted_credentials_file_path, "rb");

    // Get encrypted credentials file size
    size_t encrypted_credentials_file_size = getFileSize(encrypted_credentials_file);

    // Get encrypted credentials file content
    unsigned char *encrypted_credentials_file_content = calloc(encrypted_credentials_file_size, sizeof *encrypted_credentials_file_content);

    return 0;
}
