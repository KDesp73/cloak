#include "aes.h"
#include "cli.h"
#include "commands.h"
#include "context.h"
#include "files.h"
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
        cli_arg_new(FLAG_HELP,    "help",    "", no_argument),
        cli_arg_new(FLAG_VERSION, "version", "", no_argument),
        cli_arg_new(FLAG_KEY,     "key",     "", required_argument),
        cli_arg_new(FLAG_INPUT,   "input",   "", required_argument),
        cli_arg_new(FLAG_OUTPUT,  "output",  "", required_argument),
        NULL
    );

    Command command = GetCommand(argv[1]);
    Context ctx = {.command = command};
    ContextInit(&ctx, argc, argv);

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
                ctx.key = strdup(optarg);
                break;
            case FLAG_INPUT:
                ctx.input = strdup(optarg);
                if(is_directory(ctx.input)) 
                    ctx.is_dir = true;
                break;
            case FLAG_OUTPUT:
                ctx.output = strdup(optarg);
                break;
            default:
                CLEANUP(1);
        }
    }

    if(!SodiumInit()) CLEANUP(1);
    if(!ContextValidate(&ctx)) CLEANUP(1);

    CommandFunc exec = GetCommandFunc(command);
    if(!exec) {
        ERRO("No method defined for command '%s'", CommandToString(command));
        CLEANUP(1);
    }

    int rc = !exec(&ctx);

    CLEANUP(rc);
}
