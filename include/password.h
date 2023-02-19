#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define SYMBOLS "!\"#$%&'()*+-./:;<=>?@[\\]^_`{|}~"

int hasLength(size_t, int);
int hasSpecialChar(char *, size_t);
int hasLowercase(char *, size_t);
int hasUppercase(char *, size_t);

/**
 * @brief Checks if the given password respects the minimum password strength
 * requirements
 * @param password The password to check
 * @return 1 if given password satisfies the minimum requirements, 0 otherwise
 */
char minimumPasswordRequirementsCheck(char *password);

int pswdCheck(char *, size_t, int, char *);
char *genPswd(size_t, int);
