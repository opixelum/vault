#include "local_account.h"

char createLocalAccount(char *password)
{
    /*
    // Check if password is strong enough
    if (minimumPasswordRequirementsCheck(password) == 1)
    {
        printf("Password doesn't meet minimum requirements.\n");
        return -1;
    }
*/
    // Generate random salt
    printf("Generating random salt... ");
    char *salt = malloc(sizeof *salt * SALT_LENGTH + 1);
    if (!salt)
    {
        fprintf(stderr, "Memory allocation for salt failed.\n");
        return -2;
    }
    srand(time(NULL));
    for (int i = 0; i < SALT_LENGTH; i++) salt[i] = 33 + rand() % 94;

    // Add null terminator to salt
    salt[SALT_LENGTH] = '\0';
    printf("Done.\nSalt: %s\n", salt);

    // Reallocate password memory to concatenate salt
    printf("Concatenating salt to password... ");
    password = realloc(password, sizeof *password * (strlen(password) + SALT_LENGTH) + 1);
    if (!password)
    {
        fprintf(stderr, "Memory reallocation for password failed.\n");
        return -2;
    }
    strcat(password, salt);
    printf("Done.\nPassword with hash: %s\n", password);

    // Hash password
    printf("Hashing password... ");
    unsigned char *password_hash = malloc(sizeof *password_hash * SHA512_DIGEST_LENGTH);
    if (!password_hash)
    {
        fprintf(stderr, "Memory allocation for password hash failed.\n");
        return -2;
    }
    SHA512((unsigned char *)password, strlen(password), password_hash);
    printf("Done.\nPassword hash: ");
    for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) printf("%02x", password_hash[i]);
    printf("\n");

    // Open local account file
    FILE *local_account_file = fopen("local_account", "w");
    if (!local_account_file)
    {
        fprintf(stderr, "Failed to open local account file.\n");
        return -2;
    }

    // Write salt to local account file 
    if (fwrite(salt, sizeof *salt, SALT_LENGTH, local_account_file) != SALT_LENGTH)
    {
        fprintf(stderr, "Failed to write salt to local account file.\n");
        return -2;
    }

    // Separate salt and password hash with newline
    putc('\n', local_account_file);

    // Write password hash to local account file
    if (fwrite(password_hash, sizeof *password_hash, SHA512_DIGEST_LENGTH, local_account_file) != SHA512_DIGEST_LENGTH)
    {
        fprintf(stderr, "Failed to write password hash to local account file.\n");
        return -2;
    }

    // Close local account file
    if (fclose(local_account_file) != 0)
    {
        fprintf(stderr, "Failed to close local account file.\n");
        return -2;
    }

    // Free memory
    free(salt);
    free(password_hash);
    free(password);

    return 0;
}
