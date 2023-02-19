#include "export.h"

#define MAX_LENGTH 255

void exportCredentials()
{
    unsigned char choice;

    do
    {
        printf("\nExport credentials\n\n\
    1. Export as PDF\n\
    2. Export as CSV\n\nEnter your choice: ");

        scanf("%hhd", &choice);

        switch (choice)
        {
        case 1:
            exportCredentialsAsPDF();
            break;

        case 2:
            exportCredentialsAsCSV();
            break;

        default:
            printf("Invalid choice\n");
            break;
        }
    } while (choice != 1 && choice != 2);
}

unsigned char exportCredentialsAsPDF()
{
    char *line = malloc(sizeof *line * MAX_LENGTH);
    char *label = malloc(sizeof *label * MAX_LENGTH);
    char *url = malloc(sizeof *url * MAX_LENGTH);
    char *username = malloc(sizeof *username * MAX_LENGTH);
    char *email = malloc(sizeof *email * MAX_LENGTH);
    char *password = malloc(sizeof *password * MAX_LENGTH);

    // Decrypt credentials
    char * encrypted_credentials_file_path = getEncDecFilePath("credentials");
    FILE * encrypted_credentials_file = fopen(encrypted_credentials_file_path, "rb");
    free(encrypted_credentials_file_path);
    if (!encrypted_credentials_file)
    {
        fprintf(stderr, "ERROR: `encrypted_credentials_file` fopen failed.\n");
        exit(EXIT_FAILURE);
    }
    char * temporary_file_path = getEncDecFilePath("temporary");
    FILE * temporary_file = fopen(temporary_file_path, "w");
    if (!temporary_file)
    {
        fprintf(stderr, "ERROR: `exported_file` fopen failed.\n");
        exit(EXIT_FAILURE);
    }
    do_crypt(encrypted_credentials_file, temporary_file, 0);
    if (fclose(encrypted_credentials_file) == EOF)
    {
        fprintf(stderr, "ERROR: `encrypted_credentials_file` fclose failed.\n");
        exit(EXIT_FAILURE);
    }
    if (fclose(temporary_file) == EOF)
    {
        fprintf(stderr, "ERROR: `temporary_file` fclose failed.\n");
        if (remove(temporary_file_path) == -1)
            fprintf
            (
                stderr,
                "ERROR: couldn't delete `temporary_file`: %s.\n",
                strerror(errno)
            );
        exit(EXIT_FAILURE);
    }

    HPDF_Doc pdf;
    HPDF_Page page;
    HPDF_Font font;

    // Create a new PDF document
    pdf = HPDF_New(NULL, NULL);
    if (!pdf)
    {
        printf("Error: Unable to create PDF document.\n");
        return 1;
    }

    temporary_file = fopen(temporary_file_path, "r");
    if (!temporary_file)
    {
        fprintf(stderr, "ERROR: `temporary_file` fopen failed.\n");
        exit(EXIT_FAILURE);
    }

    // Read the header line
    fgets(line, MAX_LENGTH, temporary_file);

    int y = 750; // Cursor position
    int x = 50;  // Cursor position

    const int THRESHOLD = 60;
    const int START_Y = y;

    while (fgets(line, MAX_LENGTH, temporary_file))
    {
        if (y == START_Y)
        {
            /* Create a new page */
            page = HPDF_AddPage(pdf);
            HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

            /* Set the font and font size */
            font = HPDF_GetFont(pdf, "Helvetica", NULL);
            HPDF_Page_SetFontAndSize(page, font, 12);
        }

        /* Write the content of the credentials variable to the page */
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^\n]", label, url, username, email, password);

        HPDF_Page_BeginText(page);
        HPDF_Page_MoveTextPos(page, x, y);
        HPDF_Page_ShowText(page, "Label: ");
        HPDF_Page_ShowText(page, label);
        HPDF_Page_EndText(page);

        y -= 20; // Move the cursor at the next line

        HPDF_Page_BeginText(page);
        HPDF_Page_MoveTextPos(page, x, y);
        HPDF_Page_ShowText(page, "Url: ");
        HPDF_Page_ShowText(page, url);
        HPDF_Page_EndText(page);

        y -= 20; // Move the cursor at the next line

        HPDF_Page_BeginText(page);
        HPDF_Page_MoveTextPos(page, x, y);
        HPDF_Page_ShowText(page, "Username: ");
        HPDF_Page_ShowText(page, username);
        HPDF_Page_EndText(page);

        y -= 20; // Move the cursor at the next line

        HPDF_Page_BeginText(page);
        HPDF_Page_MoveTextPos(page, x, y);
        HPDF_Page_ShowText(page, "Email: ");
        HPDF_Page_ShowText(page, email);
        HPDF_Page_EndText(page);

        y -= 20; // Move the cursor at the next line

        HPDF_Page_BeginText(page);
        HPDF_Page_MoveTextPos(page, x, y);
        HPDF_Page_ShowText(page, "Password: ");
        HPDF_Page_ShowText(page, password);
        HPDF_Page_EndText(page);

        y -= 40; // Add a blank line

        if (y < THRESHOLD)
        {
            y = START_Y;
        }
    }

    if (fclose(temporary_file) == EOF)
    {
        fprintf(stderr, "ERROR: `temporary_file` fclose failed.\n");
        exit(EXIT_FAILURE);
    }

    // Delete the temporary file
    if (remove(temporary_file_path) == -1)
    {
        free(temporary_file_path);
        fprintf
        (
            stderr,
            "ERROR: couldn't delete `temporary_file`: %s.\n",
            strerror(errno)
        );
        exit(EXIT_FAILURE);
    }
    free(temporary_file_path);

    // Build exported file name
    time_t current_time = time(NULL);
    struct tm *local_time = localtime(&current_time);
    if (!local_time)
    {
        fprintf(stderr, "ERROR: couldn't get local timezone representation.\n");
        exit(EXIT_FAILURE);
    }
    char file_name[55];
    sprintf
    (
        file_name,
        "exported_credentials_%02d-%02d-%04d_%02d-%02d-%02d.pdf",
        local_time->tm_mday,
        local_time->tm_mon + 1,
        local_time->tm_year + 1900,
        local_time->tm_hour,
        local_time->tm_min,
        local_time->tm_sec
    );

    // Build exported file path
    char * home_directory = getenv("HOME");
    if (!home_directory)
    {
        fprintf(stderr, "ERROR: `home_directory` getenv failed.\n");
        exit(EXIT_FAILURE);
    }
    char * exported_file_path = malloc
    (
        strlen(home_directory) + strlen(file_name) + 2 // +2 for '/' & '\0'
    );
    if (!exported_file_path)
    {
        fprintf(stderr, "ERROR: `exported_file_path` malloc failed.\n");
        exit(EXIT_FAILURE);
    }
    sprintf(exported_file_path, "%s/%s", home_directory, file_name);

    HPDF_SaveToFile(pdf, exported_file_path);
    free(exported_file_path);

    HPDF_Free(pdf);
    free(line);
    free(label);
    free(url);
    free(username);
    free(email);
    free(password);

    return 0;
}

unsigned char exportCredentialsAsCSV()
{
    // Build exported file name
    time_t current_time = time(NULL);
    struct tm *local_time = localtime(&current_time);
    if (!local_time)
    {
        fprintf(stderr, "ERROR: couldn't get local timezone representation.\n");
        exit(EXIT_FAILURE);
    }
    char file_name[55];
    sprintf
    (
        file_name,
        "exported_credentials_%02d-%02d-%04d_%02d-%02d-%02d.csv",
        local_time->tm_mday,
        local_time->tm_mon + 1,
        local_time->tm_year + 1900,
        local_time->tm_hour,
        local_time->tm_min,
        local_time->tm_sec
    );

    // Build exported file path
    char * home_directory = getenv("HOME");
    if (!home_directory)
    {
        fprintf(stderr, "ERROR: `home_directory` getenv failed.\n");
        exit(EXIT_FAILURE);
    }
    char * exported_file_path = malloc
    (
        strlen(home_directory) + strlen(file_name) + 2 // +2 for '/' & '\0'
    );
    if (!exported_file_path)
    {
        fprintf(stderr, "ERROR: `exported_file_path` malloc failed.\n");
        exit(EXIT_FAILURE);
    }
    sprintf(exported_file_path, "%s/%s", home_directory, file_name);

    // Decrypt credentials
    char * encrypted_credentials_file_path = getEncDecFilePath("credentials");
    FILE * encrypted_credentials_file = fopen(encrypted_credentials_file_path, "rb");
    free(encrypted_credentials_file_path);
    if (!encrypted_credentials_file)
    {
        fprintf(stderr, "ERROR: `encrypted_credentials_file` fopen failed.\n");
        exit(EXIT_FAILURE);
    }
    FILE *exported_file = fopen(exported_file_path, "w");
    free(exported_file_path);
    if (!exported_file)
    {
        fprintf(stderr, "ERROR: `exported_file` fopen failed.\n");
        exit(EXIT_FAILURE);
    }

    // Decrypt credentials file
    do_crypt(encrypted_credentials_file, exported_file, 0);

    if (fclose(encrypted_credentials_file) == EOF)
    {
        fprintf(stderr, "ERROR: Couldn't close `encrypted_credentials_file`.\n");
        exit(EXIT_FAILURE);
    }
    if (fclose(exported_file) == EOF)
    {
        fprintf(stderr, "ERROR: Couldn't close `exported_file`.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
