#include "rsa.h"
#include "config.h"
#include "extern/logging.h"

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/core_names.h>
#include <stdio.h>

#define RSA_KEY_SIZE 2048
#define PUB_EXPONENT 65537

// NOTE: Using EVP API (OpenSSL 3.0+)
void CLOAK_RSAGenerateKeys()
{
    EVP_PKEY *pkey = NULL;
    EVP_PKEY_CTX *ctx = NULL;
    FILE *private_file = NULL, *public_file = NULL;

    // Create a context for RSA key generation
    ctx = EVP_PKEY_CTX_new_from_name(NULL, "RSA", NULL);
    if (!ctx || EVP_PKEY_keygen_init(ctx) <= 0) {
        ERR_print_errors_fp(stderr);
        goto cleanup;
    }

    // Set RSA key size using OSSL_PARAM
    size_t key_bits = RSA_KEY_SIZE;
    OSSL_PARAM params[] = {
        OSSL_PARAM_construct_size_t(OSSL_PKEY_PARAM_BITS, &key_bits),
        OSSL_PARAM_construct_end()
    };

    if (EVP_PKEY_CTX_set_params(ctx, params) <= 0) {
        ERR_print_errors_fp(stderr);
        goto cleanup;
    }

    // Generate RSA key pair
    if (EVP_PKEY_generate(ctx, &pkey) <= 0) {
        ERR_print_errors_fp(stderr);
        goto cleanup;
    }

    // Save private key
    private_file = fopen(CLOAK_CONFIG_DEFAULT_RSA_PRIVATE, "wb");
    if (!private_file || !PEM_write_PrivateKey(private_file, pkey, NULL, NULL, 0, NULL, NULL)) {
        perror("Failed to write private key");
        goto cleanup;
    }

    // Save public key
    public_file = fopen(CLOAK_CONFIG_DEFAULT_RSA_PUBLIC, "wb");
    if (!public_file || !PEM_write_PUBKEY(public_file, pkey)) {
        perror("Failed to write public key");
        goto cleanup;
    }

cleanup:
    if (private_file) fclose(private_file);
    if (public_file) fclose(public_file);
    EVP_PKEY_free(pkey);
    EVP_PKEY_CTX_free(ctx);
}

bool CLOAK_DeriveKey(const char *password, unsigned char *key, const unsigned char *salt)
{
    if(crypto_pwhash(key, CLOAK_KEY_SIZE, password, strlen(password), salt,
                  crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE,
                  crypto_pwhash_ALG_DEFAULT) != 0) 
        return false;
    return true;
}



// NOTE: Using EVP API (OpenSSL 3.0+)
// For both encryption and decryption

int CLOAK_RSAEncrypt(const unsigned char *aes_key, size_t aes_key_len, 
                     const char *public_key_path, unsigned char *encrypted_key, size_t *encrypted_key_len)
{
    FILE *pub_key_file = fopen(public_key_path, "r");
    if (!pub_key_file) {
        perror("Failed to open public key file");
        return false;
    }

    EVP_PKEY *pubkey = PEM_read_PUBKEY(pub_key_file, NULL, NULL, NULL);
    fclose(pub_key_file);
    if (!pubkey) {
        fprintf(stderr, "Failed to read public key\n");
        return false;
    }

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pubkey, NULL);
    if (!ctx) {
        EVP_PKEY_free(pubkey);
        return false;
    }

    if (EVP_PKEY_encrypt_init(ctx) <= 0) {
        ERR_print_errors_fp(stderr);
        EVP_PKEY_free(pubkey);
        EVP_PKEY_CTX_free(ctx);
        return false;
    }

    // **Ensure OAEP Padding is Set!**
    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) {
        ERR_print_errors_fp(stderr);
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pubkey);
        return false;
    }

    // **Step 1: Get the correct encrypted key size**
    if (EVP_PKEY_encrypt(ctx, NULL, encrypted_key_len, aes_key, aes_key_len) <= 0) {
        ERR_print_errors_fp(stderr);
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pubkey);
        return false;
    }

    // **Step 2: Encrypt AES Key**
    if (EVP_PKEY_encrypt(ctx, encrypted_key, encrypted_key_len, aes_key, aes_key_len) <= 0) {
        ERR_print_errors_fp(stderr);
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pubkey);
        return false;
    }

    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(pubkey);
    return true;
}

int CLOAK_RSADecrypt(const unsigned char* encrypted, size_t encrypted_len, 
                     const char* private_key_path, unsigned char* output, size_t* output_len) 
{
    FILE* key_file = fopen(private_key_path, "rb");
    if (!key_file) {
        ERRO("Failed to open private key file.");
        return false;
    }

    EVP_PKEY* private_key = PEM_read_PrivateKey(key_file, NULL, NULL, NULL);
    fclose(key_file);

    if (!private_key) {
        ERRO("Failed to read private key.");
        return false;
    }

    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(private_key, NULL);
    if (!ctx) {
        EVP_PKEY_free(private_key);
        ERRO("Failed to create context.");
        return false;
    }

    if (EVP_PKEY_decrypt_init(ctx) <= 0) {
        ERR_print_errors_fp(stderr);
        EVP_PKEY_free(private_key);
        EVP_PKEY_CTX_free(ctx);
        return false;
    }

    // **Ensure OAEP Padding Matches the Encryption**
    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) {
        ERR_print_errors_fp(stderr);
        EVP_PKEY_free(private_key);
        EVP_PKEY_CTX_free(ctx);
        return false;
    }

    // **Step 1: Verify Encrypted Key Length**
    size_t decrypted_len = 0;
    if (EVP_PKEY_decrypt(ctx, NULL, &decrypted_len, encrypted, encrypted_len) <= 0) {
        ERR_print_errors_fp(stderr);
        EVP_PKEY_free(private_key);
        EVP_PKEY_CTX_free(ctx);
        return false;
    }

    unsigned char *decrypted = malloc(decrypted_len);
    if (!decrypted) {
        ERRO("Memory allocation failed!");
        EVP_PKEY_free(private_key);
        EVP_PKEY_CTX_free(ctx);
        return false;
    }

    if (EVP_PKEY_decrypt(ctx, decrypted, &decrypted_len, encrypted, encrypted_len) <= 0) {
        ERR_print_errors_fp(stderr);
        free(decrypted);
        EVP_PKEY_free(private_key);
        EVP_PKEY_CTX_free(ctx);
        return false;
    }

    // **Ensure AES Key is Exactly 32 Bytes**
    if (decrypted_len < CLOAK_KEY_SIZE) {
        ERRO("Decrypted key too short!");
        free(decrypted);
        EVP_PKEY_free(private_key);
        EVP_PKEY_CTX_free(ctx);
        return false;
    }

    memcpy(output, decrypted, CLOAK_KEY_SIZE);
    *output_len = CLOAK_KEY_SIZE;
    free(decrypted);

    EVP_PKEY_free(private_key);
    EVP_PKEY_CTX_free(ctx);

    return true;
}

int CLOAK_RSASign(const unsigned char *aes_key, size_t aes_key_len, const char *private_key_path, unsigned char *signature, size_t *signature_len)
{
    FILE *key_file = fopen(private_key_path, "rb");
    if (!key_file) {
        perror("Failed to open private key file for signing.");
        return -1;
    }

    EVP_PKEY *private_key = PEM_read_PrivateKey(key_file, NULL, NULL, NULL);
    fclose(key_file);
    if (!private_key) {
        fprintf(stderr, "Failed to read private key\n");
        return -1;
    }

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(private_key, NULL);
    if (!ctx) {
        EVP_PKEY_free(private_key);
        return -1;
    }

    if (EVP_PKEY_sign_init(ctx) <= 0) {
        ERR_print_errors_fp(stderr);
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(private_key);
        return -1;
    }

    // Set the padding (RSA_PKCS1_PADDING for signatures)
    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING) <= 0) {
        ERR_print_errors_fp(stderr);
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(private_key);
        return -1;
    }

    // Step 1: Determine the size of the signature
    if (EVP_PKEY_sign(ctx, NULL, signature_len, aes_key, aes_key_len) <= 0) {
        ERR_print_errors_fp(stderr);
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(private_key);
        return -1;
    }

    // Step 2: Generate the signature
    if (EVP_PKEY_sign(ctx, signature, signature_len, aes_key, aes_key_len) <= 0) {
        ERR_print_errors_fp(stderr);
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(private_key);
        return -1;
    }

    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(private_key);
    return 0;
}

int CLOAK_RSAVerify(const unsigned char *aes_key, size_t aes_key_len, const unsigned char *signature, size_t signature_len, const char *public_key_path)
{
    FILE *key_file = fopen(public_key_path, "rb");
    if (!key_file) {
        perror("Failed to open public key file for verification.");
        return -1;
    }

    EVP_PKEY *public_key = PEM_read_PUBKEY(key_file, NULL, NULL, NULL);
    fclose(key_file);
    if (!public_key) {
        fprintf(stderr, "Failed to read public key\n");
        return -1;
    }

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(public_key, NULL);
    if (!ctx) {
        EVP_PKEY_free(public_key);
        return -1;
    }

    if (EVP_PKEY_verify_init(ctx) <= 0) {
        ERR_print_errors_fp(stderr);
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(public_key);
        return -1;
    }

    // Set the padding (RSA_PKCS1_PADDING for signatures)
    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING) <= 0) {
        ERR_print_errors_fp(stderr);
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(public_key);
        return -1;
    }

    // Verify the signature
    int verify_result = EVP_PKEY_verify(ctx, signature, signature_len, aes_key, aes_key_len);
    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(public_key);

    return (verify_result == 1) ? 0 : -1;  // Return 0 on success, -1 on failure
}
