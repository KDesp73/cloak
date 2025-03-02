#include "commands.h"
#include "aes.h"
#include "compression.h"
#include "context.h"
#include "extern/logging.h"
#include "files.h"
#include "listing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>


static void removeSubstring(char* str, const char* sub) {
    char* pos;
    size_t len = strlen(sub);

    if (len == 0) return;

    while ((pos = strstr(str, sub)) != NULL) {
        memmove(pos, pos + len, strlen(pos + len) + 1);
    }
}

static char* get_decrypted_path(const char *in)
{
    // Remove ".cloak" extension
    char *temp = strdup(in);
    if (!temp) return NULL;

    removeSubstring(temp, ".cloak");

    // Create a pointer to traverse the temp string safely
    char *trimmed_temp = temp;

    // Skip leading "./" to avoid redundant paths
    while (*trimmed_temp == '.' && *(trimmed_temp + 1) == '/') {
        trimmed_temp += 2; // Move past "./"
    }

    // Allocate memory for the new path
    size_t new_size = strlen(CLOAK_CONFIG_DEFAULT_DECRYPTED) + strlen(trimmed_temp) + 2;
    char *output = malloc(new_size);
    if (!output) {
        perror("Memory allocation failed");
        free(temp);  // Free original strdup memory
        return NULL;
    }

    // Construct the final path
    snprintf(output, new_size, "%s/%s", CLOAK_CONFIG_DEFAULT_DECRYPTED, trimmed_temp);

    free(temp); // Free original strdup memory (safe now)
    return output;
}

static int decryptFile(const char* in, const char* out, unsigned char key[CLOAK_KEY_SIZE])
{
    char* zip = NULL;
    asprintf(&zip, "%s.bin", in);

    if (CLOAK_AESDecryptFile(in, zip, key) != 0) {
        ERRO("Decryption of file %s failed.", in);
        file_delete(zip);
        free(zip);
        return false;
    }

    char* output = NULL;
    if(out) {
        output = strdup(out);
    } else {
        output = get_decrypted_path(in);
        create_parent_directories(output);
    }

    /*
    if(is_file(output)) {
        WARN("%s already exists", output);
        file_delete(zip);
        free(zip);
        free(output);
        return false;
    }
    */

    if(CLOAK_Decompress(zip, output) != 0){
        ERRO("Failed decompressing file %s", in);
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

static int decryptMultiple(const char* in, const char* out, unsigned char key[CLOAK_KEY_SIZE])
{
    CLOAK_List list = {0};
    CLOAK_ListLoadEncrypted(&list, in);

    if (out && !dir_create(out)) { // Ensure the output directory exists
        CLOAK_ListFree(&list);
        return 0;
    }

    for (size_t i = 0; i < list.count; i++) {
        char* output = strdup(list.files[i]);
        removeSubstring(output, ".cloak");

        char* final_output = NULL;
        if (out) {
            // Allocate enough space for output path
            final_output = malloc(strlen(out) + strlen(output) + 2);
            if (!final_output) {
                free(output);
                ERRO("Memory allocation failed");
                CLOAK_ListFree(&list);
                return 0;
            }
            sprintf(final_output, "%s/%s", out, output);

            // Ensure all parent directories exist
            char* last_slash = strrchr(final_output, '/');
            if (last_slash) {
                *last_slash = '\0'; // Temporarily terminate at last directory separator
                dir_create(final_output); // Create the parent directories
                *last_slash = '/'; // Restore full path
            }
        } else {
            final_output = NULL;
        }

        if (!decryptFile(list.files[i], final_output, key)) {
            free(output);
            free(final_output);
            CLOAK_ListFree(&list);
            return false;
        }

        free(output);
        free(final_output);
    }

    CLOAK_ListFree(&list);
    return 1;
}

int CLOAK_CommandDecrypt(CLOAK_Context* ctx)
{
    const char* key_path = (ctx->key) ? ctx->key : CLOAK_KEY_FILE;

    unsigned char key[CLOAK_KEY_SIZE];
    FILE* key_file = fopen(key_path, "rb");
    if (!key_file) {
        ERRO("Failed to open key file for reading.");
        return false;
    }

    // TODO: decrypt using public rsa key

    if (fread(key, 1, CLOAK_KEY_SIZE, key_file) != CLOAK_KEY_SIZE) {
        ERRO("Failed to read the complete key from file.");
        fclose(key_file);
        return false;
    }
    fclose(key_file);

    if (ctx->is_dir) {
        if(!decryptMultiple(ctx->input, ctx->output, key)) {
            INFO("Aborting...");
            return false;
        }
    } else {
        if(!decryptFile(ctx->input, ctx->output, key))
            return false;
    }
    
    return true;
}

