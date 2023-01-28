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
    char *salt = malloc(sizeof *salt * SALT_LENGTH + 1);
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
    FILE *local_account_file = fopen("local_account", "w");
    if (!local_account_file)
    {
        fprintf(stderr, "Failed to open local account file.\n");
        return -2;
    }

    // Write salt to local account file 
    fprintf(local_account_file, "%s\n", salt);

    // Write password hash to local account file
    for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
    {
        fprintf(local_account_file, "%02x", password_hash[i]);
    }
    fprintf(local_account_file, "\n");

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
    FILE *local_account_file = fopen("local_account", "r");
    if (!local_account_file)
    {
        fprintf(stderr, "Failed to open local account file.\n");
        return -2;
    }

    // Read salt from local account file
    char *salt = malloc(sizeof *salt * SALT_LENGTH + 1);
    size_t saltlen = 255;
    if (!salt)
    {
        fprintf(stderr, "Memory allocation for salt failed.\n");
        return -2;
    }
    getline(&salt, &saltlen, local_account_file);

    // Add null terminator to salt
    salt[SALT_LENGTH] = '\0';

    printf("\nSalt: %s\n", salt);

    // Read password hash from local account file
    unsigned char *password_hash_from_file = malloc(sizeof *password_hash_from_file * SHA512_DIGEST_LENGTH);
    size_t sha512len = 128;
    if (!password_hash_from_file)
    {
        fprintf(stderr, "Memory allocation for password hash failed.\n");
        return -2;
    }
    getline((char **)&password_hash_from_file, &sha512len, local_account_file);

    // Add null terminator to password hash
    password_hash_from_file[sha512len] = '\0';

    printf("\nPassword hash from file: %s\n", password_hash_from_file);

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

    // Add null terminator to password hash
    password_hash_from_user[sha512len] = '\0';

    for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
    {
        printf("%02x", password_hash_from_user[i]);
    }

    // Compare password hashes
    if (strcmp((char *)password_hash_from_file, (char *)password_hash_from_user) == 0)
    {
        printf("\nPassword hashes match.\n");
        return 0;
    }
    else
    {
        printf("\nPassword hashes don't match.\n");
        return -1;
    }
}
