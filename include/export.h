#include <stdio.h>
#include <hpdf.h>
#include <time.h>
#include "password.h"
#include "encdec.h"

void exportCredentials();
char exportCredentialsAsPDF();
/**
 * @brief Export credentials as CSV in the home directory.
 * @return 0 if successful, exit with failure otherwise.
 */
unsigned char exportCredentialsAsCSV();
