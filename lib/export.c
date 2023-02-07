#include "export.h"

#define MAX_LENGTH 255

char exportCredentials()
{
    char *line = malloc(MAX_LENGTH * sizeof(char));
    char *label = malloc(MAX_LENGTH * sizeof(char));
    char *url = malloc(MAX_LENGTH * sizeof(char));
    char *username = malloc(MAX_LENGTH * sizeof(char));
    char *email = malloc(MAX_LENGTH * sizeof(char));
    char *password = malloc(MAX_LENGTH * sizeof(char));

    // Get the credentials from the user and store them in a variable
    FILE *file = fopen("credentials.csv", "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Could not open file.\n");
        return -1;
    }

    HPDF_Doc pdf;
    char pdf_file[256];
    HPDF_Page page;
    HPDF_Font font;

    time_t current_time = time(NULL);
    struct tm *local_time = localtime(&current_time);

    char file_name[100];
    sprintf(file_name, "exported_credentials_%02d-%02d-%04d_%02d-%02d-%02d.pdf", local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);

    /* Create a new PDF document */
    pdf = HPDF_New(NULL, NULL);
    if (!pdf)
    {
        printf("Error: Unable to create PDF document.\n");
        return 1;
    }

    // Read the header line
    fgets(line, MAX_LENGTH, file);

    int y = 750;
    int x = 50;

    const int THRESHOLD = 60;
    const int START_Y = y;

    while (fgets(line, MAX_LENGTH, file))
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

        y -= 20;

        HPDF_Page_BeginText(page);
        HPDF_Page_MoveTextPos(page, x, y);
        HPDF_Page_ShowText(page, "Url: ");
        HPDF_Page_ShowText(page, url);
        HPDF_Page_EndText(page);

        y -= 20;

        HPDF_Page_BeginText(page);
        HPDF_Page_MoveTextPos(page, x, y);
        HPDF_Page_ShowText(page, "Username: ");
        HPDF_Page_ShowText(page, username);
        HPDF_Page_EndText(page);

        y -= 20;

        HPDF_Page_BeginText(page);
        HPDF_Page_MoveTextPos(page, x, y);
        HPDF_Page_ShowText(page, "Email: ");
        HPDF_Page_ShowText(page, email);
        HPDF_Page_EndText(page);

        y -= 20;

        HPDF_Page_BeginText(page);
        HPDF_Page_MoveTextPos(page, x, y);
        HPDF_Page_ShowText(page, "Password: ");
        HPDF_Page_ShowText(page, password);
        HPDF_Page_EndText(page);

        y -= 40;

        if (y < THRESHOLD)
        {
            y = START_Y;
        }
    }

    /* Save the PDF file */
    sprintf(pdf_file, file_name);
    HPDF_SaveToFile(pdf, pdf_file);

    /* Clean up and exit */
    HPDF_Free(pdf);
    fclose(file);
    free(line);
    free(label);
    free(url);
    free(username);
    free(email);
    free(password);
    return 0;
}
