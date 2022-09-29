#include "libs/password/password.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
    char *pswd = genPswd(100);
    printf("%s\n", pswd);
    free(pswd);

    return 0;
}
