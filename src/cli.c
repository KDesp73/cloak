#include "cli.h"
#include <string.h>

char* CLOAK_CommandToString(CLOAK_Command c)
{
    switch (c) {
    case CLOAK_COMMAND_ENCRYPT:
        return "encrypt";
    case CLOAK_COMMAND_DECRYPT:
        return "decrypt";
    case CLOAK_COMMAND_HASH:
        return "hash";

    case CLOAK_COMMAND_NONE:
    default:
        return NULL;
    }
}

CLOAK_Command CLOAK_GetCommand(const char* command)
{
    if(command == NULL) return CLOAK_COMMAND_NONE;

#define COMPARE_AND_RETURN_COMMAND(c) \
    if(!strcmp(command, CLOAK_CommandToString(c))) return c
    
    COMPARE_AND_RETURN_COMMAND(CLOAK_COMMAND_ENCRYPT);
    else COMPARE_AND_RETURN_COMMAND(CLOAK_COMMAND_DECRYPT);
    else COMPARE_AND_RETURN_COMMAND(CLOAK_COMMAND_HASH);
    
#undef COMPARE_AND_RETURN_COMMAND

    return CLOAK_COMMAND_NONE;
}

