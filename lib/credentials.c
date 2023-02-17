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
    free(encrypted_file_path);

    // Remove the temporary file
    remove(temporary_file_path);
    free(temporary_file_path);

    return 0;
}

CREDENTIALS_T * getCredentials(char * label)
{
    // Decrypt the encrypted file to a temporary file
    char * encrypted_file_path = getEncDecFilePath("credentials");
    FILE * encrypted_file = fopen(encrypted_file_path, "rb");
    if (!encrypted_file)
    {
        free(encrypted_file_path);
        return NULL;
    }
    char * temporary_file_path = getEncDecFilePath("temporary");
    FILE * temporary_file = fopen(temporary_file_path, "wb");
    if (!temporary_file)
    {
        free(encrypted_file_path);
        free(temporary_file_path);
        return NULL;
    }
    do_crypt(encrypted_file, temporary_file, 0);
    fclose(encrypted_file);
    fclose(temporary_file);
    free(encrypted_file_path);

    // Open temporary file for reading
    temporary_file = fopen(temporary_file_path, "r");

    // Loop through the temporary file and find the credentials
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    CREDENTIALS_T * credentials = NULL;
    while ((read = getline(&line, &len, temporary_file)) != -1)
    {
        // Remove the newline character
        line[strlen(line) - 1] = '\0';

        // Split the line into tokens
        char * token = strtok(line, ",");
        char * tokens[5];
        int i = 0;
        while (token != NULL)
        {
            tokens[i] = token;
            token = strtok(NULL, ",");
            i++;
        }

        // Check if the label matches
        if (strcmp(tokens[0], label) == 0)
        {
            // Allocate memory for the credentials
            credentials = malloc(sizeof credentials * 5);
            if (!credentials)
            {
                fprintf(stderr, "ERROR: Could not allocate memory for credentials.\n");
                return NULL;
            }

            // Copy the credentials
            credentials->label = malloc(strlen(tokens[0]) + 1);
            strcpy(credentials->label, tokens[0]);
            credentials->url = malloc(strlen(tokens[1]) + 1);
            strcpy(credentials->url, tokens[1]);
            credentials->username = malloc(strlen(tokens[2]) + 1);
            strcpy(credentials->username, tokens[2]);
            credentials->email = malloc(strlen(tokens[3]) + 1);
            strcpy(credentials->email, tokens[3]);
            credentials->password = malloc(strlen(tokens[4]) + 1);
            strcpy(credentials->password, tokens[4]);
            break;
        }
    }
    fclose(temporary_file);

    // Remove the temporary file
    remove(temporary_file_path);
    free(temporary_file_path);

    return credentials;
}
