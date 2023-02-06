#include "export.h"

char exportCredentials(CREDENTIALS_T credentials)
{
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

    /* Create a new page */
    page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    /* Set the font and font size */
    font = HPDF_GetFont(pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize(page, font, 12);

    /* Write the content of the credentials variable to the page */
    HPDF_Page_BeginText(page);
    HPDF_Page_MoveTextPos(page, 50, 750);
    HPDF_Page_ShowText(page, "Label: ");
    HPDF_Page_ShowText(page, credentials.label);
    HPDF_Page_ShowText(page, "\nURL: ");
    HPDF_Page_ShowText(page, credentials.url);
    HPDF_Page_ShowText(page, "\nUsername: ");
    HPDF_Page_ShowText(page, credentials.username);
    HPDF_Page_ShowText(page, "\nEmail: ");
    HPDF_Page_ShowText(page, credentials.email);
    HPDF_Page_ShowText(page, "\nPassword: ");
    HPDF_Page_ShowText(page, credentials.password);
    HPDF_Page_EndText(page);

    /* Save the PDF file */
    sprintf(pdf_file, file_name);
    HPDF_SaveToFile(pdf, pdf_file);

    /* Clean up and exit */
    HPDF_Free(pdf);
    return 0;
}
