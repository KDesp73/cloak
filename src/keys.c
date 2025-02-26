#include "keys.h"

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

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
        return -1;
    }

    EVP_PKEY *pubkey = PEM_read_PUBKEY(pub_key_file, NULL, NULL, NULL);
    fclose(pub_key_file);
    if (!pubkey) {
        fprintf(stderr, "Failed to read public key\n");
        return -1;
    }

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pubkey, NULL);
    if (!ctx) {
        EVP_PKEY_free(pubkey);
        return -1;
    }

    if (EVP_PKEY_encrypt_init(ctx) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pubkey);
        return -1;
    }

    if (EVP_PKEY_encrypt(ctx, NULL, encrypted_key_len, aes_key, aes_key_len) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pubkey);
        return -1;
    }

    if (EVP_PKEY_encrypt(ctx, encrypted_key, encrypted_key_len, aes_key, aes_key_len) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pubkey);
        return -1;
    }

    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(pubkey);
    return 0;
}

int CLOAK_RSADecrypt(const unsigned char *encrypted_key, size_t encrypted_key_len, 
                    const char *private_key_path, unsigned char *aes_key, size_t *aes_key_len)
{
    FILE *priv_key_file = fopen(private_key_path, "r");
    if (!priv_key_file) {
        perror("Failed to open private key file");
        return -1;
    }

    EVP_PKEY *privkey = PEM_read_PrivateKey(priv_key_file, NULL, NULL, NULL);
    fclose(priv_key_file);
    if (!privkey) {
        fprintf(stderr, "Failed to read private key\n");
        return -1;
    }

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(privkey, NULL);
    if (!ctx) {
        EVP_PKEY_free(privkey);
        return -1;
    }

    if (EVP_PKEY_decrypt_init(ctx) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(privkey);
        return -1;
    }

    if (EVP_PKEY_decrypt(ctx, NULL, aes_key_len, encrypted_key, encrypted_key_len) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(privkey);
        return -1;
    }

    if (EVP_PKEY_decrypt(ctx, aes_key, aes_key_len, encrypted_key, encrypted_key_len) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(privkey);
        return -1;
    }

    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(privkey);
    return 0;
}
