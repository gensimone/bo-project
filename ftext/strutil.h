#include "queue.h"
#include <stddef.h>

unsigned int rstrlen(const char* str);
char* strspace(const size_t n);
unsigned int is_all_space(const char* str);
void sx_align(Queue* q, char* dst);
void bx_align(Queue* q, char* dst, int nspaces);
