#include "encdec.h"

void generate_custom_rsa_key(const char* private_key_path, const char* public_key_path) {
    // Define the custom private key
    const char* custom_private_key = "your_custom_private_key";

    // Generate the RSA key pair
    RSA* rsa_key = RSA_new();
    BIGNUM* e = BN_new();
    BN_set_word(e, 65537);
    RSA_generate_key_ex(rsa_key, 2048, e, NULL);

    // Set the custom private key
    BN_hex2bn(&rsa_key->d, custom_private_key);

    // Write the private key to a PEM file
    FILE* private_fp = fopen(private_key_path, "wb");
    PEM_write_RSAPrivateKey(private_fp, rsa_key, NULL, NULL, 0, NULL, NULL);
    fclose(private_fp);

    // Write the public key to a PEM file
    FILE* public_fp = fopen(public_key_path, "wb");
    PEM_write_RSAPublicKey(public_fp, rsa_key);
    fclose(public_fp);

    // Clean up
    RSA_free(rsa_key);
    BN_free(e);
}

int encrypt_file_rsa(const char* input_file, const char* output_file, RSA* rsa_key)
{
    int result = 0;
    int rsa_key_size = RSA_size(rsa_key);
    int input_length = 0;
    unsigned char input_buffer[rsa_key_size];
    unsigned char output_buffer[rsa_key_size];

    // Open input and output files
    FILE* input_fp = fopen(input_file, "rb");
    FILE* output_fp = fopen(output_file, "wb");

    // Check if input and output files are opened successfully
    if (!input_fp || !output_fp) {
        result = -1;
        goto close_files;
    }

    // Read input file and encrypt it using RSA public key
    // Write the encrypted data to output file
    while ((input_length = fread(input_buffer, 1, rsa_key_size - 11, input_fp)) > 0) {
        RSA_public_encrypt(input_length, input_buffer, output_buffer, rsa_key, RSA_PKCS1_PADDING);
        fwrite(output_buffer, 1, rsa_key_size, output_fp);
    }

close_files:
    // Close input and output files
    if (input_fp) fclose(input_fp);
    if (output_fp) fclose(output_fp);
    return result;
}

int decrypt_file_rsa(const char* input_file, const char* output_file, RSA* rsa_key)
{
    int result = 0;
    int rsa_key_size = RSA_size(rsa_key);
    int input_length = 0;
    unsigned char input_buffer[rsa_key_size];
    unsigned char output_buffer[rsa_key_size];

    // Open input and output files
    FILE* input_fp = fopen(input_file, "rb");
    FILE* output_fp = fopen(output_file, "wb");

    // Check if input and output files are opened successfully
    if (!input_fp || !output_fp) {
        result = -1;
        goto close_files;
    }

    // Read input file and decrypt it using RSA private key
    // Write the decrypted data to output file
    while ((input_length = fread(input_buffer, 1, rsa_key_size, input_fp)) > 0) {
        RSA_private_decrypt(input_length, input_buffer, output_buffer, rsa_key, RSA_PKCS1_PADDING);
        fwrite(output_buffer, 1, input_length, output_fp);
    }

close_files:
    // Close input and output files
    if (input_fp) fclose(input_fp);
    if (output_fp) fclose(output_fp);
    return result;
}
