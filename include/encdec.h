#include <openssl/evp.h>
#include <openssl/sha.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Builds the file path for encrypted/decrypted related data such as the
 * IV, the tag & credentials.
 * @param data A string of wanted data, such as "iv", "tag" or "credentials".
 * @return A pointer to the built file path.
 * @warning The returned pointer must be freed.
 */
char *getEncDecFilePath(const char *data);

int do_crypt(FILE *in, FILE *out, int do_encrypt);
