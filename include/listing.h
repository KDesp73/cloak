#ifndef LISTING_H
#define LISTING_H

#include <stddef.h>

typedef struct {
    char** files;
    size_t count;
} CLOAK_List;

#define CLOAK_IGNORE_FILE ".cloakignore"

size_t CLOAK_ListLoad(CLOAK_List* list, const char* path);
size_t CLOAK_LoadIgnore(char*** patterns);
int CLOAK_IsIgnored(const char* filename, char** patterns, size_t pattern_count);

void CLOAK_ListFree(CLOAK_List* list);
void CLOAK_ListPrint(const CLOAK_List* list);

#endif // LISTING_H
