#include "context.h"
#include "extern/logging.h"
#include "listing.h"
#include <string.h>

int CLOAK_CommandLs(CLOAK_Context* ctx)
{
    if(
        ctx->type == NULL ||
        (ctx->type != NULL && !strcmp(ctx->type, "files"))
    ) { // Default behaviour
        const char* path = (ctx->input) ? ctx->input : ".";

        CLOAK_List list = {0};
        CLOAK_ListLoad(&list, path, ctx->include_gitignore, ctx->include_cloakignore);
        CLOAK_ListPrint(&list);
        CLOAK_ListFree(&list);

        return true;
    }

    ERRO("Unknown type '%s'", ctx->type);
    return false;
}

