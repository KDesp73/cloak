#ifndef LISTING_H
#define LISTING_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    char** files;
    size_t count;
} CLOAK_List;

#define CLOAK_IGNORE_FILE ".cloakignore"

size_t CLOAK_ListLoad(CLOAK_List* list, const char* path, bool include_gitignore);
size_t CLOAK_LoadIgnore(char*** patterns, bool include_gitignore);
int CLOAK_IsIgnored(const char* filename, char** patterns, size_t pattern_count);
size_t CLOAK_ListLoadEncrypted(CLOAK_List* list, const char* path);

void CLOAK_ListFree(CLOAK_List* list);
void CLOAK_ListPrint(const CLOAK_List* list);

#ifdef CLOAK_REMOVE_PREFIXES
#define List CLOAK_List
#define ListLoad CLOAK_ListLoad
#define LoadIgnore CLOAK_LoadIgnore
#define IsIgnored CLOAK_IsIgnored
#define ListLoadEncrypted CLOAK_ListLoadEncrypted
#define ListFree CLOAK_ListFree
#define ListPrint CLOAK_ListPrint
#endif //CLOAK_REMOVE_PREFIXES


#endif // LISTING_H
