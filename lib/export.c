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

char exportCredentialsAsPDF()
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

char exportCredentialsAsCSV()
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

    time_t current_time = time(NULL);
    struct tm *local_time = localtime(&current_time);

    char file_name[100];
    sprintf(file_name, "exported_credentials_%02d-%02d-%04d_%02d-%02d-%02d.csv", local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);

    FILE *exported_file = fopen(file_name, "w");
    if (exported_file == NULL)
    {
        fprintf(stderr, "Error: Could not open file.\n");
        return -1;
    }

    // Read the header line
    fgets(line, MAX_LENGTH, file);
    fprintf(exported_file, "Label, URL, Username, Email, Password\n");

    // Write data into new file
    while (fgets(line, MAX_LENGTH, file))
    {
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^\n]", label, url, username, email, password);

        char *csv_text = strcat(label, ",");
        csv_text = strcat(csv_text, url);
        csv_text = strcat(csv_text, ",");
        csv_text = strcat(csv_text, username);
        csv_text = strcat(csv_text, ",");
        csv_text = strcat(csv_text, email);
        csv_text = strcat(csv_text, ",");
        csv_text = strcat(csv_text, password);
        csv_text = strcat(csv_text, "\n");

        if (fprintf(exported_file, "%s", csv_text) == EOF)
        {
            fprintf(stderr, "Error: Could not write to file.\n");
            return -1;
        }
    }

    // Close file
    if (fclose(file) == EOF)
    {
        fprintf(stderr, "Error: Could not close file.\n");
        return -1;
    }

    return 0;
}
