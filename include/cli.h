#ifndef CLOAK_CLI_H
#define CLOAK_CLI_H

typedef enum {
    CLOAK_COMMAND_NONE,
    CLOAK_COMMAND_ENCRYPT,
    CLOAK_COMMAND_DECRYPT,
    CLOAK_COMMAND_HASH
} CLOAK_Command;

typedef enum {
    CLOAK_FLAG_HELP    = 'h',
    CLOAK_FLAG_VERSION = 'v',
    CLOAK_FLAG_KEY = 'k',
    CLOAK_FLAG_INPUT = 'i',
    CLOAK_FLAG_OUTPUT = 'o',
} CLOAK_Flag;

char* CLOAK_CommandToString(CLOAK_Command c);
CLOAK_Command CLOAK_GetCommand(const char* command);

#ifdef CLOAK_REMOVE_PREFIXES
#define COMMAND_NONE CLOAK_COMMAND_NONE
#define COMMAND_ENCRYPT CLOAK_COMMAND_ENCRYPT
#define COMMAND_DECRYPT CLOAK_COMMAND_DECRYPT
#define COMMAND_HASH CLOAK_COMMAND_HASH
#define Command CLOAK_Command

#define FLAG_HELP CLOAK_FLAG_HELP
#define FLAG_VERSION CLOAK_FLAG_VERSION
#define FLAG_KEY CLOAK_FLAG_KEY
#define FLAG_INPUT CLOAK_FLAG_INPUT
#define FLAG_OUTPUT CLOAK_FLAG_OUTPUT
#define Flag CLOAK_Flag

#define CommandToString CLOAK_CommandToString
#define GetCommand CLOAK_GetCommand
#endif // CLOAK_REMOVE_PREFIXES

#endif // CLOAK_CLI_H
