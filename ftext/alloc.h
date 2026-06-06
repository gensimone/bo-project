#include <stddef.h>

void* die_on_fail_calloc(size_t nmemb, size_t size);
char** die_on_fail_malloc(size_t size);
void* die_on_fail_realloc(void* ptr, size_t size);
char** die_on_fail_palloc(int nl, int nc);
void free_page(char** pp, int n);
