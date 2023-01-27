#include "password.h"
#include "ui.h"
#include "encdec.h"
#include <unistd.h>

int main(int argc, char const *argv[])
{
    unsigned char isRunning = 1;

    while (isRunning)
    {
        mainMenu(&isRunning);
    }

    /* Test encryption and decryption

    // Encrypt "Hello World" with the password "password"
    char *plaintext = "Hello World";
    char *password = "password";
    ENCRYPTED_DATA_T *encrypted_data = encrypt(plaintext, password);

    // Print the encrypted data
    printf("Ciphertext is: ");
    for (int i = 0; i < strlen(plaintext); i++)
    {
        printf("%02x", encrypted_data->ciphertext[i]);
    }

    // Decrypt the ciphertext
    unsigned char *decrypted_text = decrypt(encrypted_data, password);

    // Print the decrypted text
    printf("\nDecrypted text is: %s\n", decrypted_text);

    // Free the memory
    free(encrypted_data->ciphertext);
    free(encrypted_data->iv);
    free(encrypted_data->tag);
    free(encrypted_data);
    free(decrypted_text);
    */

    return 0;
}
