#include <stdlib.h>
#include <stdio.h>
#include "password.h"

char storeCredentials(Credentials_t);


typedef struct Credentials_t {
    char *label;
    char *url;
    char *username;
    char *email;
    char *password;
} Credentials_t;
