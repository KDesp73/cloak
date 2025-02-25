#include "cli.h"
#include <string.h>

char* CommandToString(Command c)
{
    switch (c) {
    case COMMAND_ENCRYPT:
        return "encrypt";
    case COMMAND_DECRYPT:
        return "decrypt";

    case COMMAND_NONE:
    default:
        return NULL;
    }
}

Command GetCommand(const char* command)
{
    if(command == NULL) return COMMAND_NONE;
#define COMPARE_AND_RETURN_COMMAND(c) \
    if(!strcmp(command, CommandToString(c))) return c
    
    COMPARE_AND_RETURN_COMMAND(COMMAND_ENCRYPT);
    else COMPARE_AND_RETURN_COMMAND(COMMAND_DECRYPT);
    

#undef COMPARE_AND_RETURN_COMMAND
    return COMMAND_NONE;
}

