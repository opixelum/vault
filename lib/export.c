#include "export.h"

char exportCredentials()
{
    HPDF_Doc pdf;
    char pdf_file[256];
    HPDF_Page page;
    HPDF_Font font;

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

    /* Write some text to the page */
    HPDF_Page_BeginText(page);
    HPDF_Page_MoveTextPos(page, 50, 750);
    HPDF_Page_ShowText(page, "This is a test PDF file created with libHaru.");
    HPDF_Page_EndText(page);

    /* Save the PDF file */
    sprintf(pdf_file, "test2.pdf");
    HPDF_SaveToFile(pdf, pdf_file);

    /* Clean up and exit */
    HPDF_Free(pdf);
    return 0;
}
