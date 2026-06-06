#include "util.h"
#include <stdlib.h>

/*
  Here are defined some memory "allocation" releated functions,
  mainly wrappers to some stdlib allocation methods.
  The main idea here is that if something like malloc fails we
  do not want to continue the execution of the program.
 */

/* Wrap calloc. Die when calloc fails. */
void* die_on_fail_calloc(size_t nmemb, size_t size)
{
  void* ptr = calloc(nmemb, size);
  if (ptr == NULL)
    die("cannot calloc %zu bytes", size);
  return ptr;
}

/* Wrap malloc. Die when malloc fails. */
void* die_on_fail_malloc(size_t size)
{
  void* ptr = malloc(size);
  if (ptr == NULL)
    die("cannot malloc %zu bytes", size);
  return ptr;
}

/* Wrap realloc. Die when ralloc fails. */
void* die_on_fail_realloc(void* ptr, size_t size)
{
  void* rptr = realloc(ptr, size);
  if (rptr == NULL)
    die("cannot realloc %zu bytes", size);
  return rptr;
}

/*
  Allocate a pointer (page) to nl pointers of type char of size nc.
  Use calloc to allocate the pointers.
  Invoke 'die' if calloc fails.
 */
char** die_on_fail_palloc(int nl, int nc)
{
  char** pp = (char**) die_on_fail_calloc(nl, sizeof(char*));
  for (int i = 0; i < nl; i++)
    pp[i] = die_on_fail_calloc(nc, sizeof(char));
  return pp;
}

/*
  Free the pointer pp and all its n pointers associated.
  The pointer pp must have been returned by a previous call
  to die_on_fail_palloc or a similar function.
 */
void free_page(char** pp, int n)
{
  for (int i = 0; i < n; i++)
    free(pp[i]);
  free(pp);
}
