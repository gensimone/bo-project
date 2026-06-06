#include "core.h"
#include "alloc.h"
#include "queue.h"
#include "strutil.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Read from the stream and return the first word.
  If '\n' is found, a pointer to '\0' is returned.
  If EOF is found, NULL is returned.
 */
char* next_word(FILE* stream)
{
  int c;
  size_t len = 0;

  /* OK, using such a small value seems silly because it could cause several reallocations, */
  /* but in reality, in English, words have an average length of about 5-6. */
  /* Doing so can save a lot of memory, depending on the case and the language used. */

  size_t cap = 6;
  char* buf = die_on_fail_calloc(cap, sizeof(char));

  while ((c = fgetc(stream)) != EOF && isspace(c)) {
    if (c == '\n') {
      buf[0] = '\0';
      return buf;
    }
  }

  /* EOF reached, nothing to do.. */

  if (c == EOF) {
    free(buf);
    return NULL;
  }

  /* Start reading the characters that matters. */

  while (c != EOF && !isspace(c)) {
    /* Capacity exceeded. */
    if (len + 1 >= cap)
      buf = die_on_fail_realloc(buf, cap *= 2);

    buf[len++] = (char) c;
    c = fgetc(stream);
  }

  buf[len] = '\0';
  return buf;
}

/*
  This is probably the worst function, but I don't know how to rewrite it right now.
  The idea is to split the input 'src' when it's longer than 'width'.
*/
static void split_word(Queue* q, char* src, const unsigned int width)
{
  unsigned int src_length = strlen(src);
  if (src_length <= width)
    queue_push(q, src);
  else {
    char* chunk;
    size_t inc_width = 0;
    do {
      chunk = (char*) die_on_fail_calloc(width + 1, sizeof(char));
      strncpy(chunk, src + inc_width, width);
      queue_push(q, chunk);
      inc_width += width;
    } while (src_length > inc_width);
    free(src);
  }
}

/*
  Load the word from stream into q.
  Do not consider multiple consecutive blank lines.
  A single blank line is considered and loaded as ''.
  (We later use '' as "new paragraph command".)
*/
void load_words(Queue* q, FILE* stream, const unsigned int width)
{
  char* word;
  int in_paragraph_divider = 0;
  while ((word = next_word(stream)) != NULL) { // NULL => EOF.
    if (strlen(word) > 0) {
      in_paragraph_divider = 0;
      split_word(q, word, width);
    } else {
      if (in_paragraph_divider == 0) {
        queue_push(q, word);
        in_paragraph_divider = 1;
      } else
        free(word);
    }
  }
}

/* Format a single Page with the provided configuration and using the words in stream_q. */
void format_page(char** page, Queue* stream_q, const unsigned int cols, const unsigned int lines,
                 const unsigned int width, const unsigned int gap)
{
  for (unsigned int c = 0; c < cols; c++) {    // Format columns
    for (unsigned int l = 0; l < lines; l++) { // Format lines

      /* Stream is empty, nothing to do.. */

      if (queue_size(stream_q) == 0)
        return;

      /* We notified ourselves to skip this line because of the new paragraph. */
      /* See load_words. */

      if (is_all_space(queue_head(stream_q))) {
        char* w = queue_pop(stream_q);
        free(w);
        continue;
      }

      /* Consider only the first words that fit within the width. */
      /* Hitting '' means go to the next paragraph. */

      Queue* line_q = queue_create();
      unsigned short new_paragraph = 0;
      while (queue_size(stream_q) > 0 &&
             queue_length(line_q) + rstrlen(queue_head(stream_q)) +
                     (strlen(queue_head(stream_q)) > 0 ? queue_size(line_q)
                                                       : queue_size(line_q) - 1) <=
                 width) {
        char* word = queue_pop(stream_q);
        if (strlen(word) == 0) {
          new_paragraph = 1;
          free(word);
          break;
        }
        queue_push(line_q, word);
      }

      /* Write spaces in order to align the following line of text. */
      /* This version does not suffers from trailing whitespaces. */

      int nspaces = (width * c + c * gap) - rstrlen(page[l]);
      if (nspaces > 0) {
        char* spaces = strspace(nspaces);
        strcat(page[l], spaces);
        free(spaces);
      }

      /* Write the actual line of text. */
      /* Load a line of length 'width' to notify new paragraph. */

      char* line = die_on_fail_calloc((width * 4) + 1, sizeof(char));

      if (new_paragraph) {
        sx_align(line_q, line);
        queue_top(stream_q, strspace(width)); // Placeholder for new paragraph line.
      } else
        bx_align(line_q, line, width - queue_length(line_q));
      strcat(page[l], line);

      /* Free unused memory and assert queue length. */

      assert(queue_size(line_q) == 0);
      free(line);
      free(line_q);
    }
  }
}

/* Print the formatted page to the specified stream. */
void print_page(char** page, FILE* stream, const unsigned int lines)
{
  for (unsigned int l = 0; l < lines; l++)
    if (l < lines - 1 && !strlen(page[l]) && !strlen(page[l + 1]))
      break;
    else
      fprintf(stream, "%s\n", page[l]);
}

/* Format the entire input stream and place the result in the output stream using the specified
 * options. */
void format(FILE* input_stream, FILE* output_stream, const unsigned cols, const unsigned lines,
            const unsigned width, const unsigned gap)
{
  Queue* stream_q = queue_create();
  load_words(stream_q, input_stream, width);

  const size_t page_width = (width * cols + (cols - 1) * gap) * 4;
  char** page;

  do {
    page = die_on_fail_palloc(lines, page_width);
    format_page(page, stream_q, cols, lines, width, gap);
    print_page(page, output_stream, lines);
    if (queue_size(stream_q) > 0)
      fprintf(output_stream, "%s", NEW_PAGE);
    free_page(page, lines);
  } while (queue_size(stream_q) > 0);

  assert(queue_size(stream_q) == 0);
  free(stream_q);
}
