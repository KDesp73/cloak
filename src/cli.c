#include "cli.h"
#include <string.h>

char* CommandToString(Command c)
{
    switch (c) {
    case COMMAND_ENCODE:
        return "encode";
    case COMMAND_DECODE:
        return "decode";

    case COMMAND_NONE:
    default:
        return NULL;
    }
}

Command GetCommand(const char* command)
{
#define COMPARE_AND_RETURN_COMMAND(c) \
    if(!strcmp(command, CommandToString(c))) return c
    
    COMPARE_AND_RETURN_COMMAND(COMMAND_ENCODE);
    else COMPARE_AND_RETURN_COMMAND(COMMAND_DECODE);
    

#undef COMPARE_AND_RETURN_COMMAND
    return COMMAND_NONE;
}

