#include "store.h"

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
    }

    // Write to file
    fprintf(file, "%s,", credentials.label);
    fprintf(file, "%s,", credentials.url);
    fprintf(file, "%s,", credentials.username);
    fprintf(file, "%s,", credentials.email);
    fprintf(file, "%s\n", credentials.password);

    // Close file
    fclose(file);

    return 0;
}
