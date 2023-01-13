#include "credentials.h"

char storeCredentials(Credentials_t credentials)
{
    // Open file
    FILE *file = fopen("credentials.csv", "r");
    if (file == NULL)
    {
        file = fopen("credentials.csv", "a");
        fprintf(file, "Label, URL, Username, Email, Password\n");
    }
    else
    {
        file = fopen("credentials.csv", "a");
        if (file == NULL)
        {
            fprintf(stderr, "Error: Could not open file.\n");
            return -1;
        }
    }

    // Write to file
    char *csv_text = strcat(credentials.label, ",");
    csv_text = strcat(csv_text, credentials.url);
    csv_text = strcat(csv_text, ",");
    csv_text = strcat(csv_text, credentials.username);
    csv_text = strcat(csv_text, ",");
    csv_text = strcat(csv_text, credentials.email);
    csv_text = strcat(csv_text, ",");
    csv_text = strcat(csv_text, credentials.password);
    csv_text = strcat(csv_text, "\n");

    if (fprintf(file, "%s", csv_text) == EOF)
    {
        fprintf(stderr, "Error: Could not write to file.\n");
        return -1;
    }

    // Close file
    if (fclose(file) == EOF)
    {
        fprintf(stderr, "Error: Could not close file.\n");
        return -1;
    }

    return 0;
}
