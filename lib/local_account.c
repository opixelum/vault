#include "local_account.h"

unsigned char *sha512Hash(const char *string)
{
    unsigned char *hash = malloc(sizeof *hash * SHA512_DIGEST_LENGTH);

    SHA512((unsigned char *) string, SHA512_DIGEST_LENGTH, hash);

    return hash;
}
