#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
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


/**
 * @brief Encrypt & store credentials.
 * @param credentials The credentials to store.
 * @return 0 if successful, -1 otherwise.
*/
char storeCredentials(CREDENTIALS_T credentials);
