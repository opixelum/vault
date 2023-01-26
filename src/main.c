#include "ui.h"
#include "local_account.h"

int main(int argc, char const *argv[])
{
    unsigned char isRunning = 0;

    while (isRunning)
    {
        mainMenu(&isRunning);
    }

    // Compare two hashes
    char *string1 = "tast";
    char *string2 = "tast";
    unsigned char *hash1 = sha512Hash(string1);
    unsigned char *hash2 = sha512Hash(string2);
    if (sha512HashCompare(hash1, hash2) == 0)
    {
        printf("Hashes are equal\n");
    }
    else
    {
        printf("Hashes are not equal\n");
    }
    free(hash1);
    free(hash2);

    return 0;
}
