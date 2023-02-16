#include "credentials.h"

char storeCredentials(CREDENTIALS_T credentials)
{
    // Create encrypted credentials file if it doesn't exist
    char * encrypted_credentials_file_path = getEncDecFilePath("credentials");
    FILE * encrypted_credentials_file = NULL;
    if (access(encrypted_credentials_file_path, F_OK) == -1) 
    {
        if (mkdir("data", 0700) == -1)
        {
            fprintf(stderr, "ERROR: couldn't create data directory.\n");
            exit(EXIT_FAILURE);
        };
        encrypted_credentials_file = fopen
        (
            encrypted_credentials_file_path,
            "w"
        );
        if (!encrypted_credentials_file)
        {
            fprintf
            (
                stderr,
                "ERROR: couldn't create encrypted credentials file: %s.\n",
                strerror(errno)
            );
            exit(EXIT_FAILURE);
        }
        if (fclose(encrypted_credentials_file) == EOF)
        {
            fprintf
            (
                stderr,
                "ERROR: couldn't close encrypted_credentials_file.\n"
            );
            exit(EXIT_FAILURE);
        }
    }

    /* If it exists, retrieve content, decrypt it & add credentials
    // If it doesn't exist, create it, add header & add credentials

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

    // Store encrypted credentials CSV file content
    char * encrypted_credentials_csv_file_path = getEncDecFilePath("credentials");
    FILE * encrypted_credentials_file = fopen(encrypted_credentials_csv_file_path, "wb");
    fwrite
    (
        encrypted_credentials_csv_file_content->ciphertext,
        sizeof encrypted_credentials_csv_file_content->ciphertext,
        strlen((char *)encrypted_credentials_csv_file_content->ciphertext) - 1,
        encrypted_credentials_file
    );
    free(encrypted_credentials_csv_file_content->ciphertext);
    free(encrypted_credentials_csv_file_content);
    free(encrypted_credentials_csv_file_path);
    fclose(encrypted_credentials_file);
    */
    return 0;
}
