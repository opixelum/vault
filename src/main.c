#include "../include/password.h"
#include "../include/ui.h"

int main(int argc, char const *argv[])
{
    char *pswd = genPswd(8, 0);
    printf("%s\n", pswd);
    free(pswd);

    displayMenu();

    return 0;
}
