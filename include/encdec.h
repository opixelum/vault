#include <openssl/rsa.h>
#include <openssl/pem.h>

/**
 * @brief Generate a random RSA key pair and save it to the given paths
 * 
 * @param private_key_path The path to save the private key to
 * @param public_key_path The path to save the public key to
 */
void generate_custom_rsa_key(const char* private_key_path, const char* public_key_path);

/**
 * @brief Encrypt a file using RSA
 * 
 * @param input_file The path to the file to encrypt
 * @param output_file The path to save the encrypted file to
 * @param rsa_key The RSA private key to use for encryption
 * @return int 0 on success, -1 on failure
 */
int encrypt_file_rsa(const char* input_file, const char* output_file, RSA* rsa_key);

/**
 * @brief Decrypt a file using RSA
 * 
 * @param input_file The path to the file to decrypt
 * @param output_file The path to save the decrypted file to 
 * @param rsa_key The RSA public key to use for decryption
 * @return int 0 on success, -1 on failure
 */
int decrypt_file_rsa(const char* input_file, const char* output_file, RSA* rsa_key);
