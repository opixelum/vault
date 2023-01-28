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
    char *salt = malloc(sizeof *salt * SALT_LENGTH);
    if (!salt)
    {
        fprintf(stderr, "Memory allocation for salt failed.\n");
        return -2;
    }
    srand(time(NULL));
    for (int i = 0; i < SALT_LENGTH; i++) salt[i] = 33 + rand() % 94;

    // Reallocate password memory to concatenate salt
    char *salted_password = malloc(sizeof *salted_password * (strlen(password) + SALT_LENGTH) + 1);
    if (!salted_password)
    {
        fprintf(stderr, "Memory reallocation for password failed.\n");
        return -2;
    }
    salted_password[0] = '\0';
    strcat(salted_password, password);
    strcat(salted_password, salt);

    // Hash password
    unsigned char *password_hash = malloc(sizeof password_hash * SHA512_DIGEST_LENGTH);
    if (!password_hash)
    {
        fprintf(stderr, "Memory allocation for password hash failed.\n");
        return -2;
    }
    SHA512((unsigned char *)salted_password, strlen(salted_password), password_hash);

    // Open local account file
    FILE *local_account_file = fopen("local_account", "wb");
    if (!local_account_file)
    {
        fprintf(stderr, "Failed to open local account file.\n");
        return -2;
    }

    // Write salt & password hash to local account file 
    fwrite(salt, sizeof salt, SALT_LENGTH, local_account_file);
    fwrite(password_hash, sizeof password_hash, SHA512_DIGEST_LENGTH, local_account_file);

    // Close local account file
    if (fclose(local_account_file) != 0)
    {
        fprintf(stderr, "Failed to close local account file.\n");
        return -2;
    }

    // Free memory
    free(salt);
    free(salted_password);
    free(password_hash);

    return 0;
}

char connectLocalAccount(char *password)
{
    // Open local account file
    FILE *local_account_file = fopen("local_account", "rb");
    if (!local_account_file)
    {
        fprintf(stderr, "Failed to open local account file.\n");
        return -2;
    }

    // Read salt from local account file
    char *salt = malloc(sizeof *salt * SALT_LENGTH);
    if (!salt)
    {
        fprintf(stderr, "Memory allocation for salt failed.\n");
        return -2;
    }
    fread(salt, sizeof salt, SALT_LENGTH, local_account_file);
    printf("Salt: %s\n", salt);

    // Read password hash from local account file
    unsigned char *password_hash_from_file = malloc(sizeof *password_hash_from_file * SHA512_DIGEST_LENGTH);
    if (!password_hash_from_file)
    {
        fprintf(stderr, "Memory allocation for password hash failed.\n");
        return -2;
    }
    fread(password_hash_from_file, sizeof password_hash_from_file, SHA512_DIGEST_LENGTH, local_account_file);
    printf("Password: %s\n", password_hash_from_file);

    // Close local account file
    if (fclose(local_account_file) != 0)
    {
        fprintf(stderr, "Failed to close local account file.\n");
        return -2;
    }

    // Reallocate password memory to concatenate salt
    char *salted_password = malloc(sizeof *salted_password * (strlen(password) + SALT_LENGTH) + 1);
    if (!salted_password)
    {
        fprintf(stderr, "Memory reallocation for password failed.\n");
        return -2;
    }
    salted_password[0] = '\0';
    strcat(salted_password, password);
    strcat(salted_password, salt);

    // Hash password with salt
    unsigned char * password_hash_from_user = malloc(sizeof *password_hash_from_user * SHA512_DIGEST_LENGTH);
    if (!password_hash_from_user)
    {
        fprintf(stderr, "Memory allocation for password hash failed.\n");
        return -2;
    }
    SHA512((unsigned char *)salted_password, strlen(salted_password), password_hash_from_user);
    printf("Password from user: %s\n", password_hash_from_user);

    // Add null terminator to password hash
    password_hash_from_user[SHA512_DIGEST_LENGTH] = '\0';

    // Compare password hashes
    if (memcmp(password_hash_from_file, password_hash_from_user, SHA512_DIGEST_LENGTH) == 0)
    {
        printf("Password hashes match.\n");
        return 0;
    }
    else
    {
        printf("Password hashes don't match.\n");
        return -1;
    }
}
