#include "local_account.h"

unsigned char *sha512Hash(const char *string)
{
    unsigned char *hash = malloc(sizeof *hash * SHA512_DIGEST_LENGTH);

    SHA512((unsigned char *) string, SHA512_DIGEST_LENGTH, hash);

    return hash;
}

int sha512HashCompare(const unsigned char *hash1, const unsigned char *hash2)
{
    return memcmp(hash1, hash2, SHA512_DIGEST_LENGTH);
}
