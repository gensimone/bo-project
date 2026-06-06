#include "queue.h"
#include <stdio.h>

#define NEW_PAGE "\n %%%\n\n"

void format_page(char** page, Queue* stream_q, const unsigned int cols, const unsigned int lines,
                 const unsigned int width, const unsigned int gap);
void format(FILE* input_stream, FILE* output_stream, const unsigned int cols,
            const unsigned int lines, const unsigned int width, const unsigned int gap);
void load_words(Queue* q, FILE* stream, const unsigned int width);
void print_page(char** page, FILE* stream, const unsigned int lines);
