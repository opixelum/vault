#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "password.h"
#include "encdec.h"
#include "utils.h"

typedef struct CredentialsStruct
{
    char *label;
    char *url;
    char *username;
    char *email;
    char *password;
}
CREDENTIALS_T;

char storeFirstCredentials(CREDENTIALS_T credentials, char * password);

/**
 * @brief Store credentials in a csv file.
 * @param credentials The credentials to store.
 * @return 0 if successful, 1 if not.
*/
char storeCredentials(CREDENTIALS_T credentials, char * password);
