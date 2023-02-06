#include <stdio.h>
#include <hpdf.h>
#include <time.h>
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

char exportCredentials(CREDENTIALS_T credentials);
