#include "context.h"
#include "hashing.h"

int CLOAK_CommandHash(CLOAK_Context* ctx)
{
    unsigned char hash[CLOAK_HASH_SIZE];
    CLOAK_HashFile(ctx->input, hash);
    CLOAK_HashPrint(hash);

    return true;
}
