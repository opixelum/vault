#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "password.h"

typedef struct CredentialsStruct
{
    char *label;
    char *url;
    char *username;
    char *email;
    char *password;
}
CREDENTIALS_T;

/**
 * @brief Store credentials in a csv file.
 * @param Credentials_t The credentials to store.
 * @return 0 if successful, 1 if not.
*/
char storeCredentials(CREDENTIALS_T credentials);
