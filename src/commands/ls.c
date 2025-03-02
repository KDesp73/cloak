#include "context.h"
#include "listing.h"

int CLOAK_CommandLs(CLOAK_Context* ctx)
{
    const char* path = (ctx->input) ? ctx->input : ".";

    CLOAK_List list = {0};
    CLOAK_ListLoad(&list, path, ctx->include_gitignore, ctx->include_cloakignore);
    CLOAK_ListPrint(&list);
    CLOAK_ListFree(&list);

    return true;
}

