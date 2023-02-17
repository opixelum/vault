#include "credentials.h"

char storeCredentials(CREDENTIALS_T credentials)
{
    // Format credentials in CSV
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

    // Create encrypted credentials file if it doesn't exist
    char * encrypted_credentials_file_path = getEncDecFilePath("credentials");
    if (access("data", F_OK) == -1) 
    {
        if (mkdir("data", 0700) == -1)
        {
            fprintf(stderr, "ERROR: couldn't create data directory.\n");
            exit(EXIT_FAILURE);
        };
    }
    FILE * encrypted_credentials_file = fopen
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

    // Decrypt credentials file
    encrypted_credentials_file = fopen(encrypted_credentials_file_path, "rb");
    if (!encrypted_credentials_file)
    {
        fprintf
        (
            stderr,
            "ERROR: couldn't open encrypted credentials file: %s.\n",
            strerror(errno)
        );
        exit(EXIT_FAILURE);
    }
    FILE * decrypted_credentials_file = fopen("data/temp", "a+");
    if (!decrypted_credentials_file)
    {
        fprintf
        (
            stderr,
            "ERROR: couldn't create decrypted credentials file: %s.\n",
            strerror(errno)
        );
        exit(EXIT_FAILURE);
    }
    do_crypt(encrypted_credentials_file, decrypted_credentials_file, 0);
    if (fclose(encrypted_credentials_file) == EOF)
    {
        fprintf
        (
            stderr,
            "ERROR: couldn't close encrypted_credentials_file.\n"
        );
        exit(EXIT_FAILURE);
    }
    rewind(decrypted_credentials_file);

    // Retrieve decrypted credentials file content if it isn't empty
    size_t decrypted_credentials_file_size = getFileSize(decrypted_credentials_file);
    char * decrypted_credentials_file_content = NULL;
    if (decrypted_credentials_file_size > 0) 
    {
        decrypted_credentials_file_content = calloc
        (
            decrypted_credentials_file_size,
            sizeof * decrypted_credentials_file_content
        );
        if (!decrypted_credentials_file_content)
        {
            fprintf
            (
                stderr,
                "ERROR: `decrypted_credentials_file_content` calloc failed.\n"
            );
            exit(EXIT_FAILURE);
        }
    rewind(decrypted_credentials_file);
        size_t decrypted_credentials_file_content_elements_read = fread
        (
            decrypted_credentials_file_content,
            sizeof decrypted_credentials_file_content,
            decrypted_credentials_file_size,
            decrypted_credentials_file
        );
        if (decrypted_credentials_file_content_elements_read != decrypted_credentials_file_size)
        {
            fprintf
            (
                stderr,
                "ERROR: `decrypted_credentials_file` fread failed. Read %ld elements read instead of %ld.\n",
                decrypted_credentials_file_content_elements_read,
                decrypted_credentials_file_size
            );
            exit(EXIT_FAILURE);
        }
    }

    // Write new credentials
    // Add header if file is empty or if header doesn't exist in it
    char * credentials_csv_header = "label,url,username,email,password";
    if
    (
        decrypted_credentials_file_size == 0
        || !strstr(decrypted_credentials_file_content, credentials_csv_header)
    )
        fprintf(decrypted_credentials_file, "%s\n", credentials_csv_header);
    fprintf
    (
        decrypted_credentials_file,
        "%s,%s,%s,%s,%s\n",
        credentials.label,
        credentials.url,
        credentials.username,
        credentials.email,
        credentials.password
    );
    rewind(decrypted_credentials_file);

    // Encrypt credentials file
    encrypted_credentials_file = fopen(encrypted_credentials_file_path, "wb");
    if (!encrypted_credentials_file)
    {
        fprintf
        (
            stderr,
            "ERROR: couldn't open encrypted credentials file: %s.\n",
            strerror(errno)
        );
        exit(EXIT_FAILURE);
    }
    do_crypt(decrypted_credentials_file, encrypted_credentials_file, 1);
    if (fclose(encrypted_credentials_file) == EOF)
    {
        fprintf
        (
            stderr,
            "ERROR: couldn't close encrypted_credentials_file.\n"
        );
        exit(EXIT_FAILURE);
    }
    if (fclose(decrypted_credentials_file) == EOF)
    {
        fprintf
        (
            stderr,
            "ERROR: couldn't close decrypted_credentials_file.\n"
        );
        exit(EXIT_FAILURE);
    }

    // Remove decrypted credentials file
    if (remove("data/temp") != 0)
    {
        fprintf
        (
            stderr,
            "ERROR: couldn't remove decrypted credentials file: %s.\n",
            strerror(errno)
        );
        exit(EXIT_FAILURE);
    }

    return 0;
}
