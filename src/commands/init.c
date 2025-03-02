#include "commands.h"
#include "config.h"
#include "extern/logging.h"
#include "files.h"

int CLOAK_CommandInit(CLOAK_Context* ctx)
{
    if(!file_exists(CLOAK_CONFIG_DIRECTORY))
        dir_create(CLOAK_CONFIG_DIRECTORY);
    else {
        WARN("cloak is already initialized in this project");
        return false;
    }

    return true;
}

