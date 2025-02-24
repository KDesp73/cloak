#include "cli.h"
#include "version.h"
#include "help.h"

#define CLI_IMPLEMENTATION
#include "extern/cli.h"

#include <stdio.h>

int main(int argc, char** argv){
    cli_args_t args = cli_args_make(
        cli_arg_new('h', "help", "Prints this message", no_argument),
        cli_arg_new('v', "version", "Prints the program version", no_argument),
        NULL
    );
    Command command = GetCommand(argv[1]);

    int opt;
    LOOP_ARGS(opt, args){
        switch (opt) {
            case FLAG_HELP:
                Help(command);
                exit(0);
            case FLAG_VERSION:
                printf("cloak v%s\n", VERSION);
                exit(0);
            default:
                exit(1);
        }
    }

    cli_args_free(&args);

    return 0;
}
