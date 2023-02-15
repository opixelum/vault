#include "utils.h"

size_t getFileSize(FILE * file)
{
    size_t file_size = 0;

    // Got to EOF
    fseek(file, 0L, SEEK_END); 

    // Get current position of file pointer (this tell us the size)
    file_size = ftell(file);

    // Set pointer back to beginning of file
    rewind(file);    

    return file_size;
}
