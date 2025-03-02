#include "commands.h"
#include "config.h"
#include "extern/logging.h"
#include "files.h"
#include <stdio.h>

const char* ini =
"[paths]\n"
"backup = .cloak/backup\n"
"decrypted = .cloak/decrypted\n"
"rsa_private = .cloak/private.pem\n"
"rsa_public = .cloak/public.pem\n"
"\n"
"[ignore]\n"
"include_gitignore = true\n";

int CLOAK_CommandGenerate(CLOAK_Context* ctx)
{
    char* type = ctx->type;

    if(!strcmp(type, "ini")) {
        if(!file_write(CLOAK_CONFIG_FILE, ini)){
            ERRO("Could not create %s", CLOAK_CONFIG_FILE);
            return false;
        } else {
            INFO("%s generated successfully", CLOAK_CONFIG_FILE);
        }
    }
    

    return true;
}
