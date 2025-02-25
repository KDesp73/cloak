#ifndef HELP_H
#define HELP_H
#include "cli.h"

void CLOAK_Help(CLOAK_Command command);

#ifdef CLOAK_REMOVE_PREFIXES
#define Help CLOAK_Help
#endif // CLOAK_REMOVE_PREFIXES

#endif // HELP_H
