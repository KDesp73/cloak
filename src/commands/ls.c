#include "context.h"
#include "extern/logging.h"
#include "listing.h"
#include "signature.h"
#include <string.h>

int CLOAK_CommandLs(CLOAK_Context* ctx)
{
    if (ctx->type != NULL && !strcmp(ctx->type, "signatures")) {
        CLOAK_SignatureList s = {0};
        CLOAK_LoadSignatures(&s);

        for(size_t i = 0; i < s.count; i++){
            printf("- ");
            for(size_t byte = 0; byte < 16; byte++) {
                printf("%x", s.items[i][byte]);
            }
            printf("...\n");
        }
        CLOAK_FreeSignatures(&s);

        return true;
    }

    const char* path = (ctx->input) ? ctx->input : ".";

    CLOAK_List list = {0};
    CLOAK_ListLoad(&list, path, ctx->include_gitignore, ctx->include_cloakignore);
    CLOAK_ListPrint(&list);
    CLOAK_ListFree(&list);

    return true;
}

