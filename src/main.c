#include "ui.h"
#include "encdec.h"

int main(int argc, char const *argv[])
{
    /*
    unsigned char isRunning = 1;
    unsigned char isConnected = 0;

    while (isRunning)
    {
        if (!isConnected)
        {
            isLocalAccountExists()
            ? localAccountLogInDialogue()
            : localAccountCreationDialogue();
            isConnected = 1;
        }
        mainMenu(&isRunning, &isConnected);
    }
    */

    FILE *credentials_csv_file = fopen("credentials.csv", "r");
    if (!credentials_csv_file)
    {
        fprintf(stderr, "Error: Couldn't open credentials csv file.");
        return EXIT_FAILURE;
    }

    // Get size of credentials csv file
    size_t credentials_csv_file_size = 0;
    // Got to EOF
    fseek(credentials_csv_file, 0L, SEEK_END); 
    // Get current position of file pointer (this tell us the size)
    credentials_csv_file_size = ftell(credentials_csv_file);
    // Set pointer back to beginning of file
    rewind(credentials_csv_file);

    // Read content
    char *credentials_csv_file_content = calloc
    (
        sizeof *credentials_csv_file_content, credentials_csv_file_size + 1
    );
    for (unsigned int i = 0; i < credentials_csv_file_size; i++)
    {
        *(credentials_csv_file_content + i) = fgetc(credentials_csv_file);
        printf("%c", *(credentials_csv_file_content + i));
    }
    if (fclose(credentials_csv_file) != 0)
    {
        fprintf(stderr, "Error: couldn't close credentials csv file.");
        return EXIT_FAILURE;
    }
    credentials_csv_file_content[credentials_csv_file_size] = '\0';

    char *password = "password";
    char *wrong_password = "mljdfslmkj";

    // Encrypt credentials csv file content
    ENCRYPTED_DATA_T *encrypted_credentials = encrypt(credentials_csv_file_content, password);

    // Write encrypted content to credentials csv file
    credentials_csv_file = fopen("credentials.csv", "wb");
    fwrite(encrypted_credentials->ciphertext, sizeof *encrypted_credentials->ciphertext, credentials_csv_file_size, credentials_csv_file);
    if (fclose(credentials_csv_file) != 0)
    {
        fprintf(stderr, "Error: couldn't close credentials csv file.");
        return EXIT_FAILURE;
    }

    // Decrypt credentials csv file content
    FILE *encrypted_credentials_csv_file = fopen("credentials.csv", "rb");

    // Get size of credentials csv file
    credentials_csv_file_size = 0;
    // Got to EOF
    fseek(encrypted_credentials_csv_file, 0L, SEEK_END);
    // Get current position of file pointer (this tell us the size)
    credentials_csv_file_size = ftell(encrypted_credentials_csv_file);
    // Set pointer back to beginning of file
    rewind(encrypted_credentials_csv_file);

    // Get IV
    unsigned char *iv = calloc(sizeof *iv, IV_LENGTH);
    char *iv_filepath = getEncDecFilePath("iv");
    FILE *iv_file = fopen(iv_filepath, "rb");
    free(iv_filepath);
    if (!iv_file)
    {
        fprintf(stderr, "Error: Couldn't open iv file.");
        return EXIT_FAILURE;
    }
    fread(iv, sizeof *iv, IV_LENGTH, iv_file);
    fclose(iv_file);

    // Get tag
    unsigned char *tag = calloc(sizeof *tag, TAG_LENGTH);
    char *tag_filepath = getEncDecFilePath("tag");
    FILE *tag_file = fopen(tag_filepath, "rb");
    free(tag_filepath);
    if (!tag_file)
    {
        fprintf(stderr, "Error: Couldn't open tag file.");
        return EXIT_FAILURE;
    }
    fread(tag, sizeof *tag, TAG_LENGTH, tag_file);
    fclose(tag_file);

    // Read content
    unsigned char *encrypted_credentials_csv_file_content = calloc
    (
        sizeof *encrypted_credentials_csv_file_content, credentials_csv_file_size
    );
    fread
    (
        encrypted_credentials_csv_file_content,
        sizeof encrypted_credentials_csv_file_content,
        credentials_csv_file_size,
        encrypted_credentials_csv_file
    );
    if (fclose(encrypted_credentials_csv_file) != 0)
    {
        fprintf(stderr, "Error: couldn't close credentials csv file.");
        return EXIT_FAILURE;
    }
    ENCRYPTED_DATA_T encrypted_credentials_from_csv = {encrypted_credentials_csv_file_content, iv, tag};

    char *decrypted_credentials_csv_file_content = (char *)decrypt(&encrypted_credentials_from_csv, password);

    // Write decrypted content to credentials csv file
    credentials_csv_file = fopen("credentials.csv", "w");
    if (!credentials_csv_file)
    {
        fprintf(stderr, "Error: Couldn't open credentials csv file.");
        return EXIT_FAILURE;
    }   
    for (unsigned int i = 0; i < credentials_csv_file_size; i++)
    {
        fputc(*(decrypted_credentials_csv_file_content + i), credentials_csv_file);
        printf("%c", *(decrypted_credentials_csv_file_content + i));
    }
    if (fclose(credentials_csv_file) != 0)
    {
        fprintf(stderr, "Error: couldn't close credentials csv file.");
        return EXIT_FAILURE;
    }

    return 0;
}
