#include "ui.h"
#include "local_account.h"

int main(int argc, char const *argv[])
{
    unsigned char isRunning = 0;

    while (isRunning)
    {
        mainMenu(&isRunning);
    }

    // Hash test
    char *string = "test";
    unsigned char *hash = sha512Hash(string);
    for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
    {
        printf("%02x", hash[i]);
    }
    free(hash);
    printf("\n");

    return 0;
}
