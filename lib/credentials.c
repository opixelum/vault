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
        strlen((char *)encrypted_credentials_csv_file_content->ciphertext) - 1,
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
    // Get encrypted credentials
    char * encrypted_credentials_file_path = getEncDecFilePath("credentials");
    FILE * encrypted_credentials_file = fopen(encrypted_credentials_file_path, "rb");
    size_t encrypted_credentials_file_size = getFileSize(encrypted_credentials_file);
    unsigned char * encrypted_credentials_file_content = calloc
    (
        encrypted_credentials_file_size,
        sizeof * encrypted_credentials_file_content
    );
    if (!encrypted_credentials_file_content)
    {
        fprintf(stderr, "ERROR: `encrypted_credentials_file_content` calloc failed.\n");
        exit(EXIT_FAILURE);
    }
    size_t encrypted_credentials_file_elements_read = fread
    (
        encrypted_credentials_file_content,
        sizeof encrypted_credentials_file_content,
        encrypted_credentials_file_size,
        encrypted_credentials_file
    );
    if (encrypted_credentials_file_elements_read != encrypted_credentials_file_size)
    {
        fprintf
        (
            stderr,
            "ERROR: `encrypted_credentials_file` fread failed. Read %ld elements instead of %ld.\n",
            encrypted_credentials_file_elements_read,
            encrypted_credentials_file_size
        );
        exit(EXIT_FAILURE);
    }
    if (fclose(encrypted_credentials_file) == EOF)
    {
        fprintf(stderr, "ERROR: `encrypted_credentials_file` fclose failed.\n");
        exit(EXIT_FAILURE);
    };

    // Get IV
    char * iv_file_path = getEncDecFilePath("iv");
    FILE * iv_file = fopen(iv_file_path, "rb");
    unsigned char * iv = calloc(IV_LENGTH, sizeof * iv);
    if (!iv)
    {
        fprintf(stderr, "ERROR: `iv` calloc failed.\n");
        return -1;
    }
    size_t iv_elements_read = fread(iv, sizeof iv, IV_LENGTH, iv_file);
    if (iv_elements_read != IV_LENGTH)
    {
        fprintf
        (
            stderr,
            "ERROR: `iv_file` fread failed. Read %ld elements instead of %d.\n",
            iv_elements_read,
            IV_LENGTH
        );
        exit(EXIT_FAILURE);
    }
    if (fclose(iv_file) == EOF)
    {
        fprintf(stderr, "ERROR: `iv_file` fclose failed.\n");
        exit(EXIT_FAILURE);
    }

    // Get tag
    char * tag_file_path = getEncDecFilePath("tag");
    FILE * tag_file = fopen(tag_file_path, "rb");
    unsigned char * tag = calloc(TAG_LENGTH, sizeof * tag);
    if (!tag)
    {
        fprintf(stderr, "ERROR: `tag` calloc failed.\n");
        return -1;
    }
    size_t tag_elements_read = fread(tag, sizeof tag, TAG_LENGTH, tag_file);
    if (tag_elements_read != TAG_LENGTH)
    {
        fprintf
        (
            stderr,
            "ERROR: `tag_file` fread failed. Read %ld elements instead of %d.\n",
            tag_elements_read,
            TAG_LENGTH
        );
        exit(EXIT_FAILURE);
    }
    if (fclose(tag_file) == EOF)
    {
        fprintf(stderr, "ERROR: `tag_file` fclose failed.\n");
        exit(EXIT_FAILURE);
    }

    ENCRYPTED_DATA_T encrypted_credentials = {encrypted_credentials_file_content, iv, tag};

    // Decrypt credentials
    char * decrypted_credentials_file_content = (char *) decrypt(&encrypted_credentials, password); 
    printf("Decrypted: %s\n", decrypted_credentials_file_content);

    free(encrypted_credentials_file_content);
    free(iv);
    free(tag);

    return 0;
}
