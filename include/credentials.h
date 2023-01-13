#include <stdlib.h>
#include <stdio.h>
#include "password.h"

typedef struct Credentials_t {
    char *label;
    char *url;
    char *username;
    char *email;
    char *password;
} Credentials_t;

/**
 * @brief Store credentials in a csv file.
 * @param Credentials_t The credentials to store.
 * @return 0 if successful, 1 if not.
*/
char storeCredentials(Credentials_t);
