#include <errno.h>
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
 * @return 0 if successful, -1 on error,  -2 if label already exists
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

/**
 * @brief Check if label file exists.
 * @return 1 if label file exists, 0 otherwise.
 */
unsigned char doesLabelFileExist();

/**
 * @brief Check if label file is empty.
 * @return 1 if label file is empty, 0 otherwise.
 */
unsigned char isLabelFileEmpty();

/**
 * @brief Check if label exists.
 * @param label A string label to check.
 * @return 1 if label exists, 0 otherwise.
 */
unsigned char doesLabelExist(const char * label);

/**
 * @brief Delete label.
 * @param label A string label to delete.
 * @return 0 on success, -1 if there's no label to delete, -2 if no label match
 * with given label, exit with failure otherwise.
 */
char deleteLabel(char * label);

/**
 * @brief Edit credentials.
 * @param label String of the credentials' label to edit.
 * @param new_credentials Edited credentials.
 * @return 0 on success, -1 if there's no credentials to edit, -2 if no
 * credentials match with given label, exit with failure otherwise.
 */
char editCredentials(CREDENTIALS_T new_credentials);

/**
 * @brief Delete credentials.
 * @param label A string credentials' label to delete.
 * @return 0 on success, -1 if there's no credentials to delete, -2 if no
 * credentials match with given label, exit with failure otherwise.
 */
char deleteCredentials(char * label);
