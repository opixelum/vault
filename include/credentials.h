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


char storeCredentials(Credentials_t);
