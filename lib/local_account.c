#include "local_account.h"

char *getLocalAccountFilePath()
{
    // Get user's system login
    char *user_name = getlogin();
    if (!user_name)
    {
        fprintf(stderr, "Couldn't get user name.\n");
        return NULL;
    }

    // Hash user name
    unsigned char user_name_hash[SHA_DIGEST_LENGTH];
    SHA1((unsigned char *)user_name, strlen(user_name), user_name_hash);
    char user_name_hash_string[SHA_DIGEST_LENGTH * 2 + 1];
    for (unsigned char i = 0; i < SHA_DIGEST_LENGTH; i++)
        sprintf(user_name_hash_string + (i * 2), "%02x", user_name_hash[i]);
    user_name_hash_string[SHA_DIGEST_LENGTH * 2] = '\0';

    // Build local account password file path
    char *local_account_password_file_path = calloc
    (
        sizeof *local_account_password_file_path,
        strlen("data/") + strlen(user_name_hash_string) + 1
    );
    if (!local_account_password_file_path)
    {
        fprintf(stderr, "ERROR: `local_account_password_file_path` calloc failed.\n");
        return NULL;
    }
    strcpy(local_account_password_file_path, "data/");
    strcat(local_account_password_file_path, user_name_hash_string);

    return local_account_password_file_path;
}

char isLocalAccountExists()
{
    // Check if data folder exists
    if (access("data", F_OK) == -1) return 0;

    char *local_account_password_file_path = getLocalAccountFilePath();

    // Check if local account password file exists
    if (access(local_account_password_file_path, F_OK) == -1)
    {
        printf("Local account password file doesn't exist.\n");
        return 0;
    }

    // Check if local account password file is empty
    FILE *local_account_password_file = fopen(local_account_password_file_path, "r");
    free(local_account_password_file_path);
    if (!local_account_password_file)
    {
        fprintf(stderr, "ERROR: couldn't open local account password file.\n");
        return -2;
    }
    fseek(local_account_password_file, 0L, SEEK_END);
    size_t local_account_password_file_size = ftell(local_account_password_file);
    fclose(local_account_password_file);
    if (local_account_password_file_size == 0)
    {
        printf("Local account password file is empty.\n");
        return 0;
    }

    return 1;
}

char *generateRandomSalt()
{
    char *salt = NULL;

    salt = calloc(sizeof *salt, SALT_LENGTH + 1);
    if (!salt) fprintf(stderr, "ERROR: salt malloc failed.\n");

    srand(time(NULL));
    for (int i = 0; i < SALT_LENGTH; i++)
        salt[i] = 33 + rand() % 94;
    salt[SALT_LENGTH] = '\0'; // Add null terminator

    return salt;
}

unsigned char *hashPasswordWithSalt(char *password, char *salt)
{
    // Salt password
    char *salted_password = calloc
    (
        sizeof *salted_password,
        (strlen(password) + strlen(salt)) + 1
    );
    if (!salted_password)
    {
        fprintf(stderr, "Memory reallocation for password failed.\n");

        return NULL;
    }
    salted_password[0] = '\0';
    strcat(salted_password, password);
    strcat(salted_password, salt);

    // Hash password
    unsigned char *password_hash = calloc
    (
        sizeof *password_hash,
        SHA512_DIGEST_LENGTH
    );
    if (!password_hash)
    {
        fprintf(stderr, "Memory allocation for password hash failed.\n");
        return NULL;
    }
    SHA512
    (
        (unsigned char *)salted_password,
        strlen(salted_password),
        password_hash
    );
    free(salted_password);

    return password_hash;
}

char createLocalAccount(char *password)
{
    // Check if password is strong enough
    if (!minimumPasswordRequirementsCheck(password))
    {
        printf("Password doesn't meet minimum requirements.\n");
        return -1;
    }

    // Create data folder if it doesn't exist
    if (access("data", F_OK) == -1)
    {
        if (mkdir("data", 0700) != 0)
        {
            fprintf(stderr, "Failed to create data folder.\n");
            return -2;
        }
    }

    char *local_account_password_file_path = getLocalAccountFilePath();

    // Hash password with random salt
    char *salt = generateRandomSalt();
    unsigned char *password_hash = hashPasswordWithSalt(password, salt);

    // Write salt to local account password file
    FILE *local_account_password_file = fopen(local_account_password_file_path, "w");
    free(local_account_password_file_path);
    if (!local_account_password_file)
    {
        fprintf(stderr, "Failed to open local account password file.\n");
        return -2;
    }
    fprintf(local_account_password_file, "%s\n", salt);
    free(salt);

    // Write password hash to local account password file
    for (unsigned char i = 0; i < SHA512_DIGEST_LENGTH; i++)
        fprintf(local_account_password_file, "%02x", password_hash[i]);
    fprintf(local_account_password_file, "\n");

    fclose(local_account_password_file);
    free(password_hash);

    return 0;
}

char connectLocalAccount(char *password)
{
    // Open local account file
    char *local_account_password_file_path = getLocalAccountFilePath();
    FILE *local_account_file = fopen(local_account_password_file_path, "r");
    free(local_account_password_file_path);
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
    char *password_hash_from_file_string = calloc
    (
        sizeof *password_hash_from_file_string,
        SHA512_DIGEST_LENGTH
    );
    if (!password_hash_from_file_string)
    {
        fprintf(stderr, "Memory allocation for password hash failed.\n");
        return -2;
    }
    size_t password_hash_from_file_string_length = strlen(password_hash_from_file_string);
    getline
    (
        &password_hash_from_file_string,
        &password_hash_from_file_string_length,
        local_account_file
    );
    password_hash_from_file_string[strlen(password_hash_from_file_string) - 1] = '\0'; // Remove newline

    // Close local account file
    if (fclose(local_account_file) != 0)
    {
        fprintf(stderr, "Failed to close local account file.\n");
        return -2;
    }

    // Hash password given by user
    unsigned char *password_hash_from_user = hashPasswordWithSalt(password, salt);
    free(salt);

    // Convert password hash to string
    char *password_hash_from_user_string = calloc
    (
        sizeof *password_hash_from_user_string,
        SHA512_DIGEST_LENGTH * 2 + 1
    );
    if (!password_hash_from_user_string)
    {
        fprintf(stderr, "Memory allocation for password hash hex failed.\n");
        return -2;
    }
    for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
    {
        sprintf(password_hash_from_user_string + (i * 2), "%02x", password_hash_from_user[i]);
    }
    free(password_hash_from_user);
    password_hash_from_user_string[SHA512_DIGEST_LENGTH * 2] = '\0'; // Add null terminator

    // Compare password hashes
    char password_hashes_comparison_result = strcmp
    (
        password_hash_from_file_string,
        password_hash_from_user_string
    );
    free(password_hash_from_file_string);
    free(password_hash_from_user_string);
    if (password_hashes_comparison_result == 0) return 0;
    else return -1;
}

char deleteLocalAccount(char *password)
{
    char connect_local_account_result = 0; //connectLocalAccount(password);
    if (connect_local_account_result == 0)
    {
        // Delete local account files
        char * files_to_delete[3] =
        {
            getLocalAccountFilePath(),
            getEncDecFilePath("credentials"),
            getEncDecFilePath("labels")
        };
        for (int i = 0; i < 3; i++) remove(files_to_delete[i]);
        remove("data");

        return 0;
    }
    else
        return connect_local_account_result;
}
