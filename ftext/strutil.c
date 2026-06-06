#include "alloc.h"
#include "queue.h"
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/*
  Helper function that allocates and returns a string of length n
  using only the character ' '.
 */
char* strspace(size_t n)
{
  char* str = die_on_fail_calloc(n + 1, sizeof(char));
  memset(str, ' ', n);
  str[n] = '\0';
  return str;
}

/*
  Helper function that returns 0 if the specified string 'str'
  contains only whitespaces.
 */
unsigned short is_all_space(const char* str)
{
  while (*str) {
    if (!isspace((unsigned char) *str))
      return 0;
    str++;
  }
  return 1;
}

/*
  Formats all words in the queue 'q' and writes the result to 'dst'.
  Each word is separated by a single space.
  The caller is responsible for allocating enough space in dst.
  At least: (queue_length(q) * 4) + queue_size(q).
*/
void sx_align(Queue* q, char* dst)
{
  char* word;
  while ((word = queue_pop(q)) != NULL) {
    strcat(dst, word);
    free(word);
    if (queue_size(q) > 0)
      strcat(dst, " ");
  }
}

/*
  Formats all words in the queue 'q' and distributes them by
  the number of spaces 'nspaces' and writes the result to 'dst'.
  The caller is responsible for allocating enough space in dst.
  At least: (queue_length(q) * 4) + nspaces + 1.
*/
void bx_align(Queue* q, char* dst, int nspaces)
{
  char* word;
  int tmp_spaces;
  while ((word = queue_pop(q)) != NULL) {
    /* Insert the word first and then the spaces. */
    strcat(dst, word);
    free(word);
    if (queue_size(q) == 0)
      break;
    tmp_spaces = ceil((double) nspaces / queue_size(q));
    char* spaces = strspace(tmp_spaces);
    strcat(dst, spaces);
    free(spaces);
    nspaces -= tmp_spaces;
  }
}

/*
  Helper function that calculates and returns the 'real' length of a string.
  Allows you to correctly align words containing non-ASCII characters.
 */
unsigned int rstrlen(const char* str)
{
  if (strlen(str) == 0)
    return 0;

  int counter = 0;
  int in_nonascii_char = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] < 0) {
      if (in_nonascii_char == 0)
        in_nonascii_char = 1;
      else
        counter++;
    } else
      in_nonascii_char = 0;
  }

  return strlen(str) - counter;
}
