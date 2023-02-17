#include <sys/stat.h>
#include "encdec.h"

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
