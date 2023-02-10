#include "local_account.h"

unsigned char isLocalAccountExists()
{
    FILE *local_account_file = fopen("local_account", "r");

    // Check if local account file exists
    if (!local_account_file)
        return 0;

    // Check if local account file is empty
    else if (feof(local_account_file))
        return 0;

    fclose(local_account_file);
    return 1;
}

char createLocalAccount(char *password, char *confirmation_password)
{
    // Check if password is strong enough
    if (!minimumPasswordRequirementsCheck(password))
    {
        fprintf(stderr, "Password doesn't meet minimum requirements.\n");
        return -1;
    }

    // Check if password and confirmation password match
    if (strcmp(password, confirmation_password) != 0)
    {
        fprintf(stderr, "Confirmation password is different.\n");
        return -2;
    }

    // Generate random salt
    char *salt = malloc(sizeof *salt * SALT_LENGTH + 1); // +1 for null terminator
    if (!salt)
    {
        fprintf(stderr, "Memory allocation for salt failed.\n");
        return -3;
    }
    srand(time(NULL));
    for (int i = 0; i < SALT_LENGTH; i++)
        salt[i] = 33 + rand() % 94;
    salt[SALT_LENGTH] = '\0'; // Add null terminator

    // Salt password
    char *salted_password = malloc(sizeof *salted_password * (strlen(password) + SALT_LENGTH) + 1);
    if (!salted_password)
    {
        fprintf(stderr, "Memory reallocation for password failed.\n");
        return -3;
    }
    salted_password[0] = '\0';
    strcat(salted_password, password);
    strcat(salted_password, salt);

    // Hash password
    unsigned char *password_hash = malloc(sizeof password_hash * SHA512_DIGEST_LENGTH);
    if (!password_hash)
    {
        fprintf(stderr, "Memory allocation for password hash failed.\n");
        return -3;
    }
    SHA512((unsigned char *)salted_password, strlen(salted_password), password_hash);
    free(salted_password);

    // Convert password hash to hex string
    char *password_hash_hex = malloc(sizeof *password_hash_hex * SHA512_DIGEST_LENGTH * 2 + 1);
    if (!password_hash_hex)
    {
        fprintf(stderr, "Memory allocation for password hash hex failed.\n");
        return -3;
    }
    for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
        sprintf(password_hash_hex + (i * 2), "%02x", password_hash[i]);
    free(password_hash);
    password_hash_hex[SHA512_DIGEST_LENGTH * 2] = '\0'; // Add null terminator

    // Open local account file
    FILE *local_account_file = fopen("local_account", "w");
    if (!local_account_file)
    {
        fprintf(stderr, "Failed to open local account file.\n");
        return -3;
    }

    // Write salt & password hash hex to local account file on different lines
    fprintf(local_account_file, "%s\n", salt);
    free(salt);
    fprintf(local_account_file, "%s\n", password_hash_hex);
    free(password_hash_hex);

    // Close local account file
    if (fclose(local_account_file) != 0)
    {
        fprintf(stderr, "Failed to close local account file.\n");
        return -3;
    }

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
    char *salt = malloc(sizeof *salt * SALT_LENGTH);
    if (!salt)
    {
        fprintf(stderr, "Memory allocation for salt failed.\n");
        return -2;
    }
    size_t salt_length = SALT_LENGTH;
    getline(&salt, &salt_length, local_account_file);
    salt[strlen(salt) - 1] = '\0'; // Remove newline

    // Read password hash from local account file
    char *password_hash_from_file_hex = malloc(sizeof *password_hash_from_file_hex * SHA512_DIGEST_LENGTH);
    if (!password_hash_from_file_hex)
    {
        fprintf(stderr, "Memory allocation for password hash failed.\n");
        return -2;
    }
    size_t password_hash_from_file_hex_length = SHA512_DIGEST_LENGTH;
    getline(&password_hash_from_file_hex, &password_hash_from_file_hex_length, local_account_file);
    password_hash_from_file_hex[strlen(password_hash_from_file_hex) - 1] = '\0'; // Remove newline

    // Close local account file
    if (fclose(local_account_file) != 0)
    {
        fprintf(stderr, "Failed to close local account file.\n");
        return -2;
    }

    // Salt password
    char *salted_password = malloc(sizeof *salted_password * (strlen(password) + SALT_LENGTH) + 1);
    if (!salted_password)
    {
        fprintf(stderr, "Memory reallocation for password failed.\n");
        return -2;
    }
    salted_password[0] = '\0';
    strcat(salted_password, password);
    strcat(salted_password, salt);
    free(salt);

    // Hash password with salt
    unsigned char *password_hash_from_user = malloc(sizeof *password_hash_from_user * SHA512_DIGEST_LENGTH);
    if (!password_hash_from_user)
    {
        fprintf(stderr, "Memory allocation for password hash failed.\n");
        return -2;
    }
    SHA512((unsigned char *)salted_password, strlen(salted_password), password_hash_from_user);
    free(salted_password);

    // Convert password hash to hex string
    char *password_hash_from_user_hex = malloc(sizeof *password_hash_from_user_hex * SHA512_DIGEST_LENGTH * 2 + 1);
    if (!password_hash_from_user_hex)
    {
        fprintf(stderr, "Memory allocation for password hash hex failed.\n");
        return -2;
    }
    for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
        sprintf(password_hash_from_user_hex + (i * 2), "%02x", password_hash_from_user[i]);
    free(password_hash_from_user);
    password_hash_from_user_hex[SHA512_DIGEST_LENGTH * 2] = '\0'; // Add null terminator

    // Compare password hashes
    if (strcmp(password_hash_from_file_hex, password_hash_from_user_hex) == 0)
    {
        free(password_hash_from_file_hex);
        free(password_hash_from_user_hex);
        return 0;
    }
    else
    {
        free(password_hash_from_file_hex);
        free(password_hash_from_user_hex);
        return -1;
    }
}

char deleteLocalAccount(char *password)
{
    char connect_local_account_result = connectLocalAccount(password);
    if (connect_local_account_result == 0)
    {
        // Delete local account file
        if (remove("local_account") != 0)
        {
            fprintf(stderr, "Failed to delete local account file.\n");
            return -2;
        }

        return 0;
    }
    else
        return connect_local_account_result;
}
