#include "utils.h"

unsigned char isDirectory(const char * path)
{
    struct stat statbuf;
    if (stat(path, &statbuf) != 0) return 0;
    int s_isdir_result = S_ISDIR(statbuf.st_mode);
    if (s_isdir_result == 0) return 0;
    return 1;
}