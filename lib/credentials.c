#include "credentials.h"

unsigned char storeLabel(char *label)
{
    // Check if data directory exists
    if (access("data", F_OK) == -1)
    {
        if (mkdir("data", 0700) == -1)
        {
            fprintf(stderr, "ERROR: Could not create data directory.\n");
            exit(EXIT_FAILURE);
        }
    }

    char *encrypted_file_path = getEncDecFilePath("labels");
    char *temporary_file_path = getEncDecFilePath("temporary");

    // Decrypt the encrypted file to a temporary file
    FILE *encrypted_file = fopen(encrypted_file_path, "rb");
    // If the encrypted file does not exist, create it
    if (!encrypted_file)
    {
        encrypted_file = fopen(encrypted_file_path, "wb");
        fclose(encrypted_file);
        encrypted_file = fopen(encrypted_file_path, "rb");
    }
    FILE *temporary_file = fopen(temporary_file_path, "w");
    do_crypt(encrypted_file, temporary_file, 0);
    fclose(encrypted_file);
    fclose(temporary_file);

    // Append the label to the temporary file
    temporary_file = fopen(temporary_file_path, "a");
    fprintf(temporary_file, "%s\n", label);
    fclose(temporary_file);

    // Encrypt the temporary file to the encrypted file
    temporary_file = fopen(temporary_file_path, "rb");
    encrypted_file = fopen(encrypted_file_path, "wb");
    free(encrypted_file_path);
    do_crypt(temporary_file, encrypted_file, 1);
    fclose(temporary_file);
    fclose(encrypted_file);

    // Delete the temporary file
    if (remove(temporary_file_path) == -1)
    {
        free(temporary_file_path);
        fprintf(stderr, "ERROR: Could not delete temporary file.\n");
        exit(EXIT_FAILURE);
    }
    free(temporary_file_path);

    return 0;
}

char storeCredentials(CREDENTIALS_T credentials)
{
    if (doesLabelExist(credentials.label))
        return -2;

    storeLabel(credentials.label);

    char *encrypted_file_path = getEncDecFilePath("credentials");
    char *temporary_file_path = getEncDecFilePath("temporary");

    // Decrypt the encrypted file to a temporary file
    FILE *encrypted_file = fopen(encrypted_file_path, "rb");
    // If the encrypted file does not exist, create it
    if (!encrypted_file)
    {
        encrypted_file = fopen(encrypted_file_path, "wb");
        fclose(encrypted_file);
        encrypted_file = fopen(encrypted_file_path, "rb");
    }
    FILE *temporary_file = fopen(temporary_file_path, "wb");
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

    // If credentials are empty, set "NULL"
    if (strlen(credentials.url) == 0)
        credentials.url = "NULL";
    if (strlen(credentials.username) == 0)
        credentials.username = "NULL";
    if (strlen(credentials.email) == 0)
        credentials.email = "NULL";
    if (strlen(credentials.password) == 0)
        credentials.password = "NULL";

    // Write new credentials to temporary file
    fprintf(
        temporary_file,
        "%s,%s,%s,%s,%s\n",
        credentials.label,
        credentials.url,
        credentials.username,
        credentials.email,
        credentials.password);
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

char **getLabels()
{
    // Check if data directory exists
    if (access("data", F_OK) == -1)
    {
        if (mkdir("data", 0700) == -1)
        {
            fprintf(stderr, "ERROR: Could not create data directory.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Decrypt the encrypted file to a temporary file
    char *encrypted_file_path = getEncDecFilePath("labels");
    FILE *encrypted_file = fopen(encrypted_file_path, "rb");
    // If the encrypted file does not exist, create it
    if (!encrypted_file)
    {
        encrypted_file = fopen(encrypted_file_path, "wb");
        fclose(encrypted_file);
        encrypted_file = fopen(encrypted_file_path, "rb");
    }
    free(encrypted_file_path);
    char *temporary_file_path = getEncDecFilePath("temporary");
    FILE *temporary_file = fopen(temporary_file_path, "wb");
    if (!temporary_file)
    {
        fclose(encrypted_file);
        free(encrypted_file_path);
        free(temporary_file_path);
        return NULL;
    }
    do_crypt(encrypted_file, temporary_file, 0);
    fclose(encrypted_file);
    fclose(temporary_file);

    // Open temporary file for reading
    temporary_file = fopen(temporary_file_path, "r");

    // Loop through the temporary file and count the number of labels
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int num_labels = 0;
    while ((read = getline(&line, &len, temporary_file)) != -1)
        num_labels++;
    free(line);
    fclose(temporary_file);

    // Allocate memory for the labels array
    char **labels = malloc(sizeof(char *) * (num_labels + 1));
    if (!labels)
    {
        free(encrypted_file_path);
        free(temporary_file_path);
        return NULL;
    }

    // Open temporary file for reading
    temporary_file = fopen(temporary_file_path, "r");

    // Loop through the temporary file and store the labels
    line = NULL;
    len = 0;
    read = 0;
    int i = 0;
    while ((read = getline(&line, &len, temporary_file)) != -1)
    {
        // Remove the newline character
        line[strlen(line) - 1] = '\0';
        labels[i] = malloc(sizeof(char) * (strlen(line) + 1));
        strcpy(labels[i], line);
        i++;
    }
    free(line);
    fclose(temporary_file);

    // Add a NULL pointer to the end of the array
    labels[num_labels] = NULL;

    // Remove the temporary file
    if (remove(temporary_file_path) != 0)
        fprintf(stderr, "ERROR: Could not remove temporary file.\n");
    free(temporary_file_path);

    return labels;
}

CREDENTIALS_T *getCredentials(char *label)
{
    // Decrypt the encrypted file to a temporary file
    char *encrypted_file_path = getEncDecFilePath("credentials");
    FILE *encrypted_file = fopen(encrypted_file_path, "rb");
    if (!encrypted_file)
    {
        free(encrypted_file_path);
        return NULL;
    }
    char *temporary_file_path = getEncDecFilePath("temporary");
    FILE *temporary_file = fopen(temporary_file_path, "wb");
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
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    CREDENTIALS_T *credentials = NULL;
    while ((read = getline(&line, &len, temporary_file)) != -1)
    {
        // Remove the newline character
        line[strlen(line) - 1] = '\0';

        // Split the line into tokens
        char *token = strtok(line, ",");
        char *tokens[5];
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
        free(line);
        len = 0;
    }
    free(line);
    fclose(temporary_file);

    // Remove the temporary file
    remove(temporary_file_path);
    free(temporary_file_path);

    return credentials;
}

unsigned char doesLabelFileExist()
{
    char *labels_file_path = getEncDecFilePath("labels");
    FILE *labels_file = fopen(labels_file_path, "rb");
    if (!labels_file)
    {
        free(labels_file_path);
        return 0;
    }
    fclose(labels_file);
    free(labels_file_path);

    return 1;
}

unsigned char isLabelFileEmpty()
{
    if (!doesLabelFileExist())
        return 0;

    // Decrypt labels file to a temporary file
    char *label_file_path = getEncDecFilePath("labels");
    FILE *label_file = fopen(label_file_path, "rb");
    free(label_file_path);
    if (!label_file)
    {
        free(label_file_path);
        fprintf(stderr, "ERROR: Could not open labels file.\n");
        exit(EXIT_FAILURE);
    }
    char *temporary_file_path = getEncDecFilePath("temporary");
    FILE *temporary_file = fopen(temporary_file_path, "wb");
    if (!temporary_file)
    {
        free(temporary_file_path);
        fprintf(stderr, "ERROR: Could not open temporary file.\n");
        exit(EXIT_FAILURE);
    }
    do_crypt(label_file, temporary_file, 0);
    fclose(label_file);
    rewind(temporary_file);

    fseek(temporary_file, 0L, SEEK_END);
    long temporary_file_size = ftell(temporary_file);

    fclose(temporary_file);
    remove(temporary_file_path);
    free(temporary_file_path);

    if (temporary_file_size == 0)
        return 1;
    return 0;
}

unsigned char doesLabelExist(const char *label)
{
    if (!doesLabelFileExist())
        return 0;

    // Decrypt the encrypted file to a temporary file
    char *labels_file_path = getEncDecFilePath("labels");
    FILE *labels_file = fopen(labels_file_path, "rb");
    if (!labels_file)
    {
        free(labels_file_path);
        fprintf(stderr, "ERROR: Could not open labels file.\n");
        exit(EXIT_FAILURE);
    }
    char *temporary_file_path = getEncDecFilePath("temporary");
    FILE *temporary_file = fopen(temporary_file_path, "wb");
    if (!temporary_file)
    {
        fclose(labels_file);
        free(labels_file_path);
        free(temporary_file_path);
        fprintf(stderr, "ERROR: Could not open temporary file.\n");
        exit(EXIT_FAILURE);
    }
    do_crypt(labels_file, temporary_file, 0);
    fclose(labels_file);
    fclose(temporary_file);
    free(labels_file_path);

    // Open temporary file for reading
    temporary_file = fopen(temporary_file_path, "r");

    // Loop through the temporary file and find the label
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, temporary_file)) != -1)
    {
        // Remove the newline character
        line[strlen(line) - 1] = '\0';
        if (strcmp(line, label) == 0)
        {
            // Label found
            free(line);
            fclose(temporary_file);
            remove(temporary_file_path);
            free(temporary_file_path);
            return 1;
        }
        free(line);
        len = 0;
    }

    // Label not found
    free(line);
    fclose(temporary_file);
    remove(temporary_file_path);
    free(temporary_file_path);

    return 0;
}

char deleteLabel(char *label)
{
    if (!doesLabelFileExist())
        return -1;
    if (isLabelFileEmpty())
        return -1;
    if (!doesLabelExist(label))
        return -2;

    // Decrypt the encrypted file to a temporary file
    char *labels_file_path = getEncDecFilePath("labels");
    FILE *labels_file = fopen(labels_file_path, "rb");
    if (!labels_file)
    {
        free(labels_file_path);
        fprintf(stderr, "ERROR: Could not open labels file.\n");
        exit(EXIT_FAILURE);
    }
    char *temporary_file_path = getEncDecFilePath("temporary");
    FILE *temporary_file = fopen(temporary_file_path, "wb");
    if (!temporary_file)
    {
        fclose(labels_file);
        free(labels_file_path);
        free(temporary_file_path);
        fprintf(stderr, "ERROR: Could not open temporary file.\n");
        exit(EXIT_FAILURE);
    }
    do_crypt(labels_file, temporary_file, 0);
    fclose(labels_file);
    fclose(temporary_file);

    // Open temporary file for reading
    temporary_file = fopen(temporary_file_path, "r");
    if (!temporary_file)
    {
        fprintf(
            stderr,
            "ERROR: couldn't open `temporary_file`: %s.\n",
            strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Create new temporary file without deleted credentials
    char *new_temporary_file_path = getEncDecFilePath("new temporary");
    FILE *new_temporary_file = fopen(new_temporary_file_path, "w");
    if (!new_temporary_file)
    {
        fprintf(
            stderr,
            "ERROR: couldn't create `new_temporary_file`: %s.\n",
            strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Rewrite the temporary file without the deleted credentials
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, temporary_file)) != -1)
    {
        // Remove the newline character
        line[strlen(line) - 1] = '\0';
        if (strcmp(line, label) != 0)
            fprintf(new_temporary_file, "%s\n", line);
        free(line);
        len = 0;
    }
    free(line);

    // Close the files
    fclose(temporary_file);
    fclose(new_temporary_file);

    // Remove the temporary file
    remove(temporary_file_path);
    free(temporary_file_path);

    // Encrypt the new temporary file to the encrypted file
    labels_file = fopen(labels_file_path, "wb");
    free(labels_file_path);
    if (!labels_file)
    {
        fprintf(
            stderr,
            "ERROR: couldn't open `labels_file`: %s.\n",
            strerror(errno));
        exit(EXIT_FAILURE);
    }
    new_temporary_file = fopen(new_temporary_file_path, "rb");
    if (!new_temporary_file)
    {
        fprintf(
            stderr,
            "ERROR: couldn't open `new_temporary_file`: %s.\n",
            strerror(errno));
        exit(EXIT_FAILURE);
    }
    do_crypt(new_temporary_file, labels_file, 1);
    fclose(labels_file);
    fclose(new_temporary_file);

    // Remove the new temporary file
    remove(new_temporary_file_path);
    free(new_temporary_file_path);

    return 0;
}

char deleteCredentials(char *label)
{
    char label_deletion_status = deleteLabel(label);
    if (label_deletion_status != 0)
        return label_deletion_status;

    // Decrypt the encrypted file to a temporary file
    char *credentials_file_path = getEncDecFilePath("credentials");
    FILE *credentials_file = fopen(credentials_file_path, "rb");
    if (!credentials_file)
    {
        free(credentials_file_path);
        fprintf(stderr, "ERROR: Could not open credentials file.\n");
        exit(EXIT_FAILURE);
    }
    char *temporary_file_path = getEncDecFilePath("temporary");
    FILE *temporary_file = fopen(temporary_file_path, "wb");
    if (!temporary_file)
    {
        fclose(credentials_file);
        free(credentials_file_path);
        free(temporary_file_path);
        fprintf(stderr, "ERROR: Could not open temporary file.\n");
        exit(EXIT_FAILURE);
    }
    do_crypt(credentials_file, temporary_file, 0);
    fclose(credentials_file);
    fclose(temporary_file);

    // Open temporary file for reading
    temporary_file = fopen(temporary_file_path, "r");
    if (!temporary_file)
    {
        fprintf(
            stderr,
            "ERROR: couldn't open `temporary_file`: %s.\n",
            strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Create new temporary file without deleted credentials
    char *new_temporary_file_path = getEncDecFilePath("new temporary");
    FILE *new_temporary_file = fopen(new_temporary_file_path, "w");
    if (!new_temporary_file)
    {
        fprintf(
            stderr,
            "ERROR: couldn't create `new_temporary_file`: %s.\n",
            strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Rewrite the temporary file without the deleted credentials
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, temporary_file)) != -1)
    {
        char *line_to_tok = strdup(line);
        char *label_column = strtok(line_to_tok, ",");
        if (strcmp(label_column, label) != 0)
            fprintf(new_temporary_file, "%s", line);
        free(line);
        len = 0;
    }
    free(line);

    fclose(temporary_file);
    fclose(new_temporary_file);

    // Remove the temporary file
    remove(temporary_file_path);
    free(temporary_file_path);

    // Encrypt the new temporary file to the encrypted file
    credentials_file = fopen(credentials_file_path, "wb");
    free(credentials_file_path);
    if (!credentials_file)
    {
        fprintf(
            stderr,
            "ERROR: couldn't open `credentials_file`: %s.\n",
            strerror(errno));
        exit(EXIT_FAILURE);
    }
    new_temporary_file = fopen(new_temporary_file_path, "rb");
    if (!new_temporary_file)
    {
        fprintf(
            stderr,
            "ERROR: couldn't open `new_temporary_file`: %s.\n",
            strerror(errno));
        exit(EXIT_FAILURE);
    }
    do_crypt(new_temporary_file, credentials_file, 1);
    fclose(credentials_file);
    fclose(new_temporary_file);

    // Remove the new temporary file
    remove(new_temporary_file_path);
    free(new_temporary_file_path);

    return 0;
}

char editCredentials(CREDENTIALS_T new_credentials)
{
    // Delete the old credentials
    // It already checks if the label exists
    char credentials_deletion_status = deleteCredentials(new_credentials.label);
    if (credentials_deletion_status != 0) return credentials_deletion_status;

    // Add the new credentials
    if (storeCredentials(new_credentials) == -1)
    {
        fprintf(stderr, "ERROR: Couldn't store new credentials.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int compare_strings(const void * a, const void * b)
{
    const char **s1 = (const char **)a;
    const char **s2 = (const char **)b;
    return strcasecmp(*s1, *s2);
}

// Function to sort lines in a file
void sort_lines(const char * filename)
{
    FILE * fp;
    char lines[MAX_LINES][MAX_LINE_LENGTH];
    char *line_ptrs[MAX_LINES];
    int n = 0;

    fp = fopen(filename, "r+");
    if (fp == NULL)
    {
        printf("Failed to open file.\n");
        return;
    }

    // Read lines from file and store them in array of strings
    while (fgets(lines[n], MAX_LINE_LENGTH, fp) != NULL)
    {
        // Remove newline character from end of line
        lines[n][strcspn(lines[n], "\n")] = '\0';
        // Store pointer to line in array of pointers
        line_ptrs[n] = lines[n];
        n++;
    }

    // Sort the lines using qsort function
    qsort(line_ptrs, n, sizeof(char *), compare_strings);

    // Write sorted lines back to input file
    rewind(fp);
    for (int i = 0; i < n; i++)
    {
        fputs(line_ptrs[i], fp);
        fputc('\n', fp);
    }

    fclose(fp);
}
