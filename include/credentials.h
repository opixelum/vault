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
 * @brief Encrypt & store label.
 * @param label A string of the label to store.
 * @return 0 if successful, exit with failure otherwise
 */
unsigned char storeLabel(char * label);

/**
 * @brief Encrypt & store credentials.
 * @param credentials The credentials to store.
 * @return 0 if successful, -1 otherwise.
 */
char storeCredentials(CREDENTIALS_T credentials);

/**
 * @brief Retrieve all labels for GUI purposes.
 * @return An array of labels. If no labels are found, the first element of the
 * array is NULL.
 */
char ** getLabels();

/**
 * @brief Retrieve credentials.
 * @param label The label of the credentials to retrieve.
 * @return A pointer to the credentials if successful, NULL otherwise.
 * @warning The returned pointer must be freed.
 */
CREDENTIALS_T * getCredentials(char * label);
