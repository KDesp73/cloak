#include "commands.h"
#include "aes.h"
#include "compression.h"
#include "config.h"
#include "context.h"
#include "extern/logging.h"
#include "files.h"
#include "rsa.h"
#include "listing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

static int encryptFile(const char* in, const char* out, unsigned char key[CLOAK_KEY_SIZE])
{
    char* output = NULL;
    if (out) {
        output = strdup(out);
        if (!output) {
            ERRO("Failed to allocate memory for output filename.");
            return false;
        }
    } else {
        if (asprintf(&output, "%s.cloak", in) == -1) {
            ERRO("Failed to create output filename.");
            return false;
        }
    }

    char* zip = NULL;
    asprintf(&zip, "%s.bin", in);
    CLOAK_Compress(in, zip);

    if (CLOAK_AESEncryptFile(zip, output, key) != 0) {
        ERRO("Encryption of file %s failed.", in);
        file_delete(zip);
        free(zip);
        free(output);
        return false;
    }
    file_delete(zip);
    free(zip);
    free(output);
    return true;
}

int CLOAK_CommandEncrypt(CLOAK_Context* ctx)
{
    unsigned char key[CLOAK_KEY_SIZE];
    CLOAK_AESGenerateKey(key);

    // Step 1: Sign the AES key BEFORE encryption
    unsigned char signature[CLOAK_RSA_SIGNATURE_SIZE];
    size_t signature_len = 0;

    const char* private_rsa_path = CLOAK_CONFIG_GET_RSA_PRIVATE(&ctx->config);

    if (CLOAK_RSASign(key, CLOAK_KEY_SIZE, private_rsa_path, signature, &signature_len) != 0) {
        ERRO("Failed to sign the AES key");
        return false;
    }

    // Step 2: Encrypt the AES key with RSA
    const char* public_rsa_path = CLOAK_CONFIG_GET_RSA_PUBLIC(&ctx->config);

    unsigned char rsa_encrypted_key[CLOAK_RSA_KEY_SIZE];
    size_t encrypted_key_len;
    if (!CLOAK_RSAEncrypt(key, CLOAK_KEY_SIZE, public_rsa_path, rsa_encrypted_key, &encrypted_key_len)) {
        ERRO("RSA encryption failed");
        return false;
    }

    // Step 3: Encrypt files or directories
    if (ctx->is_dir) {
        CLOAK_List list = {0};
        CLOAK_ListLoad(&list, ctx->input, ctx->include_gitignore, ctx->include_cloakignore);

        for (size_t i = 0; i < list.count; i++) {
            if (!encryptFile(list.files[i], NULL, key))
                ERRO("Could not encrypt '%s'", list.files[i]);
        }

        CLOAK_ListFree(&list);
    } else {
        if (!encryptFile(ctx->input, ctx->output, key))
            return false;
    }

    // Step 4: Save the RSA-encrypted key and signature
    FILE* key_file = fopen(CLOAK_KEY_FILE, "wb");
    if (!key_file) {
        ERRO("Failed to open key file for writing.");
        return false;
    }
    fwrite(rsa_encrypted_key, 1, encrypted_key_len, key_file);
    fclose(key_file);

    FILE* sig_file = fopen(CLOAK_RSA_SIGNATURE_FILE, "wb");
    if (!sig_file) {
        ERRO("Failed to open signature file for writing.");
        return false;
    }
    fwrite(signature, 1, signature_len, sig_file);
    fclose(sig_file);

    return true;
}

