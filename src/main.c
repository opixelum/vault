#include "../include/password.h"

int main(int argc, char const *argv[])
{
    char *pswd = genPswd(8, 0);
    printf("%s\n", pswd);
    free(pswd);

    return 0;
}
