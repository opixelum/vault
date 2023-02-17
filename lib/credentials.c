#include "credentials.h"

char storeCredentials(CREDENTIALS_T credentials) {
    const char *enc_file = "encrypted_credentials";
    const char *tmp_file = "temp_decrypted";

    // Decrypt the encrypted file to a temporary file
    FILE *enc_in = fopen(enc_file, "rb");
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


/*
char storeCredentials(CREDENTIALS_T credentials)
{
    // Create encrypted credentials file if it doesn't exist
    if (access("data", F_OK) == -1) 
    {
        if (mkdir("data", 0700) == -1)
        {
            fprintf(stderr, "ERROR: couldn't create data directory.\n");
            exit(EXIT_FAILURE);
        };
    }
    char * encrypted_credentials_file_path = getEncDecFilePath("credentials");
    FILE * encrypted_credentials_file = NULL;
    if (access(encrypted_credentials_file_path, F_OK) == -1)
    {
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
    FILE * decrypted_credentials_file = fopen("data/temp", "w");
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
            "ERROR: couldn't close `encrypted_credentials_file`.\n"
        );
        exit(EXIT_FAILURE);
    }
    if (fclose(decrypted_credentials_file) == EOF)
    {
        fprintf
        (
            stderr,
            "ERROR: couldn't close `decrypted_credentials_file`.\n"
        );
        exit(EXIT_FAILURE);
    }

    // Retrieve decrypted credentials file content if it isn't empty
    decrypted_credentials_file = fopen("data/temp", "r");
    size_t decrypted_credentials_file_size = getFileSize(decrypted_credentials_file);
    printf("decrypted_credentials_file_size: %ld\n", decrypted_credentials_file_size);
    char * decrypted_credentials_file_content = NULL;
    if (decrypted_credentials_file_size > 0) 
    {
        decrypted_credentials_file_content = calloc(decrypted_credentials_file_size, 1);
        if (!decrypted_credentials_file_content)
        {
            fprintf
            (
                stderr,
                "ERROR: `decrypted_credentials_file_content` calloc failed.\n"
            );
            exit(EXIT_FAILURE);
        }
        size_t decrypted_credentials_file_content_elements_read = fread
        (
            decrypted_credentials_file_content,
            1,
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
        }
    }
    if (fclose(decrypted_credentials_file) == EOF)
    {
        fprintf
        (
            stderr,
            "ERROR: couldn't close `decrypted_credentials_file`.\n"
        );
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < decrypted_credentials_file_size; i++)
        printf("%c", decrypted_credentials_file_content[i]);
    free(decrypted_credentials_file_content);
    printf("\n");

    // Write new credentials
    // Add header if file is empty or if header doesn't exist in it
    char * credentials_csv_header = "label,url,username,email,password";
    decrypted_credentials_file = fopen("data/temp", "a");
    if (decrypted_credentials_file_size == 0)
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

    // Encrypt credentials file
    encrypted_credentials_file = fopen(encrypted_credentials_file_path, "ab+");
    free(encrypted_credentials_file_path);
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
    rewind(decrypted_credentials_file);
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

/*
    if (remove("data/temp") == -1)
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
*/
