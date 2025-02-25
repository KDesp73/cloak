#include "aes.h"
#include "cli.h"
#include "commands.h"
#include "context.h"
#include "help.h"
#include "version.h"

#define CLI_IMPLEMENTATION
#include "extern/cli.h"
#define LOGGING_IMPLEMENTATION
#include "extern/logging.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define CLEANUP(exitcode) \
    do { \
        cli_args_free(&args); \
        ContextFree(&ctx); \
        exit(exitcode); \
    } while(0)


int main(int argc, char** argv){
    cli_args_t args = cli_args_make(
        cli_arg_new('h', "help", "Prints this message", no_argument),
        cli_arg_new('v', "version", "Prints the program version", no_argument),
        cli_arg_new('k', "key", "Specify the encryption/decryption key", required_argument),
        cli_arg_new('i', "input", "Specify the input path", required_argument),
        cli_arg_new('o', "output", "Specify the output path", required_argument),
        NULL
    );

    Command command = GetCommand(argv[1]);
    Context ctx = {.command = command};
    ContextInit(&ctx, argc, argv);

    // TODO: Extract in its own function returning the resulting Context
    int opt;
    LOOP_ARGS(opt, args){
        switch (opt) {
            case FLAG_HELP:
                Help(command);
                CLEANUP(0);
            case FLAG_VERSION:
                printf("cloak v%s\n", VERSION);
                CLEANUP(0);
            case FLAG_KEY:
                ctx.key_random = false;
                // TODO: Check argument length
                memcpy(ctx.key, optarg, KEY_SIZE);
                break;
            case FLAG_INPUT:
                ctx.input = strdup(optarg);
                break;
            case FLAG_OUTPUT:
                ctx.output = strdup(optarg);
                break;
            default:
                CLEANUP(1);
        }
    }

    if(!SodiumInit()) return 1;

    if(!ContextValidate(&ctx)) return 1;

    CommandFunc exec = GetCommandFunc(command);
    if(!exec) {
        ERRO("No command specified");
        CLEANUP(1);
    }

    exec(&ctx);

    CLEANUP(0);
}
