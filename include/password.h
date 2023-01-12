#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define SYMBOLS "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"

int hasLength(size_t, int);
int hasSpecialChar(char *, size_t);
int hasLowercase(char *, size_t);
int hasUppercase(char *, size_t);
int pswdCheck(char *, size_t, int, char *);
char *genPswd(size_t, int);
